// ==UserScript==
// @name         UF Logo Preview
// @namespace    http://tampermonkey.net/
// @version      1
// @description  Logo preview
// @author       BuGiPoP
// @match        http://foro.unionfansub.com/forumdisplay.php*
// @require https://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    jQuery('.ttitle').hover(function() {
        var link = jQuery(this).children('a[id^=tid_]')[0].href.replace('showthread', 'portada');
        var masinfo = jQuery(jQuery(this).find('.masinfo')[0]);

        // If there's no logo loaded yet, create a container and load it inside
        if(!masinfo.find('div')[0]) {
            masinfo.prepend('<div style="text-align:center;"></div>');

            var logobox = jQuery(masinfo.find('div')[0]);
            // Retrieve image url
            logobox.load(link, function(response) {
                // Add image to load
                logobox.html('<div class="portada" style="background-image:url(' + response + ')"></div>');
                // Style
                jQuery(logobox.children()[0]).css('margin', '0');
            });
        }
    }, function() {});
})();
