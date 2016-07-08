#!/bin/bash

for d in data/*-l10n; do
	mod="modules/$(basename "$d" | sed 's/-l10n$//')"
	if [ ! -d "$mod" ]; then
		continue
	fi
	find "$mod"/src -name '*Error.cpp' | while read msrc; do
		cls="$(basename "$msrc" | sed 's/\.cpp$//')"
		if ! grep -Fq "REDSTRAIN_DEFINE_ERROR($cls)" "$msrc"; then
			continue
		fi
		fntail="$(echo "$msrc" | sed "s#^$mod/src/##")"
		dsrc="$d/src/$fntail"
		if [ ! -f "$dsrc" ]; then
			echo "Have '$msrc', but not '$dsrc'."
		fi
	done
done
