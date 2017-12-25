<?php
header("Content-type: application/vnd.ms-excel; charset=utf-8");
header("Content-Disposition: attachment; filename={$report}.xml");
$sheet='';
$rows='';
$styleArray=array();
$colorArray=array();
$notable=1;
$nobody=1;
$col=array();
$colno=0;
$colnoMax=0;
$row=array();
foreach($document as $rowkey=>$cell){
	$spans='';
	$style=array();
	$format='';
	if(isset($cell['col'])){
		$col[$cell['col']]=$cell;
	}
	if(isset($cell['rs'])){
		$i=$cell['rs']-1;
		$spans="ss:MergeDown='{$i}'";
	}
	if(isset($cell['cs'])){
		$i=$cell['cs']-1;
		$spans="ss:MergeAcross='{$i}'";
	}
	if(isset($cell['tabname'])){
		$row=array();
		if($notable==0){
			for ($i=0;$i<$colnoMax;$i++){
				$sheet.='<Column ss:Width="'.($i*10+60).'"/>';
			}
			$sheet.=$rows.'</Row></Table><x:WorksheetOptions/></ss:Worksheet>';
		}
		$rows='
<Row ss:Height="12">';
		if(is_numeric($cell['tabname'])){
			if($cell['tabname']==0){
				$v=$cell['th'];
				if(strlen($v)>20){
					$v='Лист'.$rowkey;
				}
			}else{
				$v='Лист'.$cell['tabname'];
			}
		}else{
			$v=$cell['tabname'];
			if(strlen($v)>20){
				$rows='
<Row ss:Height="12">
<Cell ><Data ss:Type="String">'.htmlspecialchars($v).'</Data></Cell>
</Row>
<Row ss:Height="12">';
				$v='Лист'.$rowkey;
			}elseif(strlen($v)==0){
				$v='Лист'.$rowkey;
			}
		}
		$v=htmlspecialchars($v);
		$sheet.='
<ss:Worksheet ss:Name="'.$v.'"><Table ss:StyleID="ta1">';
		$notable=0;
		$col=array();
		$colno=0;
	}
	if (isset($cell['tr'])) {
		$rows.='
</Row>
<Row ss:Height="12">';
		$row=$cell['tr'];
		if($colnoMax<$colno)
			$colnoMax=$colno;
		$colno=0;
	}
	$style['text-align']='Center';
	$style['color']='000';
	$style['bcolor']='fff';
	if(isset($col[$colno])){
		$style=addstyle2003($col[$colno],$style);
	} 
	if(isset($cell['format'])){
		$format=$cell['format'];
	}elseif (isset($row['format'])){
		$format=$row['format'];
	}elseif (isset($col[$colno]['format'])){
		$format=$col[$colno]['format'];
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
	if((isset($cell['th']))or(isset($cell['td']))){
		$v=0;
		if(isset($cell['td'])){
			$v=$cell['td'];
		}elseif(isset($cell['th'])){
			$v=$cell['th'];
		}
		if((strlen($format)>0)and(strlen($v)>0)){
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
			}elseif($format=='dg'){
				$format='dd.mm.yy';
				$v=phpDateToExcel($v);
			}elseif($format=='dG'){
				$format='dd.mm.yyyy';
				$v=phpDateToExcel($v);
			}elseif($format=='dgt'){
				$format='dd.mm.yy hh:nn:ss';
				$v=phpDateToExcel($v);
			}elseif($format=='dGt'){
				$format='dd.mm.yyyy hh:nn:ss';
				$v=phpDateToExcel($v);
			}
		}else{
			$format='';
		}
		$stl_k='t'.$style['text-align'].'c'.$style['color'].'b'.$style['bcolor'].'f'.$format;
		if(isset($styleArray[$stl_k])){
			$stl_no=$styleArray[$stl_k][0];
		}else{
			$stl_no=count($styleArray)+1;
			$styleArray[$stl_k][0]=$stl_no;
			$stl='<Style ss:ID="ce'.$stl_no.'"><Alignment ss:Horizontal="'.$style['text-align'].
							'" ss:Vertical="Top" ss:WrapText="1" ss:Indent="0" ss:Rotate="0"/><Borders><Border ss:Position="Bottom" ss:LineStyle="Continuous" ss:Weight="1" ss:Color="#000000"/>'.
							'<Border ss:Position="Left" ss:LineStyle="Continuous" ss:Weight="1" ss:Color="#000000"/><Border ss:Position="Right" ss:LineStyle="Continuous" ss:Weight="1" ss:Color="#000000"/>'.
							'<Border ss:Position="Top" ss:LineStyle="Continuous" ss:Weight="1" ss:Color="#000000"/></Borders>'.
							'<Font ss:Color="#'.$style['color'].'" ss:FontName="Arial" ss:Size="10"/>';
			if(strlen($format)>0){
				$stl.='<NumberFormat ss:Format="'.$format.'"/>';
			}
			$stl.='<Interior ss:Color="#'.$style['bcolor'].'" ss:Pattern="Solid"/></Style>
';
			$styleArray[$stl_k][1]=$stl;
		}
		if(isset($cell['a'])){
			$a=str_replace('|','
',$v);
		}else{
			$a=str_replace('|','
',$v);
		}
		$index='';
		if(isset($cell['colno'])){
			$index='ss:Index="'.$cell['colno'].'" ';
		}
		if(($format!=='')and(is_numeric($v))){
			$rows.='
<Cell '.$index.$spans.' ss:StyleID="ce'.$stl_no.'"><Data ss:Type="Number">'.$a.'</Data></Cell>';
		}elseif ($v!==''){
			$rows.='
<Cell '.$index.$spans.' ss:StyleID="ce'.$stl_no.'"><Data ss:Type="String">'.htmlspecialchars($a).'</Data></Cell>';
		}else{
			$rows.='
<Cell '.$index.$spans.' ss:StyleID="ce'.$stl_no.'" />';
		}
	}
	$colno++;
}
if($notable==0){
	for($i=1;$i<$colnoMax;$i++){
		if(isset($col[$i-1]['width'])){
			$sheet.='
<Column ss:Width="'.$col[$i-1]['width'].'"/>';
		}else{
			$sheet.='
<Column ss:Width="60"/>';
		}
	}
	$sheet.=$rows.'
</Row>
</Table><x:WorksheetOptions/></ss:Worksheet>
';
}
echo '<?xml version="1.0" encoding="UTF-8"?><?mso-application progid="Excel.Sheet"?>
<Workbook xmlns="urn:schemas-microsoft-com:office:spreadsheet" xmlns:c="urn:schemas-microsoft-com:office:component:spreadsheet" xmlns:html="http://www.w3.org/TR/REC-html40" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:ss="urn:schemas-microsoft-com:office:spreadsheet" xmlns:x2="http://schemas.microsoft.com/office/excel/2003/xml" xmlns:x="urn:schemas-microsoft-com:office:excel" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
<OfficeDocumentSettings xmlns="urn:schemas-microsoft-com:office:office">
<Colors>';
$v=3;
foreach ($colorArray as $cKey=>$cVal){
	echo '
<Color><Index>'.$v.'</Index><RGB>#'.$cKey.'</RGB></Color>';
	$v++;
}
echo '
</Colors>
</OfficeDocumentSettings>
<ExcelWorkbook xmlns="urn:schemas-microsoft-com:office:excel"><WindowHeight>9000</WindowHeight><WindowWidth>13860</WindowWidth>
<WindowTopX>240</WindowTopX><WindowTopY>75</WindowTopY><ProtectStructure>False</ProtectStructure><ProtectWindows>False</ProtectWindows></ExcelWorkbook>
';
// Может изменяться
echo '<Styles>
<Style ss:ID="Default" ss:Name="Default"><Alignment ss:Vertical="Bottom"/><Font ss:FontName="Arial"/></Style>
<Style ss:ID="Result" ss:Name="Result"><Alignment ss:Vertical="Bottom"/><Font ss:Bold="1" ss:FontName="Arial" ss:Italic="1" ss:Underline="Single"/></Style>
<Style ss:ID="Result2" ss:Name="Result2"><Alignment ss:Vertical="Bottom"/><Font ss:Bold="1" ss:FontName="Arial" ss:Italic="1" ss:Underline="Single"/>
<NumberFormat ss:Format="Currency"/></Style>
<Style ss:ID="Heading" ss:Name="Heading"><Alignment ss:Vertical="Bottom"/><Font ss:Bold="1" ss:FontName="Arial" ss:Italic="1" ss:Size="16"/></Style>
<Style ss:ID="Heading1" ss:Name="Heading1"><Alignment ss:Vertical="Bottom" ss:Rotate="90"/>
<Font ss:Bold="1" ss:FontName="Arial" ss:Italic="1" ss:Size="16"/></Style>
<Style ss:ID="co1"/><Style ss:ID="co2"/><Style ss:ID="co3"/><Style ss:ID="co4"/>
<Style ss:ID="co5"/><Style ss:ID="co6"/><Style ss:ID="co7"/><Style ss:ID="co8"/>
<Style ss:ID="co9"/><Style ss:ID="co10"/><Style ss:ID="co11"/>
<Style ss:ID="ta1"/>
';
foreach ($styleArray as $stl) {
	echo $stl[1];
}
echo '</Styles>',$sheet,'</Workbook>';
