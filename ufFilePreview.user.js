// ==UserScript==
// @name         UF File Preview
// @namespace    http://tampermonkey.net/
// @version      2
// @description  File preview
// @downloadURL  https://rawgit.com/Phr33d0m/Random/master/ufFilePreview.user.js
// @author       BuGiPoP
// @match        http://foro.unionfansub.com/forumdisplay.php*
// @require https://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    document.styleSheets[0].insertRule(".masinfo span {display: inline-block !important}", 0);
    document.styleSheets[0].insertRule(".masinfo .ficha div {line-height: 12px !important}", 0);

    jQuery('.ttitle').hover(function() {
        var link = jQuery(this).children('a[id^=tid_]')[0].href;
        var masinfo = jQuery(jQuery(this).find('.masinfo')[0]);

        // If there's no file loaded yet, load it
        if(!masinfo.find('.ficha')[0]) {
            masinfo.text('Cargando...');
            masinfo.load(link + ' .ficha');
        }
    }, function() {});
})();
