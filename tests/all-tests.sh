#!/bin/bash

me="$(basename $0)"

for test in *.sh
do
  if [ "$(basename $test)" != "$me" ]
  then
    echo "Running $test"
    "./$test"
  fi
done
