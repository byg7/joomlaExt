<?php
$level=0;
$collapsed='false';
$zipClass = 'ZipArchive';
$zip = new $zipClass;
$ro = new ReflectionObject($zip);
$zipOverWrite = $ro->getConstant('OVERWRITE');
$zipCreate = $ro->getConstant('CREATE');
$zipFile=__DIR__.'/'.time().'.zip';
if($zip->open($zipFile, $zipOverWrite) !== true){
	if($zip->open($zipFile, $zipCreate) !== true){
		header('Content-type: text/html; charset=utf-8');
		echo "error";
		exit;
	}
}
header("Content-type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
header("Content-Disposition: attachment; filename={$report}.xlsx");
$tempString='';
for ($i=0;$i<200;$i++){
	$tempString.=$i.'-'.colName($i).', ';
}
//$zip->addFromString('colName.txt',$tempString);
/* make Sheet */
$tempString='';
$col=array();
$rows='';
$mrg=array();
$col=array();
$sheets=0;
$styles=array();
$rowNo=1;
$colNo=0;
$styles=array(
	'xf'=>array(),
	'xfXml'=>'',
	'numFmt'=>array(),
	'numFmtXml'=>'',
	'font'=>array(),
	'fontXml'=>'',
	'fill'=>array(),
	'fillXml'=>'',
);
$row=array();
foreach ($document as $rowkey=>$cell){
	if(isset($cell['tabname'])){
		if(strlen($rows)>0){
			$sheets++;
			$zip->addFromString("xl/worksheets/sheet".$sheets.".xml",sheetXlsx($rows,$mrg,$colNo,$rowNo,$col));
		}
		$rowNo=1;
		$colNo=0;
		$mrg=array();
		$col=array();
		$rows='<row r="'.$rowNo.
'" customFormat="false" ht="12.8" hidden="false" customHeight="false" outlineLevel="0" collapsed="false">';
	}
	if(isset($cell['tr'])){
		if(is_array($cell['tr'])){
			$row=$cell['tr'];
		}else{
			$row=array();
		}
		$rowNo++;
		$colNo=0;
		if(isset($cell['level'])){
			// Здесь уровень СЛЕДУЮЩЕЙ строки, а текущий на 1 меньше
			if($level>($cell['level']-1)){
				$collapsed='true';
			}else{
				$collapsed='false';
			}
			$level=$cell['level'];
			$rows.='
</row>
<row r="'.$rowNo.'" customFormat="false" ht="12.8" hidden="';
			if($level>1){
				$rows.='true';
			}else{
				$rows.='false';
			}
			$rows.='" customHeight="false" outlineLevel="'.($level-1).'" collapsed="'.$collapsed.'">';
		}else{
			$rows.='
</row>
<row r="'.$rowNo.'" customFormat="false" ht="12.8" hidden="';
			if($level>0){
				$rows.='true';
			}else{
				$rows.='false';
			}
			$rows.='" customHeight="false" outlineLevel="'.$level.'" collapsed="'.$collapsed.'">';
		}
	}
	if(isset($cell['col'])){
		$col[$cell['col']]=$cell;
	}
	if(isset($cell['colno'])){
		$colNo=$cell['colno']-1;
	}
	if(isset($cell['cs'])){
		if(isset($cell['rs'])){
			$mrg[]=colName($colNo).$rowNo.':'.colName($colNo+$cell['cs']-1).($rowNo+$cell['rs']-1);
		}else{
			$mrg[]=colName($colNo).$rowNo.':'.colName($colNo+$cell['cs']-1).$rowNo;
		}
	}elseif(isset($cell['rs'])){
		$mrg[]=colName($colNo).$rowNo.':'.colName($colNo).($rowNo+$cell['rs']-1);
	}
// Таблица стилей
	$style['text-align']='Center';
	$style['color']='000';
	$style['bcolor']='fff';
	if(isset($col[$colNo])){
		$style=addstyle2003($col[$colNo],$style);
	}
	if(isset($cell['format'])){
		$format=$cell['format'];
	}elseif(isset($row['format'])){
		$format=$row['format'];
	}elseif(isset($col[$colNo]['format'])){
		$format=$col[$colNo]['format'];
	}else{
		$format='';
	}
	$style=addstyle2003($row,$style);
	$style=addstyle2003($cell,$style);
	if(strlen($style['color'])==3){
		$style['color']=substr($style['color'],0,1).substr($style['color'],0,1).
			substr($style['color'],1,1).substr($style['color'],1,1).
			substr($style['color'],2,1).substr($style['color'],2,1);
		$colorArray[$style['color']]=1;
	}
	if(strlen($style['bcolor'])==3){
		$style['bcolor']=substr($style['bcolor'],0,1).substr($style['bcolor'],0,1).
			substr($style['bcolor'],1,1).substr($style['bcolor'],1,1).
			substr($style['bcolor'],2,1).substr($style['bcolor'],2,1);
		$colorArray[$style['bcolor']]=1;
	}
// преобразуем формат
	$phpDate=0;
	if(strlen($format)>0){
		$phpDate=1;
		if((substr($format,0,1)=='n')or(substr($format,0,1)=='%')){
			if(substr($format,0,1)=='%'){
				$fPos='%';
			}else{
				$fPos='';
			}
			$char2=(int)substr($format,1,1);
			if($char2>4){
				$fPre='#,##0';
			}else{
				$fPre='0';
			}
			if(($char2==0)or($char2==5)){
				$format=$fPre.$fPos;
			}else{
				$format=$fPre.'.'.str_repeat('0',$char2 % 5).$fPos;
			}
			$phpDate=0;
		}elseif($format=='dg'){
			$format='dd.mm.yy';
		}elseif($format=='dG'){
			$format='dd.mm.yyyy';
		}elseif($format=='dgt'){
			$format='dd.mm.yy hh:mm:ss';
		}elseif($format=='dGt'){
			$format='dd.mm.yyyy hh:mm:ss';
		}
	}
	$xfKey='t'.$style['text-align'].
		'v'.
		'c'.$style['color'].
		'b'.$style['bcolor'].
		'f'.$format;
	if(!isset($styles['xf'][$xfKey])){
		if(!isset($styles['numFmt'][$format])){
			if(strlen($format)>0){
				$numFmtNo=count($styles['numFmt']);
				$numFmtNo+=99;
				$styles['numFmt'][$format]=$numFmtNo;
				$styles['numFmtXml'].='
<numFmt numFmtId="'.$numFmtNo.'" formatCode="'.$format.'"/>';
			}else{
				$numFmtNo=0;
			}
		}else{
			$numFmtNo=$styles['numFmt'][$format];
		}
		if(!isset($styles['font'][$style['color']])){
			$fontNo=count($styles['font']);
//			$fontNo+=0;
			$styles['font'][$style['color']]=$fontNo;
			$styles['fontXml'].='
<font>
<b val="0"/><i val="0"/>
<u val="none"/><color rgb="FF'.strtoupper($style['color']).'"/>
<name val="Arial"/><vertAlign val="baseline"/>
<sz val="10"/><strike val="0"/>
</font>';
		}else{
			$fontNo=$styles['font'][$style['color']];
		}
		if(!isset($styles['fill'][$style['bcolor']])){
			$fillNo=count($styles['fill']);
//			$fontNo+=1;
			$styles['fill'][$style['bcolor']]=$fillNo;
			$styles['fillXml'].='
<fill><patternFill patternType="solid"><fgColor rgb="FF'.strtoupper($style['bcolor']).
'"/><bgColor rgb="FF'.strtoupper($style['bcolor']).'"/></patternFill></fill>';
		}else{
			$fillNo=$styles['fill'][$style['bcolor']];
		}
		$xfNo=count($styles['xf']);
//		$xfNo+=1;
		$styles['xf'][$xfKey]=$xfNo;
		$styles['xfXml'].='
<xf numFmtId="'.$numFmtNo.'" fontId="'.($fontNo+1).'" fillId="'.($fillNo+2).
'" borderId="1" xfId="0" applyFont="true" applyBorder="true" applyAlignment="true" applyProtection="false">
<alignment horizontal="'.strtolower($style['text-align']).'" vertical="'.'top'.
'" textRotation="0" wrapText="true" indent="0" shrinkToFit="false"/>
</xf>';
	}else{
		$xfNo=$styles['xf'][$xfKey];
	}
	if(isset($cell['th'])){
//echo $cell['th'].' '.$phpDate.' "'.$format.'" ';
		if($phpDate==1){
			$rows.=stringCellXlsx(phpDateToExcel($cell['th']),$colNo,$rowNo,$xfNo);
		}else{
			$rows.=stringCellXlsx($cell['th'],$colNo,$rowNo,$xfNo);
		}
		if(isset($cell['cs'])){
			$colNo+=$cell['cs'];
		}else{
			$colNo++;
		}
	}
	if(isset($cell['td'])){
//echo $cell['td'].' '.$phpDate.' ';
		if($phpDate==1){
			$rows.=stringCellXlsx(phpDateToExcel($cell['td']),$colNo,$rowNo,$xfNo);
		}else{
			$rows.=stringCellXlsx($cell['td'],$colNo,$rowNo,$xfNo);
		}
		if(isset($cell['cs'])){
			$colNo+=$cell['cs'];
		}else{
			$colNo++;
		}
//		$colNo++;
	}
}
$sheets++;
//echo '$sheets='.$sheets;
//die();
$zip->addFromString("xl/worksheets/sheet".$sheets.".xml",sheetXlsx($rows,$mrg,$colNo,$rowNo,$col));
$numfmt=array();
$fonts=array();
$fills=array();
$xfs=array();
$tempString='<?xml version="1.0" encoding="UTF-8"?>
<styleSheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
';
if(count($styles['numFmt'])>0){
	$tempString.='<numFmts count="'.(count($styles['numFmt'])).'">'.$styles['numFmtXml'].'
</numFmts>
';
}
$tempString.='<fonts count="'.(count($styles['font'])+1).'">
<font>
<b val="0"/>
<i val="0"/>
<u val="none"/>
<color rgb="FF000000"/>
<name val="Sans"/>
<vertAlign val="baseline"/>
<sz val="10"/>
<strike val="0"/>
</font>'.$styles['fontXml'].'
</fonts>
';
$tempString.='<fills count="'.(count($styles['fill'])+2).'">
<fill><patternFill patternType="none"/></fill>
<fill><patternFill patternType="gray125"/></fill>'.$styles['fillXml'].'
</fills>
<borders count="2">
<border diagonalUp="false" diagonalDown="false"><left/><right/><top/><bottom/><diagonal/></border>
<border diagonalUp="false" diagonalDown="false"><left style="hair"/><right style="hair"/><top style="hair"/><bottom style="hair"/><diagonal/></border>
</borders>
<cellStyleXfs count="1">
<xf numFmtId="0" fontId="0" fillId="0" borderId="0"/>
</cellStyleXfs>
<cellXfs count="'.(count($styles['xf'])).'">'.$styles['xfXml'].'
</cellXfs>
</styleSheet>';
/*
<cellStyles count="6">
<cellStyle name="Normal" xfId="0" builtinId="0" customBuiltin="false"/>
<cellStyle name="Comma" xfId="15" builtinId="3" customBuiltin="false"/>
<cellStyle name="Comma [0]" xfId="16" builtinId="6" customBuiltin="false"/>
<cellStyle name="Currency" xfId="17" builtinId="4" customBuiltin="false"/>
<cellStyle name="Currency [0]" xfId="18" builtinId="7" customBuiltin="false"/>
<cellStyle name="Percent" xfId="19" builtinId="5" customBuiltin="false"/>
</cellStyles>
*/
$zip->addFromString("xl/styles.xml",$tempString);
$zip->addFromString('[Content_Types].xml','<?xml version="1.0" encoding="UTF-8"?><Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types"><Override PartName="/_rels/.rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/><Override PartName="/docProps/app.xml" ContentType="application/vnd.openxmlformats-officedocument.extended-properties+xml"/><Override PartName="/docProps/core.xml" ContentType="application/vnd.openxmlformats-package.core-properties+xml"/><Override PartName="/xl/_rels/workbook.xml.rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/><Override PartName="/xl/sharedStrings.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml"/><Override PartName="/xl/worksheets/sheet1.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml"/><Override PartName="/xl/styles.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml"/><Override PartName="/xl/workbook.xml" ContentType="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"/></Types>');
$zip->addFromString('_rels/.rels', '<?xml version="1.0" encoding="UTF-8"?><Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="xl/workbook.xml"/><Relationship Id="rId2" Type="http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties" Target="docProps/core.xml"/><Relationship Id="rId3" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties" Target="docProps/app.xml"/></Relationships>');
$zip->addFromString("docProps/app.xml", '<?xml version="1.0" encoding="UTF-8" standalone="yes"?><Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties" xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"><TotalTime>0</TotalTime><Application>LibreOffice/4.2.8.2$Linux_x86 LibreOffice_project/420m0$Build-2</Application></Properties>');
$zip->addFromString("docProps/core.xml", '<?xml version="1.0" encoding="UTF-8" standalone="yes"?><cp:coreProperties xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/" xmlns:dcmitype="http://purl.org/dc/dcmitype/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"><dcterms:created xsi:type="dcterms:W3CDTF">2016-04-04T13:29:40Z</dcterms:created><dc:language>ru-RU</dc:language><cp:revision>0</cp:revision></cp:coreProperties>');
$tempString='<?xml version="1.0" encoding="UTF-8"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles" Target="styles.xml"/>';
for ($i=1;$i<$sheets+1;$i++){
	$tempString.='<Relationship Id="rId'.($i+1).'" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet" Target="worksheets/sheet'.$i.'.xml"/>';
}
$tempString.='</Relationships>';
$zip->addFromString("xl/_rels/workbook.xml.rels", $tempString);
$tempString='<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<workbook xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
<fileVersion appName="Calc"/>
<workbookPr backupFile="false" showObjects="all" date1904="false"/>
<workbookProtection/>
<bookViews>
<workbookView showHorizontalScroll="true" showVerticalScroll="true" showSheetTabs="true" xWindow="0" yWindow="0" windowWidth="16384" windowHeight="8192" tabRatio="234" firstSheet="0" activeTab="0"/>
</bookViews>
<sheets>';
for ($i=1;$i<$sheets+1;$i++){
	$tempString.='<sheet name="Sheet'.$i.'" sheetId="1" state="visible" r:id="rId'.($i+1).'"/>';
}
$tempString.='</sheets>
<calcPr iterateCount="100" refMode="A1" iterate="false" iterateDelta="0.001"/>
</workbook>';
$zip->addFromString("xl/workbook.xml", $tempString);
$zip->close();
echo file_get_contents($zipFile);
unlink($zipFile);
function stringCellXlsx($value,$colNo,$rowNo,$xfNo){
//	echo $value.' '.colName($colNo).$rowNo.'
//';
	if(is_numeric($value)){
		return '
<c r="'.colName($colNo).$rowNo.'" t="n" s="'.$xfNo.'"><v>'.$value.'</v></c>';
	}elseif(strlen($value)>0){
		return '
<c r="'.colName($colNo).$rowNo.'" t="str" s="'.$xfNo.'"><v>'.htmlspecialchars($value).'</v></c>';
	}else{
		return '
<c r="'.colName($colNo).$rowNo.'" />';
	}
}
function sheetXlsx($rows,$mrg,$colNo,$rowNo,$col){
	$sheet='<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<worksheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
<sheetPr filterMode="false"><pageSetUpPr fitToPage="false"/></sheetPr>
<dimension ref="A1:'.colName($colNo-2).($rowNo-1).'" />
<sheetViews>
<sheetView tabSelected="1" workbookViewId="0"/>
</sheetViews>
<sheetFormatPr defaultRowHeight="12.8" />
<cols>';
	for ($i=0;$i<$colNo;$i++){
		if(isset($col[$i]['width'])){
			$sheet.='
<col collapsed="false" hidden="false" max="'.($i+1).'" min="'.($i+1).
				'" style="0" width="'.($col[$i]['width']/5).'"/>';
		}else{
			$sheet.='
<col collapsed="false" hidden="false" max="'.($i+1).'" min="'.($i+1).
				'" style="0" width="12"/>';
		}
	}
//	$sheet.='<col collapsed="false" hidden="false" max="'.($colNo-1).'" min="1" style="0" width="12"/>';
	if($level>0){
		$rowNo++;
		$rows.='
</row>
<row r="'.$rowNo.'" customFormat="false" ht="12.8" hidden="false" customHeight="false" outlineLevel="0" collapsed="true">';
	}
	$sheet.='<col collapsed="false" hidden="false" max="1025" min="'.
($colNo+1).'" style="0" width="12"/></cols>
<sheetData>'.$rows.'
</row>
</sheetData>
<mergeCells count="'.count($mrg).'">';
	foreach($mrg as $m) {
		$sheet.='
<mergeCell ref="'.$m.'"/>';
	}
	$sheet.='
</mergeCells>
</worksheet>';
	return $sheet;
}
