#!/usr/bin/env sh
tds version
if [ $? -ne 0 ]
then
    echo "Test $1 failed" >&2
    exit 1
fi
