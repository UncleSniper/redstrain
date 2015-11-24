#!/bin/bash

cd "$(dirname -- "$0")" || exit 1

for def in src/*.msgdf; do
	echo "=== Updating: $def"
	if [ "$(basename -- "$def")" = en_US.msgdf ]; then
		../../tools/updmsgs/redupdmsgs-static -v src/messages.msgsc "$def" || exit 1
	else
		../../tools/updmsgs/redupdmsgs-static -v -fsrc/en_US.msgdf src/messages.msgsc "$def" || exit 1
	fi
done
