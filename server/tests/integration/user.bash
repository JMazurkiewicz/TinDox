#!/usr/bin/env bash

failure() {
    echo $1 >&2
    exit 1
}

mkdir /tmp/tds_user_command_test
cd /tmp/tds_user_command_test
tds init .

printf "SampleUser\npassword\npassword\n" | tds user add >/dev/null 2>/dev/null
if [ $? -ne 0 ]
then
    failure "Could not add valid user"
fi

printf "sample_user\npassword\npassword\n" | tds user add >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Added user with invalid username"
fi

printf "SampleUser\npassword\npassword\n" | tds user add >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Repeated username"
fi

printf "user1\npassword\nwrong\nwrong\nwrong\nwrong\nwrong\n" | tds user add >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Too many password attempts"
fi

printf "unreachable\nunreachable\nunreachable\n" | tds user add invalid_argument >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Command 'user add' takes no extra arguments"
fi

printf "admin\nnewpass\nnewpass\n" | tds user passwd admin #>/dev/null 2>/dev/null
if [ $? -ne 0 ]
then
    failure "Password change failed"
fi

printf "newpass\nnewpass\n" | tds user passwd nosuchuser >/dev/null 2>/dev/null
if [ $? -eq 0 ]
then
    failure "Password change succeeded for non-existing user"
fi

# Todo: perms tests

tds user remove nosuchuser >/dev/null 2>/dev/null
if [ $? -ne 0 ] # SHOULD FAIL (-ne)
then
    failure "Removed non-existing user"
fi

tds user remove SampleUser >/dev/null 2>/dev/null
if [ $? -ne 0 ]
then
    failure "Failed to remove 'SampleUser' user"
fi
