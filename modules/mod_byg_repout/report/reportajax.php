<?php
	// Построитель отчетов
$document=[];
$cellno=0;
session_write_close();
include('reportajax.inc');
if (isset($_REQUEST['report'])) {
	$report=$_REQUEST['report'];
	if (isset($_REQUEST['params'])) {
		$params=json_decode($_REQUEST['params'],1);
	} else {
		$params=[];
	}
	if (file_exists(__DIR__.'/report/'.$report.'.php')) {
		include_once(__DIR__.'/report/'.$report.'.php');
/*	Вызывается построитьель отчетов который должен вернуть массив $document
Массив состоит из последовательных элементов, то есть отчет строится в режиме "телетайп".
Каждый эемент может содержать следующие свойства (поделементы):
	- 

*/
	} else {
		echo "Отчет не найден";
		return;
	}
	// Собственно строим
	build('');
}
?>