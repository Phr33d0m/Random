#!/bin/bash

## VARS

APP_NAME="$(qdbus org.ktorrent.ktorrent /MainApplication org.qtproject.Qt.QCoreApplication.applicationName)"
APP_VER="$(qdbus org.ktorrent.ktorrent /MainApplication org.qtproject.Qt.QCoreApplication.applicationVersion)"
ARGS="$1"

TORRENTS_IDS=0
TORRENTS_COUNT_TOTAL=0
TORRENTS_COUNT_RUNNING=0

SPEED_DL=0
SPEED_DL_RAW=0
SPEED_UP=0
SPEED_UP_RAW=0

SIZE=0
SIZE_RAW=0
SIZE_COMPL=0
SIZE_COMPL_RAW=0

PERCENTAGE=0

TORRENTS_IDS=($(qdbus org.ktorrent.ktorrent /core torrents))
TORRENTS_COUNT_RUNNING=$(qdbus org.ktorrent.ktorrent /core numTorrentsRunning)
TORRENTS_COUNT_TOTAL=${#TORRENTS_IDS[@]}

declare -a OUTPUT

## AUX FUNCTION: PRINT OUTPUT & END

function end {
	for LINE in ${!OUTPUT[*]}; do
		echo "[${APP_NAME}]: ${OUTPUT[LINE]}"
	done

	exit
}

## 0. GET RUNNING / TOTAL
function getinitinfo {
	if (( ${TORRENTS_COUNT_TOTAL} > 0 )); then
		OUTPUT[0]="Torrents: ${TORRENTS_COUNT_RUNNING} running / ${TORRENTS_COUNT_TOTAL} total"
	else
		OUTPUT[0]="No active torrents"
		end
	fi
}


## 1. GET INDIVIDUAL TORRENT INFO

OUTPUT_LINE=1
function gettorrents {
	for ID in ${TORRENTS_IDS[@]}; do

		TORR_NAME="$(qdbus org.ktorrent.ktorrent /torrent/${ID} name)"
		TORR_SPEED_DL_RAW=$(qdbus org.ktorrent.ktorrent /torrent/${ID} downloadSpeed)
		TORR_SPEED_DL="$(numfmt --to=iec-i --suffix=B ${TORR_SPEED_DL_RAW})"
		TORR_SPEED_UP_RAW=$(qdbus org.ktorrent.ktorrent /torrent/${ID} uploadSpeed)
		TORR_SPEED_UP="$(numfmt --to=iec-i --suffix=B ${TORR_SPEED_UP_RAW})"
		TORR_SIZE_RAW=$(qdbus org.ktorrent.ktorrent /torrent/${ID} totalSize)
		TORR_SIZE="$(numfmt --to=iec-i --suffix=B ${TORR_SIZE_RAW})"
		TORR_SIZE_COMPL_RAW=$(qdbus org.ktorrent.ktorrent /torrent/${ID} bytesDownloaded)
		TORR_SIZE_COMPL="$(numfmt --to=iec-i --suffix=B ${TORR_SIZE_COMPL_RAW})"
		TORR_PERCENTAGE="$((TORR_SIZE_COMPL_RAW*100/TORR_SIZE_RAW))%"

		if [[ ${ARGS} == "--verbose" ]]; then
			OUTPUT[OUTPUT_LINE]="${OUTPUT_LINE}. ${TORR_NAME} | Speed: ${TORR_SPEED_DL}/s ~ ${TORR_SPEED_UP}/s | Completed: ${TORR_PERCENTAGE}% (${TORR_SIZE_COMPL} / ${TORR_SIZE})"

			OUTPUT_LINE=$((OUTPUT_LINE + 1))
		fi

		## 1.2. SUM UP TOTALS

		SPEED_DL_RAW=$((SPEED_DL_RAW + TORR_SPEED_DL_RAW))
		SPEED_UP_RAW=$((SPEED_UP_RAW + TORR_SPEED_UP_RAW))
		SIZE_RAW=$((SIZE_RAW + TORR_SIZE_RAW))
		SIZE_COMPL_RAW=$((SIZE_COMPL_RAW + TORR_SIZE_COMPL_RAW))
	done
}


## 2. GET TOTALS

function gettotals {
	SPEED_DL="$(numfmt --to=iec-i --suffix=B ${SPEED_DL_RAW})"
	SPEED_UP="$(numfmt --to=iec-i --suffix=B ${SPEED_UP_RAW})"
	SIZE="$(numfmt --to=iec-i --suffix=B ${SIZE_RAW})"
	SIZE_COMPL="$(numfmt --to=iec-i --suffix=B ${SIZE_COMPL_RAW})"
	PERCENTAGE=$((SIZE_COMPL_RAW*100/SIZE_RAW))

	OUTPUT[OUTPUT_LINE]="Total speed: ${SPEED_DL}/s ~ ${SPEED_UP}/s | Completed: ${PERCENTAGE}% (${SIZE_COMPL} / ${SIZE})"
}


## 3. BUILD INFO

getinitinfo
gettorrents
gettotals
end