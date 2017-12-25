<?php
if (isset($_POST['hr_ajax'])) {
  $hr_ajax=$_POST["hr_ajax"];
} else {
  $hr_ajax=False;
}
if (!$hr_ajax) {
//      
	$doc = JFactory::getDocument();
//   CSS-  
	$doc->addStylesheet( JURI::root(true)."/modules/mod_hiderows/mod_hiderows.css" );
// Ajax не используется или не поддерживается, вставляем код
?>
<script src="/media/editors/tinymce/tinymce.min.js"></script>
<script Language="JavaScript">
function XmlHttp() {
 var xmlhttp;
 try{xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");}
 catch(e) {
  try {xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");} 
  catch (E) {xmlhttp = false;}
 }
 if (!xmlhttp && typeof XMLHttpRequest!='undefined') {
  xmlhttp = new XMLHttpRequest();
 }
 return xmlhttp;
}
function hr_ajax(param) {
	if (window.XMLHttpRequest) req = new XmlHttp();
	method=(!param.method ? "POST" : param.method.toUpperCase());
        var divid=param.divid;
	var div=document.getElementById(divid);
	var elems = div.getElementsByTagName('*');
	if(method=="GET") {
		send=null;
		param.url=param.url+"&hr_ajax=true";
	} else {
		send="";
		for(var i=0; i<elems.length; i++) {
			if (elems[i].id.length>0)
				send+=elems[i].id+"="+elems[i].value+"&";
		}
		send=send+"hr_ajax=true";
	}
	req.open(method, param.url, true);
	if (param.divid)
		document.getElementById(param.divid).innerHTML = '<p><b>Please wait!!!</b></p>'+param.url;
	req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	req.send(send);
	req.onreadystatechange = function() {
		if (req.readyState == 4 && req.status == 200) {
			if (param.divid)
				document.getElementById(param.divid).innerHTML =req.responseText;
		}
	}
}
function hr_log(text) {
	document.getElementById("hr_form_log").innerHTML += text+"<br/>";
}
function hr_mix(text, pkey) {
	if (text.length=0) return text;
	var bkey=[]; // Преобразуем ключ
	for (var i=1;i<pkey.length;i+=2) {
		bkey.push(pkey.charCodeAt(i-1)*10+pkey.charCodeAt(i));
	} 
	var r=[]; // Преобразуем текст
	for (var i=0;i<text.length;i++) {
		r.push(text.charAt(i));
	} 
	for (var i=0; i<r.length; i++) {
		j=(bkey[i%bkey.length]+i)%r.length;
		var c=r[i];
		r[i]=r[j];
		r[j]=c;
	}
	var s="";
	for (var i=0; i<r.length; i++) {
		s=s+r[i];
	}
	return s;
}
function hr_code(text, pkey) {
	if (text.length=0) return text;
	var bkey=[]; // Преобразуем ключ
	for (var i=1;i<pkey.length;i+=2) {
		bkey.push(pkey.charCodeAt(i-1)*10+pkey.charCodeAt(i));
	} 
	var btext=[]; // Преобразуем текст
	for (var i=0;i<text.length;i++) {
		btext.push(text.charAt(i));
	} 
//hr_log(String.concat("hr_code ", btext));
	var bcharcnt=[]; // Количество символов
	var bcharnum=[]; // Номер символа
	var bcharval=[]; // символ
	for (var i=0; i<btext.length; i++) {
		var b=0;
		for (var j=0; j<bcharcnt.length; j++) {
			if (bcharval[j]==btext[i]) {
				bcharcnt[j]++;
				b=1;
				break;
			}
		}
		if (b==0) {
			bcharcnt.push(1);
			bcharnum.push(-1);
			bcharval.push(btext[i]);
		}
	}
	for (var i=0; i<bcharval.length; i++) { 
		var maxc=0;
		var numc=0;
		for (var j=0; j<bcharval.length; j++) {
			if (bcharnum[j]==-1) {
				if (maxc<bcharcnt[j]) {
					maxc=bcharcnt[j];
					numc=j;
				}
			}
		}
		bcharnum[numc]=i;
		bcharcnt[numc]=-1;
	}
	var r=[];
	for (var i=0; i<btext.length; i++) {
		for (var j=0; j<bcharval.length; j++) {
			if (bcharval[j]==btext[i]) {
				var n=bcharnum[j];
				while (n>24) {
					var m=n%25;
					r.push(String.fromCharCode(m+97));
					n=(n-m)/25;
				}
				m=n%25;
				r.push(String.fromCharCode(m+65));
				if (bcharcnt[j]==-1) {
					var c=""+bcharval[j].charCodeAt(0);
					for (var z=0;z<c.length;z++) {
						r.push(c.charAt(z));
					}
					bcharcnt[j]=0;
				}
			}
		}
	}
//hr_log(String.concat("hr_code ", r));
	for (var i=0; i<r.length; i++) {
		j=(bkey[i%bkey.length]+i)%r.length;
		var c=r[i];
		r[i]=r[j];
		r[j]=c;
	}
//hr_log(String.concat("hr_code ", r));
	var s="";
	for (var i=0; i<r.length; i++) {
		s=s+r[i];
	}
	return s;
}
function hr_decode(text, pkey) {
	if (text.length=0) return text;
	var bkey=[]; // Преобразуем ключ
	for (var i=1;i<pkey.length;i+=2) {
		bkey.push(pkey.charCodeAt(i-1)*10+pkey.charCodeAt(i));
	} 
	var btext=[]; // Преобразуем текст
	for (var i=0;i<text.length;i++) {
		btext.push(text.charAt(i));
	} 
//hr_log(String.concat("hr_decode ", btext));
	for (var l=1; l<=btext.length; l++) {
		var i=btext.length-l;
		j=(bkey[i%bkey.length]+i)%btext.length;
		var c=btext[i];
		btext[i]=btext[j];
		btext[j]=c;
	}
//hr_log(String.concat("hr_decode ", btext));
	var bcharnum=[]; // Номер символа
	var bcharval=[]; // символ
	var r=[];
	for (var i=0; i<btext.length; i++) {
		var n=btext[i];
		while (n.charCodeAt(0)>96) {
			i++;
			n=btext[i]+n;
		}
		var m=-1;
		for (var j=0; j<bcharval.length; j++) {
			if (bcharnum[j]==n) {
				r.push(bcharval[j]);
				m=j;
			}
		}
		if (m==-1) {
			bcharnum.push(n);
			var code="0";
			while (btext[i+1].charCodeAt(0)<60) {
				code=code+btext[i+1];
				i++;
				if (i==btext.length-1) break;
			}
//hr_log(String.concat("hr_decode ", code, " ", String.fromCharCode(code)));
			bcharval.push(String.fromCharCode(code));
			r.push(String.fromCharCode(code));
		}
	}
//hr_log(String.concat("hr_decode ", r));
	var s="";
	for (var i=0; i<r.length; i++) {
		s=s+r[i];
	}
	return s;
}
function hr_pkey() {
	var p=document.getElementById('hr_passwd').value;
	return p;
}
function hr_pkey2() {
	var p=document.getElementById('hr_passwd').value;
/*	if (document.getElementById('hr_pas2check').checked) {
		var p2=document.getElementById('hr_pas2').value;
		var p3=document.getElementById('hr_pas3').value;
		if (p2.length>0) {
			if (p2==p3) {
				p=p2;
			}
		}
		if (p!=p2) {
			alert('Used old passwd');
		}
	}
*/	return p;
}
function hr_cell(text, w) {
	var atext=text.split("/");
	if (atext.length>2) {
		var ptext=atext[2];
	} else {
		var ptext=text;
	}
	var r="<td";
	if (w!='-1')
		r=r+" style='max-width:"+w+"px;width:"+w+"px'";
	if (text.substr(0,5)=='http:') {
		return r+"><a href='"+text+"' target='_blank'>"+ptext+"</a></td>";
	} else if (text.substr(0,6)=='https:') {
		return r+"><a href='"+text+"' target='_blank'>"+ptext+"</a></td>";
	} else if (text.substr(0,6)=='ftp:') {
		return r+"><a href='"+text+"' target='_blank'>"+ptext+"</a></td>";
	} else {
		return r+">"+text+"</td>";
	}
}
function hr_parser(text, flag) {
	if (text.length>0) {
		var hrpkey2=hr_pkey2()
		var decoded=hr_decode(text, hrpkey2);
		var key=decoded.charAt(0);
		var keylog=key+"->";
		while (key.charCodeAt(0)<60) {
			decoded=hr_decode(decoded.substr(1, decoded.length), hr_mix(document.getElementById('hr_key_'+key).value, hrpkey2));
			key=decoded.charAt(0);
			keylog+=key+"->";
		}
hr_log("parser keylog: "+keylog);
		var rtext=decoded.substr(1, decoded.length).split("\n");
		var h=0;
		if (flag=='*') {
			return rtext.join('<br />');
		} else {
			var r="<div class='hrtable'><table class='hrtable'>";
			var wtext=[];
			if (rtext.length>0) {
				var ctext=rtext[h].split("|");
				if (ctext[h]=="#") {
					wtext=ctext;
					h=1;
				}
			}
			for (var i=h; i<rtext.length; i++) {
				var ctext=rtext[i].split("|");
				if (i==h) {
					r=r+"<thead>";
				}
				r=r+"<tr>";
				for (var c=0; c<ctext.length; c++) {
					if (i==h) {
						if (wtext.length>(c+1))
							r=r+"<th style='max-width:"+wtext[c+1]+"px;width:"+wtext[c+1]+"px'>"+ctext[c]+"</th>"
						else
							r=r+"<th>"+ctext[c]+"</th>"
					} else {
						if (wtext.length>(c+1))
							r=r+hr_cell(ctext[c],wtext[c+1])
						else
							r=r+hr_cell(ctext[c],'-1');
					}
				}
				r=r+"</tr>";
				if (i==h) {
					r=r+"</thead><tbody>";
				}
			}
			r=r+"</tbody></table></div>";
			return r;
		}
	} else return "";
}
function hr_select(text, id) {
	var r="<select id='hr_select' name='select'>";
	var r2='';
	if (text.length>0) {
		var decoded=hr_decode(text, hr_pkey());
		var key=decoded.charAt(0);
		var keylog=key+"->";
		while (key.charCodeAt(0)<60) {
			decoded=hr_decode(decoded.substr(1, decoded.length), hr_mix(document.getElementById('hr_key_'+key).value, hr_pkey()));
			key=decoded.charAt(0);
			keylog+=key+"->";
		}
hr_log("select keylog: "+keylog);
//		в stext теперь будет массив полей попробуем их отсортировать
		var stext=decoded.substr(1, decoded.length).split("|");
		r=r+"<option value='"+(stext.length+2)+"'>New category</option>\n";
		var rstext=[];
		for (var i=0; i<stext.length; i++) {
			rstext.push("no");
		}
		var j=0; // Сортируем по имени
		while (j>-1) {
			j=-1;
			for (var i=0; i<stext.length; i++) {
				if (rstext[i]=="no") {
					if (j==-1) {
						j=i;
						if (stext[j].substr(0,1)=='*') {
							stextj=stext[j].substr(1,stext[j].length);
						} else {
							stextj=stext[j];
						}
					} else {
						if (stext[i].substr(0,1)=='*') {
							stexti=stext[i].substr(1,stext[i].length);
						} else {
							stexti=stext[i];
						}
						if (stexti<stextj) {
							j=i;
							stextj=stexti;
						}
					}
				}
			}
			if (j>-1) {
				rstext[j]="yes";
				if (id==j+2) {
					r2=stext[j].substr(0,1);
					r=r+"<option value='"+(j+2)+"' selected='true'>"+stextj+"</option>\n";
				} else {
					r=r+"<option value='"+(j+2)+"'>"+stextj+"</option>\n";
				}
			}
		}
	} else {
		r=r+"<option value='2'>New category</option>\n";
	}	
	r=r+"</select>";
	return [r, r2];
}
function hr_edit() {
	var p=document.getElementById('hr_passwd').value;
	if (p.length==0) {
		alert("Введите пароль");
		return;
	}
	var val_id=document.getElementById('hr_val_id').value;
	var select=document.getElementById('hr_val_1').value;
	if (select.length>0) {
		select=hr_decode(select, hr_pkey());
		var key=select.charAt(0);
		while (key.charCodeAt(0)<60) {
			select=hr_decode(select.substr(1, select.length), hr_mix(document.getElementById('hr_key_'+key).value, hr_pkey()));
			key=select.charAt(0);
		}
	}
	var aselect=select.substr(1, select.length).split("|");
	var edit_cat=document.getElementById('hr_edit_cat');
	var r="";
	if (edit_cat==null) {
		r="Вы редактируете категорию "+val_id+" ";
		if (val_id==1) {
			r="Сначала загрузите";
		} else {
			var val_id_name="";
			if (val_id-2<aselect.length) {
				val_id_name=aselect[val_id-2];
			} else {
				val_id_name="New cftegory";
			}
			var value=document.getElementById('hr_val_'+val_id).value;
			if (value.length>0) {
				value=hr_decode(value, hr_pkey2());
				var key=value.charAt(0);
				while (key.charCodeAt(0)<60) {
					value=hr_decode(value.substr(1, value.length), hr_mix(document.getElementById('hr_key_'+key).value, hr_pkey2()));
					key=value.charAt(0);
				}
				value=value.substr(1, value.length);
			}
			value=value.split('|').join(' | ');
			r=r+val_id_name+" введите новое название <input type='text' id='hr_edit_cat' value='"+
				val_id_name+"'/><a class='btn' href='#' onclick='tinyMCE.execCommand("+
				'"mceToggleEditor",false,"hr_edit_val"'+");return false;' title='Включить редактор'>"+
				"<span class='icon-eye'></span>Включить редактор</a><textarea class='hredit' id='hr_edit_val'>"+value+"</textarea>";
		}
		document.getElementById("hr_form_work").innerHTML=r;
		if (val_id_name.substr(0,1)=='*') {
			tinyMCE.init({
	selector : "textarea",
	plugins: [
		'advlist autolink lists link image charmap print preview anchor textcolor',
		'searchreplace visualblocks code fullscreen',
		'insertdatetime media table contextmenu paste code'
	]});
		}
	} else {
		var send="hr_mail="+document.getElementById("hr_mail").value+"&";
		var hrselect=document.getElementById('hr_select');
		if (hrselect!=null) {
			send+="hr_load="+hrselect.value+"&";
		}
// сначала перекодируем список
		aselect[val_id-2]=document.getElementById('hr_edit_cat').value;
		select="a"+aselect[0];
		for (var i=1; i<aselect.length;i++) {
			select=select+"|"+aselect[i];
		}
		var keys=(Math.random()*1000).toFixed(0)%8+4;
		var keylog=keys+"->";
		var hrpkey2=hr_pkey2()
		for (var i=0; i<keys;i++) {
			var key="_";
			while (document.getElementById('hr_key_'+key)==null) {
				key=(Math.random()*1000).toFixed(0)%10;
			}
			keylog+=key+", ";
			select="".concat(key, hr_code(select, hr_mix(document.getElementById('hr_key_'+key).value, hrpkey2)));
		}
hr_log("select keys="+keylog);
		select=hr_code(select, hrpkey2);
		send+="hr_set_1="+select+"&";
		send+="hr_set="+val_id+"&";
// теперь сами данные
		select="a"+document.getElementById('hr_edit_val').value;
		select=select.split("	").join("|");
		select=select.split("  |").join("|");
		select=select.split("|  ").join("|");
		select=select.split(" |").join("|");
		select=select.split("| ").join("|");
		var keys=(Math.random()*1000).toFixed(0)%8+4;
		var keylog=keys+"->";
		for (var i=0; i<keys;i++) {
			var key="_";
			while (document.getElementById('hr_key_'+key)==null) {
				key=(Math.random()*1000).toFixed(0)%10;
			}
			keylog+=key+", ";
			select="".concat(key, hr_code(select, hr_mix(document.getElementById('hr_key_'+key).value, hrpkey2)));
		}
hr_log("select keys="+ keylog);
		select=hr_code(select, hrpkey2);
		send+="hr_set_val="+select+"&";
		send=send+"hr_ajax=true";//hr_log(String.concat("send ", send));
		if (window.XMLHttpRequest) req = new XmlHttp();
		req.open("POST", "/modules/mod_hiderows/mod_hiderows.php", true);
		if ("hr_form_container")
			document.getElementById("hr_form_container").innerHTML = "<p><b>Please wait!!!</b></p>";
		req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		req.send(send);
		req.onreadystatechange = function() {
			if (req.readyState == 4 && req.status == 200) {
				var rT=req.responseText;
				document.getElementById("hr_form_container").innerHTML =rT;
				document.getElementById("hr_form_work").innerHTML="Edit complited.";
			}
		}
//		tinyMCE.destroy();
	}
}
function hr_load() {
document.getElementById("hr_form_log").innerHTML="";
	var p=document.getElementById('hr_passwd').value;
	if (p.length==0) {
		alert("Введите пароль");
		return;
	}
	var hrselect=document.getElementById('hr_select');
	send="hr_mail="+document.getElementById("hr_mail").value+"&";
	if (hrselect!=null) {
		send+="hr_load="+hrselect.value+"&";
	}
	send=send+"hr_ajax=true";
	if (window.XMLHttpRequest) req = new XmlHttp();
	req.open("POST", "/modules/mod_hiderows/mod_hiderows.php", true);
	if ("hr_form_container")
		document.getElementById("hr_form_container").innerHTML = "<p><b>Please wait!!!</b></p>";
	req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	req.send(send);
	req.onreadystatechange = function() {
		if (req.readyState == 4 && req.status == 200) {
			var rT=req.responseText;
			document.getElementById("hr_form_container").innerHTML =rT;
			var hr_sel=rT.split("hr_val_1' value='")[1].split("'")[0];
			var iH="";
			if (hrselect!=null) {
				iHa=hr_select(hr_sel, hrselect.value);
				iH=iHa[0];
				hr_sel=rT.split("hr_val_"+hrselect.value+"' value='")[1].split("'")[0];
				iH=iH+hr_parser(hr_sel, iHa[1]);
			} else {
				iH=hr_select(hr_sel, 0)[0];
			}
			document.getElementById("hr_form_work").innerHTML =iH;
		}
	}
}
function hr_BackUp() {
	var p=document.getElementById('hr_passwd').value;
	if (p.length==0) {
		alert("Введите пароль");
		return;
	}
	var val_id=document.getElementById('hr_val_id').value;
	var select=document.getElementById('hr_val_1').value;
	if (select.length>0) {
		select=hr_decode(select, hr_pkey());
		var key=select.charAt(0);
		while (key.charCodeAt(0)<60) {
			select=hr_decode(select.substr(1, select.length), hr_mix(document.getElementById('hr_key_'+key).value, hr_pkey()));
			key=select.charAt(0);
		}
	}
	var aselect=select.substr(1, select.length).split("|");
	var backup_text=document.getElementById('hr_backup_text');
	var r="";
	if (backup_text==null) {
		var hrselect=document.getElementById('hr_select');
		if (hrselect==null) {
			hr_load();
			return;
		}
		r="Скопируйте и сохраните этот текст как копию.<br/>А для воостановления наоборот скопируйте текст из копии и нажмите еще раз кнопку BackUp.";
		var bakuptext=hr_code(select,'backup');
		bakuptext=select+"\n"+aselect.length;
		r=r+"<textarea class='hredit' id='hr_backup_text'>"+bakuptext+"</textarea>";
		var send="hr_mail="+document.getElementById("hr_mail").value+"&";
		send+="hr_backup="+aselect.length+"&";
		send=send+"hr_ajax=true";
		if (window.XMLHttpRequest) req = new XmlHttp();
		req.open("POST", "/modules/mod_hiderows/mod_hiderows.php", true);
		if ("hr_form_container")
			document.getElementById("hr_form_container").innerHTML = "<p><b>Please wait!!!</b></p>";
		req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		req.send(send);
		req.onreadystatechange = function() {
			if (req.readyState == 4 && req.status == 200) {
				var rT=req.responseText;
				document.getElementById("hr_form_work").innerHTML=r+rT;
				var b="";
				for (i=1;i<aselect.length+2;i++) {
					var bk=document.getElementById('hr_bk_'+i).value;
					if (bk.length>0) {
						bk=hr_decode(bk, hr_pkey());
						var key=bk.charAt(0);
						while (key.charCodeAt(0)<60) {
							bk=hr_decode(bk.substr(1, bk.length), hr_mix(document.getElementById('hr_key_'+key).value, hr_pkey()));
							key=bk.charAt(0);
						}
					}
					b=b+hr_code(bk,'backup')+"\n";
				}
				document.getElementById("hr_backup_text").value=b;
				if ("hr_form_container")
					document.getElementById("hr_form_container").innerHTML = "<p><b>OK!</b></p>";
			}
		}
	} else { //Restore
		var hr_rst=backup_text.value.split("\n");
		var b="";
		for (var hr_id=1;hr_id<=hr_rst.length;hr_id++) {
			b=b+"Элем. "+hr_id+"\n"+hr_decode(hr_rst[hr_id-1],'backup')+"\n";
		}
		document.getElementById("hr_backup_text").value=b;
	}
}
function hr_SavePasswd() {
	var c=document.getElementById('hr_savepas');
	var p=document.getElementById('hr_passwd');
	alert(c.value);
	if (c.value.charAt(0)!='D') {
		localStorage.setItem('hrpasswd', p.value);
		c.value='Delete password from local storage';
	} else {
		localStorage.removeItem('hrpasswd');
		c.value='Save password in local storage';
	}
}
setTimeout(function() {
	if (localStorage.hrpasswd) {
		var c=document.getElementById('hr_savepas');
		var p=document.getElementById('hr_passwd');
		p.value=localStorage.hrpasswd;
		c.value='Delete password from local storage';
		hr_load();
	}
}, 100);
</script>
<form>
	<label class='hrlabel' for='hr_passwd'>Password:</label><input class='hrtext' type='password' id='hr_passwd'/>
	<input type='button' id='hr_btn' onclick='hr_load()' value='Load'/>
	<input type='button' id='hr_btn' onclick='hr_edit()' value='Edit'/>
	<input type='button' id='hr_backup' onclick='hr_BackUp()' value='BackUp'/>
	<input type='button' id='hr_savepas' onclick='hr_SavePasswd()' value='Save password in local storage'/>
	<div id='hr_form_work'>
	</div>
	<div id='hr_form_container'>
<?php
	$user = JFactory::getUser();
	$usermail=$user->email;
} else {
	if (isset($_POST['hr_mail'])) {
		$usermail=$_POST['hr_mail'];
	}
// Это запрос AJAX
	header("Content-type: text/plain; charset=UTF-8");
	header("Cache-Control: no-store, no-cache, must-revalidate");
	header("Cache-Control: post-check=0, pre-check=0", false);
	define('_JEXEC', 1);
	define('DS', DIRECTORY_SEPARATOR);
	if (file_exists(dirname(__FILE__) . '/../../defines.php')) {
 		include_once dirname(__FILE__) . '/../../defines.php';
	}
	if (!defined('_JDEFINES')) {
 		define('JPATH_BASE', dirname(__FILE__) . '/../..');
	 	require_once (JPATH_BASE.'/includes/defines.php');
        	require_once (JPATH_BASE.'/includes/framework.php');
	}
	require_once JPATH_BASE.'/includes/framework.php';
	$app = JFactory::getApplication('site');
	$app->initialise();
}
$db= JFactory::getDBO();
//$db->setQuery("DROP TABLE hr_keys");
//$d = $db->loadObjectList();
//$db->setQuery("DROP TABLE hr_vals");
//$d = $db->loadObjectList();
//      
$db->setQuery("CREATE TABLE IF NOT EXISTS hr_keys (hrid INT AUTO_INCREMENT PRIMARY KEY, hrkey VARCHAR(128))");
$d = $db->query();
//$d = $db->loadObjectList();
$db->setQuery("CREATE TABLE IF NOT EXISTS hr_vals (hrid INT, hrmail VARCHAR(50), hrval LONGBLOB, PRIMARY KEY (hrid, hrmail))");
$d = $db->query();
//$d = $db->loadObjectList();

$db->setQuery("Select Count(*) From hr_keys Where LENGTH(hrkey)<120");
$d = $db->loadResult();
$newkey=rand(0,59);
if ($newkey<10) {
	$newchar=chr($newkey+48);
} else if ($newkey<35) {
	$newchar=chr($newkey+55);
} else {
	$newchar=chr($newkey+62);
}
if ($d>"0") {
	$db->setQuery("UPDATE hr_keys Set hrkey=CONCAT(hrkey, '".$newchar."') where LENGTH(hrkey)<120");
	$d = $db->query();
//	$d = $db->loadObjectList();
} else {
	$db->setQuery("Select Count(*) From hr_keys Where LENGTH(hrkey)>119");
	$d = $db->loadResult();
	if ($d<="9") {
		$db->setQuery("INSERT INTO hr_keys (hrkey) values ('".$newchar."')");
		$d = $db->query();
		//$d = $db->loadObjectList();
	}
}
while (true){
	$db->setQuery("Select Count(*) From hr_keys Where LENGTH(hrkey)>119");
	$d = $db->loadResult();
	if ($d>"0") break;
	$newkey=rand(0,59);
	if ($newkey<10) {
		$newchar=chr($newkey+48);
	} else if ($newkey<35) {
		$newchar=chr($newkey+55);
	} else {
		$newchar=chr($newkey+62);
	}
	$db->setQuery("UPDATE hr_keys Set hrkey=CONCAT(hrkey, '".$newchar."') where LENGTH(hrkey)<120");
	$d = $db->query();
//	$d = $db->loadObjectList();
} 
if (isset($_POST['hr_set'])) {
	$hr_set=$_POST["hr_set"];
	$hr_set_val=$_POST["hr_set_val"];
	$hr_set_1=$_POST["hr_set_1"];
	$db->setQuery("INSERT INTO hr_vals SET hrval='".$hr_set_val."',hrmail='".$usermail."',hrid=".$hr_set." ON DUPLICATE KEY UPDATE hrval='".$hr_set_val."',hrmail='".$usermail."'");
	$d = $db->query();
//	$d = $db->loadObjectList();
	$db->setQuery("INSERT INTO hr_vals SET hrval='".$hr_set_1."',hrmail='".$usermail."',hrid=1 ON DUPLICATE KEY UPDATE hrval='".$hr_set_1."',hrmail='".$usermail."'");
	$d = $db->query();
//	$d = $db->loadObjectList();
	echo "Saved to Base.";
}
$db->setQuery("Select hrval From hr_vals Where hrid=1 and hrmail='".$usermail."'");
$d = $db->loadResult();
echo "<input type='hidden' id='hr_val_1' value='".$d."'/>\n";
//echo "sel=".$d;
if (isset($_POST['hr_load'])) {
	$hr_load=$_POST["hr_load"];
	$db->setQuery("Select hrval From hr_vals Where hrid=".$hr_load." and hrmail='".$usermail."'");
	$d = $db->loadResult();
	echo "<input type='hidden' id='hr_val_".$hr_load."' value='".$d."'/>\n";
	echo "<input type='hidden' id='hr_val_id' value='".$hr_load."'/>\n";
} else {
	echo "<input type='hidden' id='hr_val_id' value='1'/>\n";
}
if (isset($_POST['hr_backup'])) {
	$hr_backup=$_POST["hr_backup"];
	for ($hr_id=1;$hr_id<$hr_backup+2;$hr_id++) {
		$db->setQuery("Select hrval From hr_vals Where hrid=".$hr_id." and hrmail='".$usermail."'");
		$d = $db->loadResult();
		echo "<input type='hidden' id='hr_bk_".$hr_id."' value='".$d."'/>\n";
	}
}
echo "<input type='hidden' id='hr_mail' value='".$usermail."'/>\n";
$q = 'Select * From hr_keys';
$db->setQuery($q);
$d = $db->loadObjectList();
$s="";
$id="";
foreach ($d as $i => $val) {
	foreach ($val as $j => $sval) {
		if (strlen($sval)>119) {
			$s.="<input type='hidden' id='hr_key_".$id."' value='".$sval."'/>\n";
		}
		$id=$sval;
	}
//	echo $val . '<br/>';
}
//echo "</div>";
echo $s;
if (!$hr_ajax) {
?>
	</div>
	<div id='hr_form_log' class='prokrutka'>
	</div>
</form>
<?php
}
?>
