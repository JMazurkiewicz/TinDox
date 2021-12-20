#!/usr/bin/env bash

start_test() {
    echo "-------------------------------------------"
}

failure() {
    echo $1 >&2
    exit 1
}

start_test
tds run
if [ $? -eq 0 ]
then
    failure "Command \`tds run\` should fail - no \`.tds\` directory"
fi
start_test

tds run --path
if [ $? -eq 0 ]
then
    failure "Command \`tds run --path\` should fail - '--path' switch requires valid path"
fi

start_test
tds run --path /
if [ $? -eq 0 ]
then
    failure "Command \`tds run --path\` should fail - / is not valid path"
fi

start_test
tds run --port
if [ $? -eq 0 ]
then
    failure "Command \`tds run --port\` should fail - '--port' switch requires valid port"
fi

start_test
tds run --port 5000000
if [ $? -eq 0 ]
then
    failure "Command \`tds run --port 5000000\` should fail - 5000000 is not valid port"
fi

start_test
tds run --port 30000
if [ $? -eq 0 ]
then
    failure "Command \`tds run --port 30000\` should fail - current directory is not valid instance"
fi
