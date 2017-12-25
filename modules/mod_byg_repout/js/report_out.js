/* В теле html должна быть определена переменная
var reportURL=...
*/
var waitreportID='';
var oldReportHTML='';
var tempHTML='';
var startTime = new Date;
function showUndo(){
	if(oldReportHTML.length>0){
		return '<a class="undo" href="#" onclick="doUndo();">Назад</a>';
	}else{
		return '';
	}
}
function doUndo(){
	waitreportID=0;
	jQuery('#conteiner').html(oldReportHTML);
}
jQuery(document).ready(function() {
	jQuery('.firstReportOut').each(function(){
		showreport(this);
	});
	jQuery('.reportout').bind("click",function(e){
		showreport(this);
		e.preventDefault();
	});
});
/*						REPORT						*/
function showfromreport(e){
	var rep='report='+e.dataset.rep+'&type=html';
	var type='html';
	var prm={};
	jQuery('#conteiner').find('.param').each(function(){
		prm[''+jQuery(this).attr('name')]=''+jQuery(this).val();
	});
	rep+='&params='+JSON.stringify(prm);
	oldReportHTML=jQuery('#conteiner').html();
	startTime = new Date;
	jQuery('#conteiner').html(tempHTML=showUndo()+'<div class="paramsDiv"><div class="msg"><div class="main">Подключаемся к сервису</div></div></div>');
	jQuery.ajax({
		type: "POST",
		url: reportURL,
		data: rep,
		success: function(txt){
			var id=txt.split('=');
			if (id.length==3) {
				waitreportID=id[1];
				waitreport(id[1], type, 0);
			} else {
				jQuery('#conteiner').html(showUndo()+'<div class="paramsDiv"><div class="msg"><div class="main">ошибка:</div><div class="body">'+txt+'</div></div></div>');
			}
		}
	});
	return false;
}
function waitreport(id, type, i){
	var rep='state='+id+"&type="+type;
	if(waitreportID==id){
	setTimeout(function(){
		jQuery.ajax({
			type:"POST",
			url:reportURL,
			data:rep,
			success:function(txt){
				if(txt.charAt(0)=='0'){
					startTime = new Date;
					jQuery('#conteiner').html(tempHTML=showUndo()+txt.substr(1,txt.length));
					waitreport(id, type, 0);
				}else if(txt.charAt(0)=='1'){
					i++;
					if(i>60){
						jQuery('#conteiner').html(showUndo()+'<div class="paramsDiv"><div class="msg"><div class="main">Не дождались ответа</div></div></div>');
					}else{
						var leftTime=Math.round((new Date - startTime)/1000);
						if(leftTime>60){
							jQuery('#conteiner').html(tempHTML+((leftTime-(leftTime%60)))/60+"м "+(leftTime%60)+"с");
						}else{
							jQuery('#conteiner').html(tempHTML+(leftTime%60)+"с");
						}
						waitreport(id, type, i);
					}
				}else{
					jQuery('#conteiner').html(txt);
					reportinit();
				}
			}
		});
	}, 1000);
	}
}
function showreport(e) {
	var cont='#conteiner';
	if(e.dataset.cont){
		cont=e.dataset.cont;
	}
	var rep='report='+e.dataset.rep+'&type=html';
	var type='html';
	if (e.dataset.params) rep+='&params='+e.dataset.params;
	oldReportHTML=jQuery('#conteiner').html();
	startTime=new Date;
	jQuery(cont).html(tempHTML=showUndo()+'<div class="paramsDiv"><div class="msg"><div class="main">Подключаемся к сервису</div></div></div>');
	jQuery.ajax({
		type:"POST",
		url:reportURL,
		data:rep,
		success:function(txt){
			var id=txt.split('=');
			if(id.length==3){
				waitreportID=id[1];
				waitreport(id[1], type, 0);
			}else{
				jQuery('#conteiner').html(showUndo()+"ошибка:" + txt);
			}
		}
	});
	return false;
}
function moveTableHead() {
	var cont=document.getElementById("conteiner");
	var srcDiv=cont.getElementsByClassName("thead");
	for(var i=0;i<srcDiv.length;i++){
		var scTop=srcDiv[i].scrollTop;
		var tbls=srcDiv[i].getElementsByTagName("table");
		for(var t=0;t<tbls.length;t++){
			var id=tbls[t].id;
			if(id.split("_").length==1){ /* нащел исходную таблу */
				var clone=document.getElementById(id+"_clone");
				if(!clone){ /* создаем */
					var colgrp=tbls[t].getElementsByTagName("colgroup");
					if (colgrp.length>0) colgrp='<colgroup>'+colgrp[0].innerHTML+'</colgroup>'
					else colgrp='';
					var src=tbls[t].getElementsByTagName("thead");
					srcDiv[i].innerHTML='<table id="'+id+'_clone">'+colgrp+src[0].outerHTML+'</table>'+srcDiv[i].innerHTML;
					clone=document.getElementById(id+"_clone");
					clone.style["z-index"]="10";
				}
				for(var r=0;r<clone.rows.length;r++){
					for(var c=0;c<clone.rows[r].cells.length;c++){
						if(clone.rows[r].cells[c].offsetWidth!=
						 tbls[t].rows[r].cells[c].offsetWidth){
							var pw=0;
							try{pw=clone.rows[r].cells[c].style['min-width'];
							}catch(e){}
							pw=(pw.replace('px','')*1);
							if(pw==0){
								pw+=tbls[t].rows[r].cells[c].offsetWidth;
							}else{
								pw+=((tbls[t].rows[r].cells[c].offsetWidth-
									clone.rows[r].cells[c].offsetWidth)/1.3);
							}
							clone.rows[r].cells[c].style['min-width']=pw+'px';
							clone.rows[r].cells[c].style['max-width']=pw+'px';
						}
					}
				}
				if(scTop<(clone.offsetHeight/10)){
					clone.style['top']=(-clone.offsetHeight)+'px';
					tbls[t].style['top']=(-clone.offsetHeight)+'px';
				}else{
					clone.style['top']=scTop+'px';
				}
			}
		}
	}
}
var dateTimeDefaults={
	closeText:'Готово',
	currentText:'Сейчас',
	dateFormat:'dd.mm.yy',
	timeFormat:'HH:mm',
	monthNames:["Январь","Февраль","Март","Апрель","Май","Июнь","Июль","Август","Сентябрь","Октябрь","Ноябрь","Декабрь"],
	dayNamesShort:["Вс","Пн","Вт","Ср","Чт","Пт","Сб"],
	dayNamesMin:["Вс","Пн","Вт","Ср","Чт","Пт","Сб"],
	firstDay:1,
	timeOnlyTitle:'Выберите время',
	timeText:'Время',
	hourText:'Часы',
	minuteText:'Минуты',
	secondText:'Секунды',
	autoclose:true,
	showSecond:false,
	showMillisec:false,
	showMicrosec:false,
	showTimezone:false,
	showTime:false,
};
function reportinit(){
	setTimeout(function(){
		jQuery(".param.datetime").datetimepicker(dateTimeDefaults);
		jQuery(".param.date").datepicker(dateTimeDefaults);
		jQuery(".param.time").timepicker(dateTimeDefaults);
	},100);
	setTimeout(function(){
		showplusminus('#conteiner');
	},55);
	setTimeout(function(){
		jQuery("div.selector.calendar td").bind("mouseover", function() {
			var diff=this.dataset.diff;
			jQuery(this).parent().parent().find(".foot").each(function() {
				jQuery(this).html(diff);
			});
		});
	},105);
	setTimeout(function(){
		jQuery("div.selector.calendar td").bind("click", function() {
			var e=jQuery(this);
			while (!e.is('label')) {
				e=e.parent();
			}
			var diff=this.dataset.diff;
			e.find(".param").each(function() {
				jQuery(this).val(diff);
				reportinit();
			});
		});
	},110);
	setTimeout(function(){
		jQuery('.param.calendar').bind("blur", function() {
			var dt=paGetDate(patodt(jQuery(this).val()));
			if (dt!=jQuery(this).val()) {
				jQuery(this).val(dt);
				reportinit();
			}
		})
	},120);
	setTimeout(function(){
		jQuery('.selector.multy .checkbox').bind("click", function() {
			if (this.dataset.all=='all') {
				if (jQuery(this).is(":checked")) {
					jQuery(this).parent().parent().find('.checkbox').attr("checked","checked");
				} else {
					jQuery(this).parent().parent().find('.checkbox').removeAttr("checked");
				}
			}
			var ids='';
			var vals='';
			jQuery(this).parent().parent().find('.checkbox').each(function() {
				if ((this.dataset.all!='all')&&(jQuery(this).is(":checked"))) {
					if (ids.length==0) {
						ids=jQuery(this).val();
						jQuery(this).parent().find('i').each(function() {
							vals=jQuery(this).html();
						});
					} else {
						ids+=','+jQuery(this).val();
						jQuery(this).parent().find('i').each(function() {
							vals+=','+jQuery(this).html();
						});
					}
				}
			});
			var e=jQuery(this).parent().parent();
			while (!e.is('label')) {
				e=e.parent();
			}
			e.find(".param").each(function(){
				jQuery(this).val(ids);
			});
			e.find(".show").each(function(){
				jQuery(this).val(vals);
			});
		});
	}, 130);
	setTimeout(function(){
		jQuery('.show.multy').bind("blur",function(){
			paMulty(this);
		});
		jQuery('.param.multy').each(function(){
			paMultyInit(this);
		});
	},140);
	setTimeout(function() {
		jQuery('.thead').bind("scroll",function(){
			moveTableHead();
		});
		moveTableHead();
	},150);
}
/*		PARAMS		*/
function paMultyInit(el){
	var e=jQuery(el);
	var v=e.val();
	var a=v.split(',');
	var vals='';
	e.parent().find('.checkbox').each(function(){
		if (a.indexOf(jQuery(this).val())>=0) {
			jQuery(this).attr("checked","checked");
			if (vals.length==0) {
				vals=jQuery(this).parent().find('i').html();
			} else {
				vals+=','+jQuery(this).parent().find('i').html();
			}
		} else {
			jQuery(this).removeAttr("checked");
		}
	});
	while (!e.is('label')){
		e=e.parent();
	}
	e.find(".show").each(function(){
		jQuery(this).val(vals);
	});
}
function paMulty(el) {
	var e=jQuery(el);
	var v=e.val();
	var a=v.toUpperCase().split(',');
	var ids='';
	e.parent().find('i').each(function(){
		if(a.indexOf(jQuery(this).html().toUpperCase())>=0){
			jQuery(this).parent().find('.checkbox').attr("checked","checked");
			if(ids.length==0){
				ids=jQuery(this).parent().find('.checkbox').val();
			}else{
				ids+=','+jQuery(this).parent().find('.checkbox').val();
			}
		}else{
			jQuery(this).parent().find('.checkbox').removeAttr("checked");
		}
	});
	while(!e.is('label')){
		e=e.parent();
	}
	e.find(".param").each(function(){
		jQuery(this).val(ids);
	});
}
function paCalendar(sdt,name){
	var dt=patodt(sdt);
	dt.setDate(1);
	var monyear=['Январь','Февраль','Март','Апрель','Май','Июнь','Июль','Август','Сентябрь',
		'Октябрь','Ноябрь','Декабрь'][dt.getMonth()]+' '+dt.getFullYear();
	var td=new Date();
	var ssi='<table>';
	ssi+='<tr><td colspan="7">'+monyear+'</td></tr>';
	var tdt=new Date(dt.getFullYear(),dt.getMonth(),1);
	ssi+='<tr><td data-diff="'+paGetFDate(new Date(dt.getFullYear(),dt.getMonth(),-365))+'">&#x00ab;'+
		'</td><td data-diff="'+paGetFDate(new Date(dt.getFullYear(),dt.getMonth(),-28))+'">&#x2039;</td>'+
		'<td data-diff="'+paGetDate(new Date())+'" class="today" colspan="3">Сегодня</td>'+
		'<td data-diff="'+paGetFDate(new Date(dt.getFullYear(),dt.getMonth(),32))+'">&#x203a;</td>'+
		'<td data-diff="'+paGetFDate(new Date(dt.getFullYear(),dt.getMonth(),366))+'">&#x00bb;</td></tr>';
	ssi+='<tr><td>П</td><td>В</td><td>С</td><td>Ч</td><td>П</td><td>С</td><td>В</td></tr>';
	var fw=dt.getDay()-1;
	if (fw<0) fw=6;
	var mn=dt.getMonth();
	var r=0;
alert(1);
	var tdt=new Date(dt.getFullYear(),dt.getMonth(),1);
	var vdt=patodt(sdt);
	while (tdt.getMonth()==mn) {
		ssi+='<tr>';
		for (var c=0;c<7;c++) {
			tdt=new Date(dt.getFullYear(),dt.getMonth(),1);
			tdt.setDate(r*7+c-fw+1);
			var cl='';
			if ((tdt.getDate()==vdt.getDate())&&
				(tdt.getMonth()==vdt.getMonth())&&
				(tdt.getFullYear()==vdt.getFullYear())) {
				cl='class="valday"';
			} else if ((tdt.getDate()==td.getDate())&&
				(tdt.getMonth()==td.getMonth())&&
				(tdt.getFullYear()==td.getFullYear())) {
				cl='class="today"';
			} else if (tdt.getMonth()==mn) {
				cl='class="days"';
			}
			ssi+='<td '+cl+' data-diff="'+paGetDate(new Date(dt.getFullYear(),dt.getMonth(),r*7+c-fw+1))+'">'+tdt.getDate()+'</td>';
		}
		ssi+='</tr>';
		r++;
	}
	ssi+='<tr><td colspan="7" class="foot"></td></tr></table>';
//alert(ssi);
	return ssi;
}
function patodt(sdt){
	var dt=sdt.replace(/\//g,'.').replace(/-/g,'.').split('.');
	var tdt=new Date();
	try {
		if (dt.length>2) {
			dt=new Date(dt[2], dt[1]-1, dt[0]);
		} else if (dt.length>1) {
			if (dt[1]-6>tdt.getMonth()) {
				dt=new Date(tdt.getFullYear()-1, dt[1]-1, dt[0]);
			} else {
				dt=new Date(tdt.getFullYear(), dt[1]-1, dt[0]);
			}
		} else if (sdt.length>0) {
			dt=new Date(tdt.getFullYear(), tdt.getMonth(), sdt);
		} else  {
			dt=tdt;
		}
	} catch(e) {
		dt=new Date();
	};
	if (!(dt instanceof Date)) {
		dt=new Date();
	}
	return dt;
}
function paGetFDate(dt) {
	dt.setDate(1);
	return paGetDate(dt);
}
function paGetDate(dt) {
	var r='';
	if (dt.getDate()<10) {
		r+='0'+dt.getDate()+'.';
	} else {
		r+=dt.getDate()+'.';
	}
	if (dt.getMonth()<9) {
		r+='0'+(dt.getMonth()+1)+'.';
	} else {
		r+=(dt.getMonth()+1)+'.';
	}
	r+=dt.getFullYear();
	return r;
}
function plusminus(el){
	var className=jQuery(el).attr('id');
	if(jQuery(el).hasClass('showed')){
		jQuery('.'+className).css('display','none');
		jQuery(el).removeClass('showed');
	}else{
		jQuery('.'+className).css('display','table-row-group');
		jQuery(el).addClass('showed');
		showplusminus('#conteiner');
	}
	setTimeout(function(){
		moveTableHead();
	},100);
	setTimeout(function(){
		moveTableHead();
	},200);
	setTimeout(function(){
		moveTableHead();
	},300);
	setTimeout(function(){
		moveTableHead();
	},400);
	setTimeout(function(){
		moveTableHead();
	},500);
}
function showplusminus(el){
	jQuery(el).find('.level.plus').each(function() {
		if(!jQuery(this).hasClass('showed')){
			jQuery('.'+jQuery(this).attr('id')).css('display','none');
		}
	});
}
function selectorPos(el) {
// всякий раз, когда мы наведем курсор мышки на пункт меню, кот. имеет подменю
	var $Item = jQuery(el);
// определяем координаты относительно ближайшего эл-та родителя, 
// у кот. задан тип позиционирования
	var ItemPos = $Item.position();
// располагаем подменю в корректной позиции относительно пункта меню 
	$Item.find('.selector').css({
		top:ItemPos.top+Math.round($Item.outerHeight()),
		left:ItemPos.left
	});
};