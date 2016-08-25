#!/bin/bash

flat=0
if [ "$1" = "--flat" ]; then
	flat=1
	shift
fi
if [ $# != 1 ]; then
	echo "Usage: $(basename -- "$0") [--tree] <source-or-header>" >&2
	exit 1
fi

fstack=()

function follow {
	if [ ! -f "$1" ]; then
		echo "*** No such file: $1" >&2
		return
	fi
	for seen in "${fstack[@]}"; do
		if [ "$seen" = "$1" ]; then
			return
		fi
	done
	fstack+=("$1")
	echo "x$2$1" | sed 's/^x//'
	local nextindent dirn herepfx incpath
	if [ $flat != 0 ]; then
		nextindent="$2"
	else
		nextindent="$2    "
	fi
	grep -E -- '^#include <redstrain/.*>$' "$1" |
	sed -r 's@^#include <@@;s@>$@@;s@^redstrain/([^/]+)/@modules/\1/src/@' |
	while read incf; do
		follow "$incf" "$nextindent"
	done
	dirn="$(dirname -- "$1")"
	herepfx="$(pwd)/"
	grep -E -- '^#include ".*"$' "$1" |
	sed 's/^#include "//;s/"$//' |
	while read incf; do
		incpath="$(readlink -m -- "$dirn/$incf")"
		incpath="${incpath#$herepfx}"
		follow "$incpath" "$nextindent"
	done
	unset fstack[$(expr ${#fstack} - 1)]
}

if [ $flat != 0 ]; then
	follow "$1" '' | sort | uniq
else
	follow "$1" ''
fi
