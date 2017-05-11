// ==UserScript==
// @name         UF Seeded Threads
// @namespace    http://tampermonkey.net/
// @version      1.0
// @description  View a title icon for seeded threads
// @downloadURL  https://rawgit.com/Phr33d0m/Random/master/ufSeededThreads.user.js
// @author       BuGiPoP
// @match        http://foro.unionfansub.com/forumdisplay.php*
// @require      https://code.jquery.com/jquery-3.2.1.min.js
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    $('.ttitle').each(function(index, item) {
        var myname = $('#panel strong > a').text(),
            link = null,
            peers = [],
            _peers = $('<div>');

        // Fetch torrent peers
        link = $(this).children('a').length === 2 ? $(this).children('a')[1].href : $(this).children('a')[0].href;

        _peers.load(link + ' .ficha .torrent_peers a', null, function() {
            // Get peers' names
            _peers.children().each(function(index, peer) {
                peers.push(peer.text);
            });

            // Check if I'm a peer
            if(peers.indexOf(myname) > -1) {
                $(item).children('a').html('<span class="host torrent"></span>' + $(item).children('a').text());
            }
        });
    });
})();
