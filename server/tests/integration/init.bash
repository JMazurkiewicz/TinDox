#!/usr/bin/env bash

failure() {
    echo "Test $1 failed" >&2
    exit 1
}

tds init not/valid/path
if [ $? == 0 ]
then
    failure "Command \`tds init not/valid/path\` should fail - not/valid/path is not valid path"
fi

tds init /
if [ $? == 0 ]
then
    failure "Command \`tds init /\` should fail - not enough permissions"
fi

mkdir /tmp/tds_init_integration_test
tds init /tmp/tds_init_integration_test
if [ $? -ne 0 ]
then
    failure "Command \`tds init /tmp/tds_init_integration_test\` should succeed - path is valid"
fi

cd /tmp
mkdir tds_init_integration_test2
cd tds_init_integration_test2
tds init
if [ $? -ne 0 ]
then
    failure "Command \`tds init\` should succeed - current path is valid"
fi
