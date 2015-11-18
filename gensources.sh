#!/bin/bash

MKGRISU=tools/mkgrisu/redmkgrisu-static
GRISUTABLES=modules/text/src/GrisuTables.cpp

MSGSHDR=tools/msgshdr/redmsgshdr-static
COREMSGKEY=modules/locale/src/CoreMessageKey.hpp

if [ -x "$MKGRISU" ]; then
	echo "Generating '$GRISUTABLES'..."
	"$MKGRISU" -nredengine::text -cGrisu -sCACHED_SIGNIFICANTS -eCACHED_EXPONENTS "$GRISUTABLES"
else
	echo "Missing '$MKGRISU', skipping generation of '$GRISUTABLES'." >&2
fi

function genMsgsHdr {
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
	genModMsgsHdr error Error ERROR
else
	echo "Missing '$MSGSHDR', skipping generation of '$COREMSGKEY' and others." >&2
fi
