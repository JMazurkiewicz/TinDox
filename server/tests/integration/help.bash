#!/usr/bin/env bash

failure() {
    echo "Test $1 failed" >&2
    exit 1
}

tds >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds\` should exit with error"
fi

tds help >/dev/null 2>/dev/null
if [ $? -ne 0 ]
then
    failure "Command \`tds help\` should exit without error"
fi

tds help argument >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds help\` should exit with error if any extra arguments are passed"
fi
