<?php
$col=array();
$colNo=0;
$cols=0;
foreach($document as $rowkey=>$cell){
	if(isset($cell['col'])){
		$col[$cell['col']]=$cell;
	}
	if(isset($cell['tr'])){
		if($colNo>$cols){
			$cols=$colNo;
		}
		$colNo=0;
	}
	if((isset($cell['th']))or(isset($cell['td']))){
		if(isset($cell['cs'])){
			$colNo+=$cell['cs'];
		}else{
			$colNo++;
		}
	}
}
$w=0;
for($i=0;$i<$cols;$i++){
	if(!isset($col[$i]['width'])){
		$col[$i]['width']=50;
	}
	$w+=$col[$i]['width'];
}
if(isset($col[0]['page'])){
	if($col[0]['page']=='L'){
		$wp=270;
	}else{
		$wp=190;
	}
}elseif($cols>6){
	$wp=270;
}else{
	$wp=190;
}
if($wp>($w/2)){
	$wp=$w/2;
}
for($i=0;$i<$cols;$i++){
	$col[$i]['width']=round($col[$i]['width']*$wp/$w);
	if($i>0){
		$col[$i]['pdfX']=$col[$i-1]['pdfX']+$col[$i-1]['width'];
	}else{
		$col[$i]['pdfX']=12;
	}
}
$row=array();
$posY=12;
$colNo=0;
$pdfRow=array();
$pdfLine=array();
require('tfpdf.php');
$pdf = new tFPDF();
if(isset($col[0]['page'])){
	if($col[0]['page']=='L'){
		$pdf->AddPage('L');
		$col[0]['page']=180;
	}else{
		$pdf->AddPage('P');
		$col[0]['page']=280;
	}
}elseif($cols>6){
	$pdf->AddPage('L');
	$col[0]['page']=180;
}else{
	$pdf->AddPage('P');
	$col[0]['page']=280;
}
// Add a Unicode font (uses UTF-8)
$pdf->AddFont('DejaVu','','DejaVuSansCondensed.ttf',true);
$pdf->SetFont('DejaVu','',10);
$pdf->SetDrawColor(128,128,128);
$pdf->SetAutoPageBreak(1,0);
foreach($document as $rowkey=>$cell){
	if(isset($cell['tr'])){
		if(is_array($cell['tr'])){
			$row=$cell['tr'];
		}else{
			$row=array();
		}
		$res=pdfPrintRow($pdf,$pdfRow,$posY,$col);
		$posY=$res[0];
		$pdfRow=$res[1];
		$colNo=0;
	}
	while(isset($pdfRow[$colNo])){
		if(isset($pdfRow[$colNo]['cs'])){
			$colNo+=$pdfRow[$colNo]['cs'];
		}else{
			$colNo++;
		}
	}
	if((isset($cell['th']))or(isset($cell['td']))){
		if(isset($cell['th'])){
			$val=$cell['th'];
		}else{
			$val=$cell['td'];
		}
		$pdfColNo=$colNo;
		$pdfRow[$pdfColNo]=array();
		$pdfRow[$pdfColNo]['txt']=str_replace('|',"\n",$val);
		if(isset($cell['cs'])){
			$pdfRow[$pdfColNo]['cs']=$cell['cs'];
			$colNo+=$cell['cs'];
		}else{
			$pdfRow[$pdfColNo]['cs']=1;
			$colNo++;
		}
		if(isset($cell['rs'])){
			$pdfRow[$pdfColNo]['rs']=$cell['rs'];
		}else{
			$pdfRow[$pdfColNo]['rs']=1;
		}
		$pdfRow[$pdfColNo]['bcolor']=pdfStyle($col[$colNo],$row,$cell,'bcolor');;
		$pdfRow[$pdfColNo]['color']=pdfStyle($col[$colNo],$row,$cell,'color');;
		$pdfRow[$pdfColNo]['align']=pdfStyle($col[$colNo],$row,$cell,'align');;
	}
}
$posY=pdfPrintRow($pdf,$pdfRow,$posY,$col);
$pdf->Output();
/*
Функция выводит строку таблицы
pdf - объект tfpdf фактически генератор pdf
pdfRows - массив вида [C][prop][N], где
	C - номер солонки;
	prop - название свойства (cs,rs,color,bcolor,txt,lines);
	N - только для lines - номер строки:
posY - текущая позиция на странице;
col - массив колонок (их свойств);
*/
function pdfPrintRow($pdf,$pdfRow,$posY,$col){
	if(count($pdfRow)>0){ // Выводим строку таблицы
		foreach($pdfRow as $pdfKey=>$pdfCell){ // распарсим строки
			if(!isset($pdfCell['lines'])){
				$lastKey=$pdfKey+$pdfCell['cs']-1;
				$pdfRow[$pdfKey]['lines']=pdfText2Lines($pdf,$pdfCell['txt'],
					$col[$lastKey]['pdfX']-$col[$pdfKey]['pdfX']+$col[$lastKey]['width']);
			}
//			$pdfRow[$pdfKey]['lines'][0]=$pdfCell['rs'] .count($pdfRow[$pdfKey]['lines']) . ' '. $pdfRow[$pdfKey]['lines'][0];
		}
		$lines=1;
		foreach($pdfRow as $pdfKey=>$pdfCell){ // Определяем их к-во
			if($pdfCell['rs']==1){
				if($lines<count($pdfCell['lines'])){
					$lines=count($pdfCell['lines']);
				}
			}
		}
		for($i=0;$i<$lines;$i++){ // Выводим на страницу
			foreach($pdfRow as $pdfKey=>$pdfCell){
				$lastKey=$pdfKey+$pdfCell['cs']-1;
				$width=$col[$lastKey]['pdfX']-$col[$pdfKey]['pdfX']+$col[$lastKey]['width'];
				$color=$pdfCell['color'];
				if(strlen($color)==3){
					$pdf->SetTextColor(hexdec(substr($color,0,1)),hexdec(substr($color,1,1)),hexdec(substr($color,2)));
				}elseif(strlen($color)==6){
					$pdf->SetTextColor(hexdec(substr($color,0,2)),hexdec(substr($color,2,2)),hexdec(substr($color,4)));
				}else{
					$pdf->SetTextColor(0,0,0);
				}
				$color=$pdfCell['bcolor'];
				if(strlen($color)==3){
					$pdf->SetFillColor(hexdec(substr($color,0,1)),hexdec(substr($color,1,1)),hexdec(substr($color,2)));
					$fill=1;
				}elseif(strlen($color)==6){
					$pdf->SetFillColor(hexdec(substr($color,0,2)),hexdec(substr($color,2,2)),hexdec(substr($color,4)));
					$fill=1;
				}else{
					$fill=0;
				}
				$pdf->SetY($posY);
				$pdf->SetX($col[$pdfKey]['pdfX']);
				$align=substr(strtoupper($pdfCell['align']).'C',0,1);
				if($i>=count($pdfCell['lines'])){
					$txt='';
				}else{
					$txt=$pdfCell['lines'][$i];
				}
				if($i==0){
					if(isset($pdfCell['border'])){
						$border=$pdfCell['border'];
					}else{
						$border='LTR';
					}
				}else{
					$border='LR';
				}
				if(($i==($lines-1))and($pdfCell['rs']==1)){
					$border.='B';
				}
				$pdf->Cell($width,5,$txt,$border,0,$align,$fill);
			}
			$posY+=4.9;
			if($posY>$col[0]['page']){
				if($col[0]['page']<220){
					$pdf->AddPage('L');
				}else{
					$pdf->AddPage('P');
				}
				$posY=12;
			}
		}
		$newPdfRow=array();
		foreach($pdfRow as $pdfKey=>$pdfCell){
			if($pdfCell['rs']>1){
				foreach($pdfCell as $key=>$val){
					if($key=='lines'){
						$newPdfRow[$pdfKey][$key][0]='';
						for($i=$lines;$i<count($val);$i++){
							$newPdfRow[$pdfKey][$key][$i-$lines]=$val[$i];
						}
					}elseif($key=='rs'){
						$newPdfRow[$pdfKey]['rs']=$val-1;
					}else{
						$newPdfRow[$pdfKey][$key]=$val;
					}
					$newPdfRow[$pdfKey]['border']="LR";
				}
			}
		}
	}
	return array($posY,$newPdfRow);
}
function pdfStyle($o1,$o2,$o3,$sName){
	if(isset($o3[$sName])){
		return $o3[$sName];
	}elseif(isset($o2[$sName])){
		return $o2[$sName];
	}elseif(isset($o1[$sName])){
		return $o1[$sName];
	}else{
		return '';
	}
}
function pdfText2Lines($pdf,$txt,$width){
	$s = str_replace("\r",'',$txt);
	$nb=mb_strlen($s, 'utf-8');
	while($nb>0 && mb_substr($s,$nb-1,1,'utf-8')=="\n")	$nb--;
	$sep = -1;
	$i = 0;
	$j = 0;
	$l = 0;
	$ns = 0;
	$nl = 1;
	$lines=array();
	$row=0;
	while($i<$nb){
		// Get next character
		$c = mb_substr($s,$i,1,'UTF-8');
		if($c=="\n"){
			// Explicit line break
			$lines[$row]=mb_substr($s,$j,$i-$j,'UTF-8');
			$row++;
			$i++;
			$sep = -1;
			$j = $i;
			$l = 0;
			$ns = 0;
			$nl++;
			continue;
		}
		if($c==' '){
			$sep = $i;
			$ls = $l;
			$ns++;
		}
		$l += $pdf->GetStringWidth($c);
		if($l>$width){
			// Automatic line break
			if($sep==-1){
				if($i==$j)
					$i++;
				$lines[$row]=mb_substr($s,$j,$i-$j,'UTF-8');
			}else{
				$lines[$row]=mb_substr($s,$j,$sep-$j,'UTF-8');
				$i = $sep+1;
			}
			$row++;
			$sep = -1;
			$j = $i;
			$l = 0;
			$ns = 0;
			$nl++;
		}else
			$i++;
	}
	$lines[$row]=mb_substr($s,$j,$i-$j,'UTF-8');
	return $lines;
}
