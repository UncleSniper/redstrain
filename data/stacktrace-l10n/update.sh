#!/bin/bash

cd "$(dirname -- "$0")" || exit 1

for def in src/*.msgdf; do
	echo "=== Updating: $def"
	../../tools/updmsgs/redupdmsgs-static -v src/messages.msgsc "$def" || exit 1
done
