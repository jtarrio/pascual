my_dir="$(dirname $0)"
PC="${PC:-${my_dir}/../compiler/pascual}"
CC="${CC:-gcc}"
PINCDIR="${PINCDIR:-${my_dir}/../runtime}"
PLIBDIR="${PLIBDIR:-${my_dir}/../runtime}"

cleanups=()
add_cleanup() {
  cleanups+=("$1")
  trap 'rm -f "${cleanups[@]}"' EXIT
}

do_compile_with_fpc() {
  infile="$(mktemp --suffix=.pas)"
  add_cleanup "$infile"
  add_cleanup "${infile%.pas}.o"
  cat > "$infile"
  fpc -Mtp "$infile" -o"$1" >/dev/null 2>&1
}

do_test_syntax() {
  if [ "$PC" = 'fpc' ]
  then
    outfile="$(mktemp)"
    add_cleanup "$outfile"
    do_compile_with_fpc "$outfile"
  else
    "$PC" - -o /dev/null 2>/dev/null
  fi
}

do_compile() {
  outfile="$(mktemp)"
  if [ "$PC" = 'fpc' ]
  then
    do_compile_with_fpc "$outfile"
  else
    "$PC" - -o - | "$CC" -x c -I"$PINCDIR" -L"$PLIBDIR" - -o "$outfile" -lpascual
  fi
  echo "$outfile"
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

outputs() {
  outfile=$(do_compile)
  add_cleanup "$outfile"
  "$outfile" | diff -u --label expected <(echo -n "$1") --label actual - || (echo "Program output does not match in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
}

inputs_and_outputs() {
  outfile=$(do_compile)
  add_cleanup "$outfile"
  echo -n "$1" | "$outfile" | diff -u --label expected <(echo -n "$2") --label actual - || (echo "Program output does not match in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
}

reads_and_outputs() {
  outfile=$(do_compile)
  add_cleanup "$outfile"
  "$outfile" <(echo -n "$1") | diff -u --label expected <(echo -n "$2") --label actual - || (echo "Program output does not match in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 )
}

inputs_and_writes() {
  outfile=$(do_compile)
  add_cleanup "$outfile"
  echo -n "$1" | "$outfile" >(diff -u --label expected <(echo -n "$2") --label actual - || (echo "Program output does not match in ${BASH_SOURCE[1]}:${BASH_LINENO}" ; exit 1 ))
}
