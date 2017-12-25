<?php
//ini_set('display_errors', 1);
//ini_set('display_startup_errors', 1);
//error_reporting(E_ALL);
function build($f) {
	global $document,$cellno,$report,$params,$tempcellno;
	if(isset($_REQUEST['type'])){
		$type=$_REQUEST['type'];
		if((substr($type,0,4)=='html')or(is_numeric($f))){
			require __DIR__ . '/html.inc';
		}elseif ($type=='xlsx') {
			require __DIR__ . '/xlsx.inc';
		}elseif($type=='xls2003'){
			require __DIR__ . '/xls.inc';
		}elseif($type=='fpdf'){
			require __DIR__ . '/pdf.inc';
		}
	}
}
function addstyle2003($cell,$style){
	if(isset($cell['align'])){
		$style['text-align']=$cell['align'];
	}
	if(isset($cell['color'])){
		$style["color"]=$cell['color'];
	}
	if(isset($cell['bcolor'])){
		$style["bcolor"]=$cell['bcolor'];
	}
	return $style;
}
function newcell($a){
	global $document, $cellno;
	if(isset($a)){
		$document[$cellno++]=$a;
	}else{
		$document[$cellno++]=array('td'=>'Null');
	}
}
function phpDateToExcel($date){
	$uDateTime=strtotime($date.' Europe/London'); // Гринвич наше все!!!
	$edate=($uDateTime/86400+25569);
	if($edate<61){ // 1900 год в Excel высокосный из-за Lotus 1-2-3
		$edate--;
	}
	return $edate;
}
function colName($index){
	if($index>25){
		$i=($index % 26);
		return colName((($index-$i)/26)-1).colName($i);
	}else{
		return chr(65+$index);
	}
}
