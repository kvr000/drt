function prepend_path
{
	local n
	local p
	local a
	n=$1
	eval "p=\$$n"
	a=$2
	p=`echo ":$p:" | sed "s#:$a:##" | sed 's/^://' | sed "s#^#$a:#" | sed 's/:$//'`
	eval "export $n=\"\$p\""
}

function main
{
	local p

	result=

	if ! echo $0 | grep 'senv\.sh$' > /dev/null; then
		if [ ! -f senv.sh ]; then
			echo "error: using shell that doesn't pass correct script name, you must be in drt-root" >&2
			result='( exit 2 )'
		fi
		p=
	else
		p=`echo $0 | sed 's-^./--' | sed 's-[^/]*$--'`
	fi

	if [ -z "$result" ]; then
		result=true

		if echo $p | grep -v ^/ >/dev/null; then
			p=$PWD/$p
		fi
		o=""
		while [ "$p" != "$o" ]; do
			o=$p
			p=`echo $p | sed 's-/[^/]*/\.\./-/-'`
		done
		export DR_ROOT=$p

		prepend_path LD_LIBRARY_PATH "${p}lib"
		prepend_path PATH "${p}util:${p}bin"

		if [ $# -gt 0 ]; then
			export DR_TARG=$1
		fi

		case `uname -m` in
		i[0-9]86)
			DR_ARCH=x86
			;;

		alpha)
			DR_ARCH=alpha
			;;

		mips)
			DR_ARCH=mipsel
			;;

		x86_64)
			DR_ARCH=x86_64
			;;

		*)
			echo unknown machine `uname -m` >&2
			DR_ARCH=gen
			result=false
			;;
		esac
		export DR_ARCH

		case `uname -s` in
		FreeBSD)
			DR_OSTYPE=unix
			;;

		Linux)
			DR_OSTYPE=unix
			;;

		Win32)
			DR_OSTYPE=wapi
			;;

		Mac)
			DR_OSTYPE=mac
			;;

		CYGWIN_NT-5.1)
			DR_OSTYPE=unix
			;;

		CYGWIN_NT-6.1-WOW64)
			DR_OSTYPE=unix
			;;

		*)
			echo unknown operating system `uname -s` >&2
		esac
		export DR_OSTYPE

		export DR_TARG=${DR_TARG:-fg}
		export DRO_MM=${DRO_MM:-c}

	fi

	eval $result
}

main
