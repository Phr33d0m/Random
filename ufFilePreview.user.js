// ==UserScript==
// @name         UF File Preview
// @namespace    http://tampermonkey.net/
// @version      3.1
// @description  File preview
// @downloadURL  https://rawgit.com/Phr33d0m/Random/master/ufFilePreview.user.js
// @author       BuGiPoP
// @match        http://foro.unionfansub.com/forumdisplay.php*
// @require      https://code.jquery.com/jquery-3.1.1.min.js
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    document.styleSheets[0].insertRule(".masinfo {z-index: 1 !important}", 0);
    document.styleSheets[0].insertRule(".masinfo span {display: inline-block !important}", 0);
    document.styleSheets[0].insertRule(".masinfo .ficha div {line-height: 12px !important}", 0);

    $('.ttitle').hover(function() {
        var link = $(this).children('a').length === 2 ? $(this).children('a')[1].href : $(this).children('a')[0].href;

        // If there's no masinfo element, add it
        if(!$(this).find('.masinfo').length) {
            $(this).append('<span class="masinfo"></span>');
        }

        var masinfo = $($(this).find('.masinfo')[0]);

        // If there's no file loaded yet, load it
        if(!masinfo.find('.ficha')[0]) {
            masinfo.text('Cargando...');
            masinfo.load(link + ' .ficha');
        }
    }, function() {});
    
    // Pre-load everything on load. Use this responsibly
    // $('.ttitle').each(function(index, item) {
    //     $(item).trigger('mouseenter');
    // });
})();
