<?php
// Опрос задания
if(isset($_POST['isreport'])) {
	$dir=__DIR__.'/question/';
	for($i=0;$i<40;$i++) {
		$files=scandir($dir);
		foreach($files as $file){
			if(!is_dir($dir.$file)) {
				if(substr($file,-4)=='.txt'){
					echo "id=".substr($file,0,-4)."
";
					echo file_get_contents($dir.$file);
					unlink($dir.$file);
					return;
				} else {
					unlink($dir.$file);
				}
			}
		}
		sleep(1);
	}
	return;
}
// Загружаем ответ
if(isset($_POST['answer'])) {
	if(!isset($_REQUEST['id'])) {
		echo 'error';
		return;
	}
	$id=$_REQUEST['id'];
	$zipClass = 'ZipArchive';
	$zip = new $zipClass;
	$zip->open($_FILES["filename"]["tmp_name"]);
	if ($_POST['answer']=='wait') {
		$content=$zip->getFromName("{$id}.txt");
		file_put_contents(__DIR__.'/answer/'.$id.'.txt', $content);
	} else {
		$content=$zip->getFromName("{$id}.xml");
		file_put_contents(__DIR__.'/answer/'.$id.'.xml', $content);
	}
	echo "id={$id}=";
    return;
}
// Запрос формирования отчета
if(isset($_POST['report'])) {
	$report=$_REQUEST['report'];
	if (isset($_REQUEST['params'])) {
		$params=json_decode($_REQUEST['params'],1);
	} else {
		$params=array(); //[]
	}
	if (isset($_REQUEST['type'])) {
		$type=$_REQUEST['type'];
	} else {
		$type='html';
	}
//	$id=sha1($report.time().rand(1,1000).$user->email);
	$id=sha1($report.$user->email).rand(1000,9999).substr(time(),-5);
	$content="report={$report}
type={$type}
user={$user->email}";
	foreach($params as $k=>$v){
		$content.="
params_$k=$v";
	}
	file_put_contents(__DIR__.'/question/'.$id.'.txt',$content);
	echo "id={$id}=";
    return;
}
// Запрос результатов
if(isset($_POST['state'])) {
	$dir=__DIR__.'/answer/';
	$files=scandir($dir);
	foreach($files as $file){
		if(!is_dir($dir.$file)) {
			if(filemtime($dir.$file)+3600<time()) {
				unlink($dir.$file);
			}
		}
	}
	$id=$_REQUEST['state'];
	//Ждем появление файла
	for($i=0;$i<10;$i++){
		if(file_exists(__DIR__.'/answer/'.$id.'.txt')) {
			break;
		} elseif(file_exists(__DIR__.'/answer/'.$id.'.xml')) {
			break;
		}
		sleep(1);
	}
	if(file_exists(__DIR__.'/answer/'.$id.'.txt')) {
		$content=file_get_contents(__DIR__.'/answer/'.$id.'.txt');
		unlink(__DIR__.'/answer/'.$id.'.txt');
		$f='0';
	} elseif(file_exists(__DIR__.'/answer/'.$id.'.xml')) {
		$content=file_get_contents(__DIR__.'/answer/'.$id.'.xml');
		$f=' ';
	} else {
		$content="<root><report>wait</report>
<cell><tabname>Кеш не доступен. Сформируйте отчет заново.</tabname><th>1</th></cell>
<cell><tr>1</tr><td>1</td></cell>
</root>";
		$f='1';
	}
	$content=iconv('windows-1251', 'utf-8', $content);
	$document=array(); //[]
	$cellno=0;
	if($_REQUEST['type']=='xml'){
		header('Content-type: text/xml; charset=utf-8');
		echo $content;
		die();
	}
	$xml = simplexml_load_string($content, 'SimpleXMLElement');
	$content='';
	$report = $xml->report;
	include(__DIR__.'/../report/reportajax.inc');
	foreach ($xml->cell as $item) {
		$cell=array(); //[]
		foreach ($item as $key=>$child) {
			if(((string)$key=='json')and(strlen((string)$child)>1)){
				$cell=json_decode((string)$child,1);
//var_dump((string)$child);
			}elseif((((string)$key=='tr')or((string)$key=='options'))and(strlen((string)$child)>1)){
				$cell[(string)$key]=json_decode('{"'.str_replace(array(':',','),array('":"','","'),(string)$child).'"}',1);
			}else{
				$cell[(string)$key]=(string)$child;
			}
		}
		newcell($cell);
	}
	if (count($document)==0){
		$cell=array(); //[]
		$report="Ошибка";
		$cell["title"]="Ошибка";
		$cell["msg"]="Не правильный документ XML";
		$cell["body"]="Сообщите об ошибке разработчику отчета";
		newcell($cell);
	}
	build($f);
	return;
}
echo "Здесь могла бы быть Ваша реклама!";
?>