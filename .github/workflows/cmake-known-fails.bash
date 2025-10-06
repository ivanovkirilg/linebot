#!/bin/env bash

build_type="$1"

function report_passed() {
    filtered_pass=$(echo $1 | sed 's/:/_/g')
    echo "The test \"\`$1\`\" has been marked as a 'known fail', but passed" \
         "on the \`$build_type\` build!" \
         >> "known-fail-passed-$build_type-$filtered_pass-comment.txt"
}

known_fails=$(ctest -N -L known-fail | awk '/Test #/ { print $3 }')

for known_fail in $known_fails
do
    ctest -R $known_fail && report_passed $known_fail
done
