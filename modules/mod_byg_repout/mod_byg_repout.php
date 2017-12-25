<?
$doc = JFactory::getDocument();
//   CSS-  
$doc->addStylesheet(JURI::root(true)."/modules/mod_byg_repout/css/jquery-ui.css");
$doc->addStylesheet(JURI::root(true)."/modules/mod_byg_repout/css/report_form.css");
$doc->addStylesheet(JURI::root(true)."/modules/mod_byg_repout/css/font-awesome.min.css");
require __DIR__ . '/report/config.inc';
?>
<!--script type="text/javascript" src="/modules/mod_byg_repout/js/jquery.js"></script>
<script type="text/javascript" src="/modules/mod_byg_repout/js/jquery-3.1.1.min.js"></script-->
<script type="text/javascript" src="/modules/mod_byg_repout/js/jquery-ui.js"></script>
<script type="text/javascript" src="/modules/mod_byg_repout/js/jquery-ui-timepicker-addon.js"></script>
<script type="text/javascript" src="/modules/mod_byg_repout/js/jquery.ui.autocomplete.html.js"></script>
<script type="text/javascript" src="/modules/mod_byg_repout/js/autocomplete.js"></script>
<script type="text/javascript" src="/modules/mod_byg_repout/js/dateformat.js"></script>
<script type="text/javascript" src='/modules/mod_byg_repout/js/report_out.js' ></script>
<script>
var reportURL='<?php echo $mainURL; ?>'
</script>
<input id="fullscreen" type="checkbox" name="fullscreen">
<label for="fullscreen" title="Полный экран"><i class='fa btn'></i></label>
<div class='reportpositioner'>
	<div class='reportselect'>
<?php
	if(isset($_GET['report'])){
		echo "<a href='#' class='reportout firstReportOut' data-rep='{$_GET['report']}' style='display:none;'>{$_GET['report']}</a>";
		echo "<a href='#' class='reportout' data-rep='\\'>Главная</a>";
	}else{
		echo "<a href='#' class='reportout firstReportOut' data-rep='\\'>Главная</a>";
	}
?>
	</div>
	<div id='conteiner' class='fullreport size100'>
		<center>Ничего нет</center>
	</div>
</div>
