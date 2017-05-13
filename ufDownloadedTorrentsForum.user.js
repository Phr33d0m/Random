// ==UserScript==
// @name         UF Downloaded Torrents
// @namespace    http://tampermonkey.net/
// @version      3
// @description  See different torrent icon for downloaded torrents in the forum
// @downloadURL  https://rawgit.com/Phr33d0m/Random/master/ufDownloadedTorrentsForum.user.js
// @author       BuGiPoP
// @match        http://foro.unionfansub.com/forumdisplay.php*
// @require      https://code.jquery.com/jquery-3.2.1.min.js
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    var MY_ID = $('#panel strong > a')[0].href.replace('http://foro.unionfansub.com/member.php?action=profile&uid=', '');
    var API_SEEDS = 'http://foro.unionfansub.com/seedeando.php?uid=' + MY_ID;
    var TIDS = [];

    $('.ttitle > a').each(function(index, item) {
        // Get tid from a.href
        var tid = item.href.replace('http://foro.unionfansub.com/showthread.php?tid=', '');
        TIDS.push(tid);

        // Add a helper attr to the td.ttitle
        $(item).parent('td').attr('tid', tid);
    });

    // Get seeded threads only from all the threads
    $.get(API_SEEDS, { 'tid[]': TIDS }).done(function(result) {
        var threads = JSON.parse(result);
        if(threads && threads.length) {
            threads.forEach(function(thread) {
                // Change icon for seeded threads
                $($('.ttitle[tid=' + thread + ']').closest('tr').find('span.host.torrent')[0])
                    .addClass('downloaded')
                    .attr('title', 'Torrent Descargado');
            });
        }
    });
})();
