#!/bin/env bash

function report_passed() {
    filtered_pass=$(echo $1 | sed 's/:/_/g')
    echo "The test \"$1\" has been marked as a 'known fail', but passed!" \
         >> "known-fail-passed-$filtered_pass-comment.txt"
}

known_fails=$(ctest -N -L known-fail | awk '/Test #/ { print $3 }')

for known_fail in $known_fails
do
    ctest -R $known_fail && report_passed $known_fail
done
# TODO:
# A) read the above files in a github action that posts a comment? or
# B) post a comment directly using curl
