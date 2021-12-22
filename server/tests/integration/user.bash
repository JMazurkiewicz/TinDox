#!/usr/bin/env bash

start_test() {
    echo "-------------------------------------------"
}

failure() {
    echo $1 >&2
    exit 1
}

mkdir /tmp/tds_user_command_test
cd /tmp/tds_user_command_test
tds init .

start_test
printf "SampleUser\npassword\npassword\n" | tds user add
if [ $? -ne 0 ]
then
    failure "Could not add valid user"
fi

start_test
printf "sample_user\npassword\npassword\n" | tds user add
if [ $? -eq 0 ]
then
    failure "Added user with invalid username"
fi

start_test
printf "SampleUser\npassword\npassword\n" | tds user add
if [ $? -eq 0 ]
then
    failure "Repeated username"
fi

start_test
printf "user1\npassword\nwrong\nwrong\nwrong\nwrong\nwrong\n" | tds user add
if [ $? -eq 0 ]
then
    failure "Too many password attempts"
fi

start_test
printf "unreachable\nunreachable\nunreachable\n" | tds user add invalid_argument
if [ $? -eq 0 ]
then
    failure "Command 'user add' takes no extra arguments"
fi

start_test
printf "admin\nnewpass\nnewpass\n" | tds user passwd admin
if [ $? -ne 0 ]
then
    failure "Password change failed"
fi

start_test
printf "newpass\nnewpass\n" | tds user passwd nosuchuser
if [ $? -eq 0 ]
then
    failure "Password change succeeded for non-existing user"
fi

start_test
tds user perms SampleUser +rw -u
if [ $? -ne 0 ]
then
    failure "Failed to change permissions of user"
fi

start_test
tds user perms SampleUser -rw +u
if [ $? -ne 0 ]
then
    failure "Failed to change permissions of user"
fi

start_test
tds user perms SampleUser +h
if [ $? -eq 0 ]
then
    failure "Succeeded to add invalid perms"
fi

start_test
tds user remove nosuchuser
if [ $? -eq 0 ]
then
    failure "Removed non-existing user"
fi

start_test
tds user remove SampleUser
if [ $? -ne 0 ]
then
    failure "Failed to remove 'SampleUser' user"
fi
