$(document).ready(function() {
	if ($('#firstForm').length>0) {
		showform($('#firstForm')[0]);
	}
	$('.form').bind("click", function(e) {
		showform(this);
		e.preventDefault();
	});
	if ($('.firstReport').length>0) {
		$('.firstReport').each(function() {
			showreport(this);
		});
	}
	$('.report').bind("click", function(e) {
		showreport(this);
		e.preventDefault();
	});
});
/*						FORMS					*/
function showform(e) {
	var form='';
	var data=[];
	var index=0;
	if (e.dataset.form) {
		form='form='+e.dataset.form;
	}
	$('#conteiner').find('table').each(function() {
		if ((form.length==0)&&(this.dataset.form)) {
			form='form='+this.dataset.form;
		}
		$(this).find('input').each(function() {
			data[index]=$(this).val();
			index++;
		});
	});
	if (e.dataset.state) {
		form+='&state='+e.dataset.state;
	}
	if (e.dataset.key) {
		form+='&key='+e.dataset.key;
	}
	if (e.dataset.params) {
		form+='&params='+e.dataset.params;
	}
//alert(data.length);
	if (data.length>0) {
		form+='&data='+JSON.stringify(data);
	}
//alert(form);
	$('#conteiner').html('<br/><center><strong>Идет подготовка формы<br/>Ждите...</strong></center>');
	$.ajax({
		type: "POST",
		url: '/panel/formajax',
		data: form,
		success: function(data){
			$('#conteiner').html(data);
			forminit();
		}
	});
	return false;
}
function forminit() {
	setTimeout(function() {
		$('#conteiner .new').bind("click", function(e) {
			showform(this);
			e.preventDefault();
		});
		$('#conteiner .edit').bind("click", function(e) {
			showform(this);
			e.preventDefault();
		});
		$('#conteiner .delete').bind("click", function(e) {
			showform(this);
			e.preventDefault();
		});
		$('#conteiner .cancel').bind("click", function(e) {
			showform(this);
			e.preventDefault();
		});
		$('#conteiner .save').bind("click", function(e) {
			showform(this);
			e.preventDefault();
		});
	},190);
	setTimeout(function() {
		$('#conteiner .param.calendar').each(function() {
			var name=$(this).attr('name');
			$("div[data-name='"+name+"']").html(paCalendar($(this).val(),name));
		})
	},100);
	setTimeout(function() {
		$("#conteiner div.selector.calendar td").bind("mouseover", function() {
			var diff=this.dataset.diff;
			$(this).parent().parent().find(".foot").each(function() {
				$(this).html(diff);
			});
		});
	},105);
	setTimeout(function() {
		$("#conteiner div.selector.calendar td").bind("click", function() {
			var e=$(this);
			while (!e.is('label')) {
				e=e.parent();
			}
			var diff=this.dataset.diff;
			e.find(".param").each(function() {
				$(this).val(diff);
				reportinit();
			});
		});
	},110);
	setTimeout(function() {
		$('#conteiner .param.calendar').bind("blur", function() {
			var dt=paGetDate(patodt($(this).val()));
			if (dt!=$(this).val()) {
				$(this).val(dt);
				reportinit();
			}
		})
	},120);
	setTimeout(function() {
		$('.thead').bind("scroll", function() {
			moveTableHead();
		});
		moveTableHead();
	},150);
}
/*						REPORT						*/
function showfromreport(e) {
	var rep='report='+e.dataset.rep+'&type=html';
	var prm={};
	$('#conteiner').find('.param').each(function() {
		prm[''+$( this ).attr('name')]=''+$( this ).val();
	});
	rep+='&params='+JSON.stringify(prm);
	$('#conteiner').html('<br/><center><strong>Идет подготовка отчета<br/>Ждите...</strong></center>');
	$.ajax({
		type: "POST",
		url: '/panel/reportajax',
		data: rep,
		success: function(txt){
			$('#conteiner').html(txt);
			reportinit();
		}
	});
	return false;
}
function showreport(e) {
	var cont='#conteiner';
	if (e.dataset.cont) {
		cont=e.dataset.cont;
	}
	var rep='report='+e.dataset.rep+'&type=html';
	if (e.dataset.params) rep+='&params='+e.dataset.params;
	$(cont).html('<br/><center><strong>Идет подготовка отчета<br/>Ждите...</strong></center>');
	$.ajax({
		type: "POST",
		url: '/panel/reportajax',
		data: rep,
		success: function(data){
			$(cont).html(data);
			reportinit();
		}
	});
	return false;
}
function moveTableHead() {
	var cont=document.getElementById("conteiner");
	var srcDiv=cont.getElementsByClassName("thead");
	for (var i=0;i<srcDiv.length;i++) {
		var scTop=srcDiv[i].scrollTop;
		var tbls=srcDiv[i].getElementsByTagName("table");
		for (var t=0;t<tbls.length;t++) {
			var id=tbls[t].id;
			if (id.split("_").length==1) { /* нащел исходную таблу */
				var clone=document.getElementById(id+"_clone");
				if (!clone) { /* создаем */
					var colgrp=tbls[t].getElementsByTagName("colgroup");
					if (colgrp.length>0) colgrp='<colgroup>'+colgrp[0].innerHTML+'</colgroup>'
					else colgrp='';
					var src=tbls[t].getElementsByTagName("thead");
					srcDiv[i].innerHTML='<table id="'+id+'_clone">'+colgrp+src[0].outerHTML+'</table>'+srcDiv[i].innerHTML;
					clone=document.getElementById(id+"_clone");
					clone.style["z-index"]="10";
				}
				for (var r=0;r<clone.rows.length;r++) {
					for (var c=0;c<clone.rows[r].cells.length;c++) {
						if (clone.rows[r].cells[c].offsetWidth!=
						 tbls[t].rows[r].cells[c].offsetWidth) {
							var pw=0;
							try {pw=clone.rows[r].cells[c].style['min-width'];
							} catch (e) {}
							pw=(pw.replace('px','')*1);
							if (pw==0) {
								pw+=tbls[t].rows[r].cells[c].offsetWidth;
							} else {
								pw+=((tbls[t].rows[r].cells[c].offsetWidth-
									clone.rows[r].cells[c].offsetWidth)/1.3);
							}
							clone.rows[r].cells[c].style['min-width']=pw+'px';
							clone.rows[r].cells[c].style['max-width']=pw+'px';
						}
					}
				}
				if (scTop<(clone.offsetHeight/10)) {
					clone.style['top']=(-clone.offsetHeight)+'px';
					tbls[t].style['top']=(-clone.offsetHeight)+'px';
				} else {
					clone.style['top']=scTop+'px';
				}
			}
		}
	}
}
var dateTimeDefaults={
	closeText: 'Готово',
	currentText: 'Сейчас',
	dateFormat: 'dd.mm.yy',
	timeFormat: 'HH:mm',
	monthNames: [ "Январь","Февраль","Март","Апрель","Май","Июнь","Июль","Август","Сентябрь","Октябрь","Ноябрь","Декабрь" ],
	dayNamesShort: [ "Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб" ],
	dayNamesMin: [ "Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб" ],
	firstDay: 1,
	timeOnlyTitle: 'Выберите время',
	timeText: 'Время',
	hourText: 'Часы',
	minuteText: 'Минуты',
	secondText: 'Секунды',
	autoclose: true,
};
function reportinit() {
	setTimeout(function() {
    	$( ".param.date" ).blur(function(){
//			$(this).val(checkDateTime($(this).val(),'dd.mm.yyyy'));
		});
		$(".param.date").datepicker(dateTimeDefaults);
	},100);	
	setTimeout(function() {
    	$( ".param.time" ).blur(function(){
//			$(this).val(checkDateTime($(this).val(),'hh:MM'));
		});
		$(".param.time").timepicker(dateTimeDefaults);
	},110);	
	setTimeout(function() {
		$(".param.datetime").datetimepicker(dateTimeDefaults);
    	$( ".param.datetime" ).blur(function(){
//			$(this).val(checkDateTime($(this).val(),'dd.mm.yyyy hh:MM'));
		});
	},120);	
	setTimeout(function() {
		$('.selector.multy .checkbox').bind("click", function() {
			if (this.dataset.all=='all') {
				if ($(this).is(":checked")) {
					$(this).parent().parent().find('.checkbox').attr("checked","checked");
				} else {
					$(this).parent().parent().find('.checkbox').removeAttr("checked");
				}
			}
			var ids='';
			var vals='';
			$(this).parent().parent().find('.checkbox').each(function() {
				if ((this.dataset.all!='all')&&($(this).is(":checked"))) {
					if (ids.length==0) {
						ids=$(this).val();
						$(this).parent().find('i').each(function() {
							vals=$(this).html();
						});
					} else {
						ids+=','+$(this).val();
						$(this).parent().find('i').each(function() {
							vals+=','+$(this).html();
						});
					}
				}
			});
			var e=$(this).parent().parent();
			while (!e.is('label')) {
				e=e.parent();
			}
			e.find(".param").each(function() {
				$(this).val(ids);
			});
			e.find(".show").each(function() {
				$(this).val(vals);
			});
		});
	}, 130);
	setTimeout(function() {
		$('.show.multy').bind("blur", function() {
			paMulty(this);
		});
		$('.param.multy').each(function() {
			paMultyInit(this);
		});
	},140);
	setTimeout(function() {
		$('.thead').bind("scroll", function() {
			moveTableHead();
		});
		moveTableHead();
	},150);
}
/*						PARAMS						*/
function paMultyInit(el) {
	var e=$(el);
	var v=e.val();
	var a=v.split(',');
	var vals='';
	e.parent().find('.checkbox').each(function() {
		if (a.indexOf($(this).val())>=0) {
			$(this).attr("checked","checked");
			if (vals.length==0) {
				vals=$(this).parent().find('i').html();
			} else {
				vals+=','+$(this).parent().find('i').html();
			}
		} else {
			$(this).removeAttr("checked");
		}
	});
	while (!e.is('label')) {
		e=e.parent();
	}
	e.find(".show").each(function() {
		$(this).val(vals);
	});
}
function paMulty(el) {
	var e=$(el);
	var v=e.val();
	var a=v.toUpperCase().split(',');
	var ids='';
	e.parent().find('i').each(function() {
		if (a.indexOf($(this).html().toUpperCase())>=0) {
			$(this).parent().find('.checkbox').attr("checked","checked");
			if (ids.length==0) {
				ids=$(this).parent().find('.checkbox').val();
			} else {
				ids+=','+$(this).parent().find('.checkbox').val();
			}
		} else {
			$(this).parent().find('.checkbox').removeAttr("checked");
		}
	});
	while (!e.is('label')) {
		e=e.parent();
	}
	e.find(".param").each(function() {
		$(this).val(ids);
	});
}
function paCalendar(sdt,name) {
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
//alert(1);
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
function patodt(sdt) {
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
