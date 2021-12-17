#!/usr/bin/env bash

failure() {
    echo $1 >&2
    exit 1
}

tds run >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds run\` should fail - no \`.tds\` directory"
fi

tds run --path >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds run --path\` should fail - '--path' switch requires valid path"
fi

tds run --path / >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds run --path\` should fail - / is not valid path"
fi

tds run --port >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds run --port\` should fail - '--port' switch requires valid port"
fi

tds run --port 5000000 >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command \`tds run --port\` should fail - 5000000 is not valid port"
fi
