#!/bin/bash

MKGRISU=tools/mkgrisu/redmkgrisu-static
GRISUTABLES=modules/text/src/GrisuTables.cpp

if [ ! -x "$MKGRISU" -a -x run-mkgrisu ]; then
	MKGRISU=./run-mkgrisu
fi

MSGSHDR=tools/msgshdr/redmsgshdr-static
COREMSGKEY=modules/locale/src/CoreMessageKey.hpp
CALFMTMSGKEY=modules/calendar/src/FormatMessageKey.hpp
TRACEMSGKEY=modules/locale/src/StackTraceMessageKey.hpp

if [ ! -x "$MSGSHDR" -a -x run-msgshdr ]; then
	MSGSHDR=./run-msgshdr
fi

CUNT=tools/cunt/cunt-static
TERMDB=modules/damnation/src/termdb.cpp
TERMCAP_URL=http://invisible-island.net/datafiles/current/termcap.src.gz

if [ ! -x "$CUNT" -a -x run-cunt ]; then
	CUNT=./run-cunt
fi

if [ ! -f "$GRISUTABLES" ]; then
	if [ -x "$MKGRISU" ]; then
		echo "Generating '$GRISUTABLES'..."
		"$MKGRISU" -nredengine::text -cGrisu -sCACHED_SIGNIFICANTS -eCACHED_EXPONENTS "$GRISUTABLES"
	else
		echo "Missing '$MKGRISU', skipping generation of '$GRISUTABLES'." >&2
	fi
fi

function genMsgsHdr {
	if [ ! -f "$4"/src/messages.msgsc ]; then
		return
	elif [ -f "$5" ]; then
		if [ "$(stat -c '%Y' "$4"/src/messages.msgsc)" -le "$(stat -c '%Y' "$5")" ]; then
			return
		fi
	fi
	echo "Generating '$5'..."
	"$MSGSHDR" -t"redengine::$1" -p"$2_" -gREDSTRAIN_"$3"_HPP "$4"/src/messages.msgsc "$5"
}

function genModMsgsHdr {
	# <modns> <modname> <modname_uc>
	genMsgsHdr "$1"::l10n::"$2"ModuleMessageKey MSG DATA_"$3"_L10N_"$3"MODULEMESSAGEKEY data/"$1"-l10n \
			data/"$1"-l10n/src/"$2"ModuleMessageKey.hpp
}

if [ -x "$MSGSHDR" ]; then
	genMsgsHdr locale::CoreMessageKey CMK MOD_LOCALE_COREMESSAGEKEY data/locale-core "$COREMSGKEY"
	genMsgsHdr calendar::FormatMessageKey FMK MOD_CALENDAR_FORMATMESSAGEKEY data/calendar-formats "$CALFMTMSGKEY"
	genMsgsHdr locale::StackTraceMessageKey STMK MOD_LOCALE_STACKTRACEMESSAGEKEY data/stacktrace-l10n "$TRACEMSGKEY"
	genModMsgsHdr error Error ERROR
	genModMsgsHdr platform Platform PLATFORM
	genModMsgsHdr io IO IO
	genModMsgsHdr calendar Calendar CALENDAR
	genModMsgsHdr cmdline CommandLine CMDLINE
else
	echo "Missing '$MSGSHDR', skipping generation of '$COREMSGKEY' and others." >&2
fi

function genTermDB {
	tcsrc="$(basename -- "$TERMCAP_URL")"
	if [ ! -f "$tcsrc" ]; then
		wget -O "$tcsrc" -- "$TERMCAP_URL"
	fi
	if [ -f "$TERMDB" ]; then
		if [ "$(stat -c '%Y' -- "$tcsrc")" -le "$(stat -c '%Y' -- "$TERMDB")" ]; then
			return
		fi
	fi
	if [ ! -x "$CUNT" ]; then
		echo "Missing '$CUNT', skipping generation of '$TERMDB'." >&2
		return
	fi
	echo "Generating '$TERMDB'..."
	zcat -- "$tcsrc" | "$CUNT" -- - "$TERMDB"
}

genTermDB
