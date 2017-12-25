$(function() {
 
    $("form[name=form_bind_client] input[name=real_client_name]").autocomplete({
        source: "/panel/formajax/?form=getclient&action=real_client_name",
        minLength: 1,
        select: function(event, ui) {
            $("form[name=form_bind_client] input[name=real_client_id]").val(ui.item.id);
        },
 
        html: true, // optional (jquery.ui.autocomplete.html.js required)
 
      // optional (if other layers overlap autocomplete list)
        open: function(event, ui) {
            $(".ui-autocomplete").css("z-index", 1000);
        },
        change: function(event, ui) {
            if(ui.item == null)
                $("form[name=form_bind_client] input[name=real_client_id]").val("")
        }
    });
    
    $("form[name=form_bind_client] input[name=client_name]").autocomplete({
        source: "/panel/formajax/?form=getclient&action=client_name",
        minLength: 1,
        select: function(event, ui) {
            $("form[name=form_bind_client] input[name=client_id]").val(ui.item.id);
        },
 
        html: true, // optional (jquery.ui.autocomplete.html.js required)
 
      // optional (if other layers overlap autocomplete list)
        open: function(event, ui) {
            $(".ui-autocomplete").css("z-index", 1000);
        }
    });
    
    $("input[name=filter_client_name]").autocomplete({
        source: "/panel/formajax/?form=getclient&action=filter_client_name" +
                "&manager_id=" + $('select#select_manager_id').val(),
        minLength: 1,
        html: true, // optional (jquery.ui.autocomplete.html.js required)
        select: function(event, ui) {
            $("input[name=filter_client_id]").val(ui.item.id);
            var location = window.location;
            var link = location.protocol + "//" +
                    location.host +
                    location.pathname +
                    "?page=0&period=" + $('select[name=period]').val() + 
                    "&manager_id=" + $('select#select_manager_id').val() +
                    "&client_real_id=" + ui.item.id +
                    (location.hash != "" ? location.hash : "");
            window.location.href = link;
        },
 
      // optional (if other layers overlap autocomplete list)
        open: function(event, ui) {
            $(".ui-autocomplete").css("z-index", 1000);
        },
        change: function(event, ui) {
            if(ui.item == null)
                $("input[name=filter_client_id]").val("")
        }
        
    });
 
});