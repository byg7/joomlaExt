<?php
require __DIR__ . '/config.inc';
header('Content-type: text/html; charset=utf-8');
if($type=='html'){
	echo $f;
}
$notable=1;
$nobody=1;
$col=array();
$colno=0;
$row=array();
$buttonDiv=0;
$level=0;
$levelClass="";
$levelNow=0;
$tempcellno=0;
$formClass='';
$showPage=1;
$showCell=1;
echo "<div class='paramsDiv'>";
foreach($document as $rowkey=>$cell){
	$tempcellno++;
	$spans='';
	$style='';
	$format='';
	if (isset($cell['col'])) {
		$col[$cell['col']]=$cell;
	}elseif(isset($cell['page'])){ // У col тоже есть page для pdf
		if($showPage==$cell['page']){
			$showCell=1;
		}else{
			$showCell=0;
		}
	}
	if(isset($cell['rs'])){
		$spans=" rowspan='{$cell['rs']}'";
	}
	if(isset($cell['cs'])){
		$spans.=" colspan='{$cell['cs']}'";
	}
	if(isset($cell['hr'])){
		echo "<hr/>";
	}
	if(isset($cell['br'])){
		echo "<br/>";
	}
	if(isset($cell['title'])){
		echo '<h3>'.$cell['title'].'</h3>';
	}
	if(isset($cell['text'])){
		echo $cell['text'];
	}
	if(isset($cell['msg'])){
		echo "<div class='msg'>";
		if(isset($cell['title'])){
			echo "<div class='title'>{$cell['title']}</div>";
		}
		echo "<div class='main'>{$cell['msg']}</div>";
		if(isset($cell['body'])){
			echo "<div class='body'>{$cell['body']}</div>";
		}
		echo '</div>';
	}
	if(isset($cell['repout'])){
		$tmpClass="field";
		if(isset($cell['name'])){
			$tmp=$cell['name'];
		}else{
			$tmp=explode('\\',$cell['repout']);
//var_dump($tmp);
			if (strlen($tmp[count($tmp)-1])==0) {
				$tmp=$tmp[count($tmp)-2];
				$tmpClass="folder";
			}else{
				$tmp=$tmp[count($tmp)-1];
			}
		}
		if(isset($cell['class'])){
			$tmpClass=$cell['class'];
		}
		echo "<div class='{$tmpClass}'><a href='{$rootURL}?report={$cell['repout']}' onclick='return showreport(this);' class='reportout' data-rep='{$cell['repout']}'>{$tmp}</a></div>";
	}
	if(isset($cell['pages'])){
		$p=substr($type,4)-1;
		if($p<0)$p=0;
		echo "Страница: ";
		if($p>0){
			echo " <a href='#' onclick='return waitreport(\"{$_REQUEST['state']}\", \"html{$p}\", 0);'>Предыдущая </a>";
		}else{
			echo "Предыдущая ";
		}
		$p++;
		$showPage=$p;
		echo "<input class='param page' type='text' name='page' value='{$p}' onblur='return waitreport(\"{$_REQUEST['state']}\", \"html\"+this.value, 0);' />";
		if($p<$cell['pages']){
			$p++;
			echo " <a href='#' onclick='return waitreport(\"{$_REQUEST['state']}\", \"html{$p}\", 0);'> Следующая</a>";
		}else{
			echo " Следующая";
		}
		echo " всего ",$cell['pages'];
	}
	if(isset($cell['button'])){
		if($buttonDiv==0){
			$buttonDiv=1;
			echo "<div class='buttonsDiv'>";
		}
		if($cell['button']=='start'){
			if(isset($cell['promt'])){
				echo " <button class='startbtn' data-rep='{$report}' onclick='showfromreport(this)'>{$cell['promt']}</button>";
			}else{
				echo " <button class='startbtn' data-rep='{$report}' onclick='showfromreport(this)'>Сформировать</button>";
			}
		}
		if($cell['button']=='cancel'){
			if(isset($cell['promt'])){
				echo " <button class='startbtn' onclick='doUndo();'>{$cell['promt']}</button>";
			}else{
				echo " <button class='startbtn' onclick='doUndo();'>Отменить</button>";
			}
		}
		if(($cell['button']=='importout')or($cell['button']=='xls')){ ?>
<form action='<?php echo $mainURL; ?>' method='post' target='_blank'>
	<input type='hidden' name='type' value='xls2003' />
	<input type='hidden' name='state' value='<?php echo $_REQUEST['state']; ?>' />
	<input type='submit' class="xls" value='XLS' title='Excel (2003 XML)'/>
</form>
<?php	}
		if(($cell['button']=='importout')or($cell['button']=='xlsx')){ ?>
<form action='<?php echo $mainURL; ?>' method='post' target='_blank'>
	<input type='hidden' name='type' value='xlsx' />
	<input type='hidden' name='state' value='<?php echo $_REQUEST['state']; ?>' />
	<input type='submit' class="xlsx" value='XLSX' title='Excel (2010)'/>
</form>
<?php	}
		if(($cell['button']=='importout')or($cell['button']=='xml')){ ?>
<form action='<?php echo $mainURL; ?>' method='post' target='_blank'>
	<input type='hidden' name='type' value='xml' />
	<input type='hidden' name='state' value='<?php echo $_REQUEST['state']; ?>' />
	<input type='submit' class="xml" value='XML' title='Source (xml)'/>
</form>
<?php	}
		if(($cell['button']=='importout')or($cell['button']=='pdf')){ ?>
<form action='<?php echo $mainURL; ?>' method='post' target='_blank'>
	<input type='hidden' name='type' value='fpdf' />
	<input type='hidden' name='state' value='<?php echo $_REQUEST['state']; ?>' />
	<input type='submit' class="pdf" value='PDF' title='pdf'/>
</form>
<?php	}
	}
	if(isset($cell['param'])){
		if($buttonDiv==1){
			$buttonDiv=0;
			echo "</div>";
		}
		if(isset($cell['class'])){
			if(strlen($cell['class'])>1){
				$formClass=" class='{$cell['class']}'";
			}else{
				$formClass=" class='form'";
			}
		}
//		if(isset($cell['value'])){
//			$params[$cell['param']]=$cell['value'];
//		}
		if($cell['type']=='select'){
			echo " <label><div{$formClass}>{$cell['promt']}</div> ";
			echo "<select class='param' name='{$cell['param']}'>";
			foreach($cell['options'] as $key=>$val){
				if((isset($cell['value']))and($cell['value']==$key)){
					echo "<option selected value='{$key}'>{$val}</option>";
				}else{
					echo "<option value='{$key}'>{$val}</option>";
				}
			}
			echo "</select>";
			echo "</label>";
		}elseif($cell['type']=='text'){
			echo " <label><div{$formClass}>{$cell['promt']}</div> ";
			if(isset($cell['value'])){
				echo "<input class='param' type='text' name='{$cell['param']}' value='{$cell['value']}' />";
			}else{
				echo "<input class='param' type='text' name='{$cell['param']}' />";
			}
			echo "</label>";
		}elseif($cell['type']=='textarea'){
			echo " <label><div{$formClass}>{$cell['promt']}</div> ";
			if(isset($cell['value'])){
				echo "<textarea class='param' type='text' name='{$cell['param']}' >{$cell['value']}</textarea>";
			}else{
				echo "<textarea class='param' type='text' name='{$cell['param']}'></textarea>";
			}
			echo "</label>";
		}
		if(($cell['type']=='date')or
			($cell['type']=='datetime')or
			($cell['type']=='time')){
			echo " <label><div{$formClass}>{$cell['promt']}</div> ";
			if(isset($cell['value'])){
				if($cell['type']=='date'){
					$p=date('d.m.Y',strtotime($cell['value']));
				}elseif($cell['type']=='datetime'){
					$p=date('d.m.Y H:i',strtotime($cell['value']));
				}else{
					$p=date('H:i',strtotime($cell['value']));
				}
				echo "<input class='param {$cell['type']}' type='text' name='{$cell['param']}' value='{$p}' />";
			}else{
				echo "<input class='param {$cell['type']}' type='text' name='{$cell['param']}' />";
			}
			echo "</label>";
		}
		if($cell['type']=='multy'){
			echo " <label onmouseover='selectorPos(this);'><div{$formClass}>{$cell['promt']}</div>";
			if(isset($cell['value'])){
				echo "<input class='param multy' type='hidden' name='{$cell['param']}' value='{$cell['value']}' />";
			}else{
				echo "<input class='param multy' type='hidden' name='{$cell['param']}' />";
			}
			echo "<input class='show multy' type='text' />";
			echo "<div class='selector multy'>";
			echo "<label><input type='checkbox' class='checkbox' data-all='all'><i>Выбрать все</i></label>";
			foreach($cell['options'] as $key=>$val){
				echo "<label><input type='checkbox' class='checkbox' value='{$key}'><i>{$val}</i></label>";
			}
			echo "</div>";
			echo "</label>";
		}
		if($cell['type']=='hidden'){
			if(isset($cell['value'])){
				$pValue=$cell['value'];
			}else{
				$pValue='';
			}
			$html="<input class='param' type='hidden' name='{$cell['param']}' value='{$pValue}' />";
			if(isset($cell['promt'])){
				echo " <label><div{$formClass}>{$cell['promt']}</div> {$html} <strong>{$pValue}</strong></label>";
			}else{
				echo " <label>{$html}</label>";
			}
		}elseif($cell['type']=='label'){
			if(isset($cell['value'])){
				$pValue=$cell['value'];
			}else{
				$pValue='';
			}
			if(isset($cell['promt'])){
				echo " <label><div{$formClass}>{$cell['promt']}</div><strong>{$pValue}</strong></label>";
			}else{
				echo " <label><strong>{$pValue}</strong></label>";
			}
		}
	}
	if(isset($cell['tabname'])){
		if($buttonDiv==1){
			$buttonDiv=0;
			echo "</div>";
		}
		if ($notable==0) {
			echo '</tr></table>';
		}
		echo "</div>";
		if (!is_numeric($cell['tabname'])) {
			echo '<h1>'.$cell['tabname'].'</h1>';
		}
		if(is_numeric($f)) return;
		echo '<div class="thead"><table id="'.$rowkey.'"><thead><tr>';
		$notable=0;
		$col=array();
		$colno=0;
	}
	if((isset($cell['tr']))and($showCell==1)){
		if(isset($cell['level'])){
			$level=$cell['level'];
			$a=explode(' ',$levelClass);
			$levelClass='';
			for($i=0;$i<$level-1;$i++){
				$levelClass.=$a[$i].' ';
			}
			$levelNow=1;
			if(isset($cell['td'])and($nobody==1)){
				echo '</tr></thead><tbody><tr>';
				$nobody=0;
			}elseif(strlen($levelClass)>0){
				echo '</tr></tbody><tbody class="'.$levelClass.'"><tr>';
			}else{
				echo '</tr></tbody><tbody><tr>';
			}
			$levelClass.='plus'.$tempcellno.' ';
		}elseif(isset($cell['td'])and($nobody==1)){
			echo '</tr></thead><tbody><tr>';
			$nobody=0;
//		}elseif(strlen($levelClass)>0){
		}elseif($levelNow>0){
			$levelNow=0;
			echo '</tr></tbody><tbody class="'.$levelClass.'"><tr>';
		}else{
			echo '</tr><tr>';
		}
		if(is_array($cell['tr'])){
			$row=$cell['tr'];
		}else{
			$row=array();
		}
		$colno=0;
	}
	if(isset($col[$colno])){
		$style.=addstyle($col[$colno]);
	} 
	if(isset($cell['format'])){
		$format=$cell['format'];
	}elseif(isset($row['format'])){
		$format=$row['format'];
	}elseif(isset($col[$colno]['format'])){
		$format=$col[$colno]['format'];
	}
	$style.=addstyle($row);
	$style.=addstyle($cell);
	if(((isset($cell['th']))or
		(isset($cell['td']))or
		(isset($cell['p']))or
		(isset($cell['h1']))or
		(isset($cell['h2'])))and($showCell==1)) {
		viewformat($cell,$format,$style,$spans);
	}
	$colno++;
}
if($notable==0){
	echo '</tr></tbody></table>';
	echo '</div>';
}
function addstyle($cell) {
	$astyle='';
	if (isset($cell['align'])) {
		$astyle.="text-align:{$cell['align']};";
	}
	if (isset($cell['color'])) {
		$astyle.="color: #{$cell['color']};";
	}
	if (isset($cell['bcolor'])) {
		$astyle.="background-color: #{$cell['bcolor']};";
	}
	if (isset($cell['width'])) {
		if (!isset($cell['cs']))
			$astyle.="min-width: {$cell['width']};max-width: {$cell['width']};";
	}
	return $astyle;
}
function viewformat($cell,$format,$style,$spans){
	$v=0;
	if(isset($cell['td'])){
		$v=$cell['td'];
		$t='td';
	}elseif(isset($cell['th'])){
		$v=$cell['th'];
		$t='th';
	}elseif(isset($cell['p'])){
		$v=$cell['p'];
		$t='p';
	}elseif(isset($cell['h1'])){
		$v=$cell['h1'];
		$t='h1';
	}elseif(isset($cell['h2'])){
		$v=$cell['h2'];
		$t='h2';
	}
	$a='';
	if(is_array($v)){
		foreach($v as $key=>$val){
			$a.=viewformatvalue($key,$val,$format,$style).' ';
		}
	}else{
		$a=viewformatvalue($v,$cell,$format,'');
	}
	if(isset($cell['level'])){
		global $tempcellno;
		$a='<div class="level plus" id="plus'.$tempcellno.'" onclick="plusminus(this);" />'.$a;
	}
	if($style!==''){
		echo "<{$t}{$spans} style='{$style}'>".$a."</{$t}>";
	}else{
		echo "<{$t}{$spans}>".$a."</{$t}>";
	}
}
function viewformatvalue($v,$cell,$format,$style){
	if((strlen($format)>0)and(strlen($v)>0)){
		if(substr($format,0,1)=='%'){
			if(is_numeric($v))
				$v=number_format($v*100,substr($format,1,1),substr($format,2,1),substr($format,3,1)).'%';
		}elseif(substr($format,0,1)=='n'){
			if(is_numeric($v))
				$v=number_format($v,substr($format,1,1),substr($format,2,1),substr($format,3,1));
		}elseif($format=='dg'){
			$v=date('d.m.y',strtotime($v));
		}elseif($format=='dG'){
			$v=date('d.m.Y',strtotime($v));
		}elseif($format=='dgt'){
			$v=date('d.m.y H:i:s',strtotime($v));
		}elseif($format=='dGt'){
			$v=date('d.m.Y H:i:s',strtotime($v));
		}
//		$v=$v . " " . $format;
	}
	if(isset($cell['a'])){
		if($style!==''){
			$a="<a href='#' style='{$style}' onclick='return showreport(this);' data-rep='{$cell['a']['rep']}' data-params='".json_encode($cell['a']['params'])."'>".str_replace(array('|',"\n"),array('<br/>','<br/>'),$v).'</a>';
		}else {
			$a="<a href='#' onclick='return showreport(this);' data-rep='{$cell['a']['rep']}' data-params='".json_encode($cell['a']['params'])."'>".str_replace(array('|',"\n"),array('<br/>','<br/>'),$v).'</a>';
		}
	}else{
		$a=str_replace(array('|',"\n"),array('<br/>','<br/>'),$v);
	}
	return $a;
}
