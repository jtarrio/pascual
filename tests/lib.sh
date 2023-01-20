my_dir="$(dirname $0)"
PC="${PC:-${my_dir}/../compiler/pascual}"

do_test_syntax() {
  if [ "$PC" = 'fpc' ]
  then
    infile="$(mktemp --suffix=.pas)"
    outfile="$(mktemp)"
    trap "rm -f \"$infile\" \"$outfile\" \"${outfile}.o\"" EXIT
    cat > "$infile"
    fpc "$infile" -o"$outfile" >/dev/null 2>&1
  else
    "$PC" - -o /dev/null 2>/dev/null
  fi
}

is_valid() {
  do_test_syntax || ( echo "Program is not valid in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
}

is_not_valid() {
  do_test_syntax && ( echo "Program is valid in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
}

will_be_valid() {
  if [ "$PC" = 'fpc' ]
  then
    do_test_syntax || ( echo "Program is not valid in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
  else
    do_test_syntax && ( echo "Program is valid in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
  fi
}
