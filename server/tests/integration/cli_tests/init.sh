#!/usr/bin/env sh

start_test() {
    echo "-------------------------------------------"
}

failure() {
    echo "Test $1 failed" >&2
    exit 1
}

start_test
tds init not/valid/path
if [ $? == 0 ]
then
    failure "Command \`tds init not/valid/path\` should fail - not/valid/path is not valid path"
fi

start_test
tds init /
if [ $? == 0 ]
then
    failure "Command \`tds init /\` should fail - not enough permissions"
fi

start_test
mkdir /tmp/tds_init_integration_test
tds init /tmp/tds_init_integration_test
if [ $? -ne 0 ]
then
    failure "Command \`tds init /tmp/tds_init_integration_test\` should succeed - path is valid"
fi

start_test
cd /tmp
mkdir tds_init_integration_test2
cd tds_init_integration_test2
tds init
if [ $? -ne 0 ]
then
    failure "Command \`tds init\` should succeed - current path is valid"
fi

start_test
tds init
if [ $? -eq 0 ]
then
    failure "Command \`tds init\` should fail - config already exists"
fi
