#!/bin/bash

MKGRISU=tools/mkgrisu/redmkgrisu-static
GRISUTABLES=modules/text/src/GrisuTables.cpp

MSGSHDR=tools/msgshdr/redmsgshdr-static
COREMSGKEY=modules/locale/src/CoreMessageKey.hpp

if [ -x "$MKGRISU" ]; then
	"$MKGRISU" -nredengine::text -cGrisu -sCACHED_SIGNIFICANTS -eCACHED_EXPONENTS "$GRISUTABLES"
else
	echo "Missing '$MKGRISU', skipping generation of '$GRISUTABLES'." >&2
fi

if [ -x "$MSGSHDR" ]; then
	"$MSGSHDR" -tredengine::locale::CoreMessageKey -pCMK_ -gREDSTRAIN_MOD_LOCALE_COREMESSAGEKEY_HPP \
			data/locale-core/src/messages.msgsc "$COREMSGKEY"
else
	echo "Missing '$MSGSHDR', skipping generation of '$COREMSGKEY'." >&2
fi
