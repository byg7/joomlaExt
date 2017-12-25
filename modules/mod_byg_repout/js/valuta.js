var usd_ch = 0;
var eur_ch = 0;
function finance() {
    $.post('https://query.yahooapis.com/v1/public/yql', {
   	    q: 'select * from yahoo.finance.xchange where pair = "USDRUB,EURRUB"',
       	format: 'json',
        env: 'store://datatables.org/alltableswithkeys',
   	    callback: ''
    }, function (nw) {
   	    $('.finance').hide();
       	var usd;
        var eur;
   	    usd = nw.query.results.rate[0];
       	eur = nw.query.results.rate[1];
        $('.usd-now').html(usd.Rate);
   	    if (usd_ch == 0) {
       	    usd_ch = usd.Rate;
           	if ((usd.Rate - usd.Ask) < 0) {
               	$('.usd-change').html((usd.Rate - usd.Ask).toFixed(4));
                $('.usd-index').css('color', 'red');
   	            $('#usd-inx').attr('class', 'glyphicon glyphicon-chevron-down');
       	    } else {
           	    $('.usd-change').html('+' + (usd.Rate - usd.Ask).toFixed(4));
               	$('.usd-index').css('color', 'green');
                $('#usd-inx').attr('class', 'glyphicon glyphicon-chevron-up');
   	        }
       	} else {
            if ((usd.Rate - usd_ch) < 0) {
   	            $('.usd-change').html((usd.Rate - usd_ch).toFixed(4));
       	        $('.usd-index').css('color', 'red');
           	    $('#usd-inx').attr('class', 'glyphicon glyphicon-chevron-down');
            } else {
   	            $('.usd-change').html('+' + (usd.Rate - usd_ch).toFixed(4));
       	        $('.usd-index').css('color', 'green');
           	    $('#usd-inx').attr('class', 'glyphicon glyphicon-chevron-up');
            }
   	        usd_ch = usd.Rate;
       	}
	        $('.eur-now').html(eur.Rate);
   	    if (eur_ch == 0) {
       	    eur_ch = eur.Rate;
            if ((eur.Rate - eur.Ask) < 0) {
   	            $('.eur-change').html((eur.Rate - eur.Ask).toFixed(4));
       	        $('.eur-index').css('color', 'red');
           	    $('#eur-inx').attr('class', 'glyphicon glyphicon-chevron-down');
            } else {
   	            $('.eur-change').html('+' + (eur.Rate - eur.Ask).toFixed(4));
       	        $('.eur-index').css('color', 'green');
           	    $('#eur-inx').attr('class', 'glyphicon glyphicon-chevron-up');
            }
   	    } else {
       	    if ((eur.Rate - eur_ch) < 0) {
           	    $('.eur-change').html((eur.Rate - eur_ch).toFixed(4));
               	$('.eur-index').css('color', 'red');
                $('#eur-inx').attr('class', 'glyphicon glyphicon-chevron-down');
   	        } else {
       	        $('.eur-change').html('+' + (eur.Rate - eur_ch).toFixed(4));
                $('.eur-index').css('color', 'green');
   	            $('#eur-inx').attr('class', 'glyphicon glyphicon-chevron-up');
       	    }
           	eur_ch = eur.Rate;
        }
   	    $('.finance').fadeIn('slow');
    });
	setTimeout(function(){
		finance();
	}, 10000);
}
finance();