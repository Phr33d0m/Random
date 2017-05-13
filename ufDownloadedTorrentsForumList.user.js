// ==UserScript==
// @name         UF Downloaded Torrents (List)
// @namespace    http://tampermonkey.net/
// @version      2
// @description  See different torrent icon for downloaded torrents in the forum
// @downloadURL  https://rawgit.com/Phr33d0m/Random/master/ufDownloadedTorrentsForumList.user.js
// @author       BuGiPoP
// @match        http://foro.unionfansub.com/announcements.php?aid=3
// @require      https://code.jquery.com/jquery-3.2.1.min.js
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    document.styleSheets[0].insertRule(".content a { position: relative;padding-left: 20px !important; }", 0);
    document.styleSheets[0].insertRule(".content a.downloaded:before { content: '';position: absolute;display: block;left: 0px;width: 16px;height: 16px;background: url(https://i.imgur.com/viQxL7l.png); }", 0);

    var MY_ID = $('#panel strong > a')[0].href.replace('http://foro.unionfansub.com/member.php?action=profile&uid=', '');
    var API_SEEDS = 'http://foro.unionfansub.com/seedeando.php?uid=' + MY_ID;
    var TIDS = [];

    $('.content a').each(function(index, item) {
        // Get tid from a.href
        var tid = item.href.replace('http://foro.unionfansub.com/showthread.php?tid=', '');
        TIDS.push(tid);

        // Add a helper attr
        $(item).attr('tid', tid);
    });

    // Get seeded threads only from all the threads
    var _TIDS = [];
    for(var i = 0; i < TIDS.length; i++) {
        if(i%50 !== 0 && TIDS.length !== (i + 1)) {
            _TIDS.push(TIDS[i]);
        }
        else if(i !== 0) {
            getAPI(_TIDS);

            _TIDS = [];
            _TIDS.push(TIDS[i]);
        }
    }

    function getAPI(_TIDS) {
        $.get(API_SEEDS, { 'tid[]': _TIDS }).done(function(result) {
            var threads = JSON.parse(result);
            if(threads && threads.length) {
                threads.forEach(function(thread) {
                    // Change icon for seeded threads
                    $($('.content a[tid=' + thread + ']')[0]).addClass('downloaded');
                });
            }
        });
    }
})();
