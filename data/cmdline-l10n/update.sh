#!/bin/bash

cd "$(dirname -- "$0")" || exit 1

REDUPDMSGS=../../tools/updmsgs/redupdmsgs-static

if [ ! -x "$REDUPDMSGS" -a -x ../../run-updmsgs ]; then
	REDUPDMSGS=../../run-updmsgs
fi

for def in src/*.msgdf; do
	echo "=== Updating: $def"
	if [ "$(basename -- "$def")" = en_US.msgdf ]; then
		"$REDUPDMSGS" -v src/messages.msgsc "$def" || exit 1
	else
		"$REDUPDMSGS" -v -fsrc/en_US.msgdf src/messages.msgsc "$def" || exit 1
	fi
done
