#!/bin/bash

MKGRISU=tools/mkgrisu/redmkgrisu-static
GRISUTABLES=modules/text/src/GrisuTables.cpp

if [ -x "$MKGRISU" ]; then
	"$MKGRISU" -nredengine::text -cGrisu -sCACHED_SIGNIFICANTS -eCACHED_EXPONENTS "$GRISUTABLES"
else
	echo "Missing '$MKGRISU', skipping generation of '$GRISUTABLES'." >&2
fi
