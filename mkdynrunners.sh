#!/bin/bash

basedir="$(readlink -f -- "$(dirname -- "$0")")"

libdirs=''
for d in "$basedir"/modules/* "$basedir"/data/*; do
	if [ -d "$d" ]; then
		if [ -z "$libdirs" ]; then
			libdirs="$d"
		else
			libdirs="$libdirs:$d"
		fi
	fi
done

tooltpl=''
if [ -f "$basedir/project.properties" ]; then
	tooltpl="$(grep -E '^tool\.basename=' "$basedir/project.properties" | sed 's/^[^=]*=//')"
fi
if [ -z "$tooltpl" ]; then
	tooltpl='%project%%tool%'
fi

for d in "$basedir"/tools/*; do
	if [ -d "$d" ]; then
		if [ -f "$d/component.properties" ]; then
			cname="$(grep -E '^component\.name=' "$d/component.properties" | sed 's/^[^=]*=//')"
		fi
		if [ -z "$cname" ]; then
			cname="$(basename -- "$d")"
		fi
		txname="$(echo "x$tooltpl" | sed "s/^x//;s/%project%/redstrain/g;s#%tool%#$cname#g")"
		scrname="$basedir/run-$(basename "$d")"
	cat >"$scrname" <<EOF
#!/bin/bash

libdirs="$libdirs"
if [ -z "\$LD_LIBRARY_PATH" ]; then
	export LD_LIBRARY_PATH="\$libdirs"
else
	export LD_LIBRARY_PATH="\$libdirs:\$LD_LIBRARY_PATH"
fi
export REDSTRAIN_PRINT_STACK_TRACES=1
if [ -n "\$RSDEBUG" ]; then
	gdb $d/$txname
else
	$d/$txname "\$@"
fi
EOF
	chmod 755 "$scrname"
	fi
done
