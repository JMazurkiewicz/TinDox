#!/usr/bin/env bash

failure() {
    echo $1 >&2
    exit 1
}

mkdir /tmp/tds_user_command_test
cd /tmp/tds_user_command_test
tds init .

cat <<EOT
sample_user
password
password
EOT | tds user add
if [ $? -ne 0 ]
then
    failure "Could not add valid user"
fi

cat <<EOT
s##mple_user2
password
password
EOT | tds user add
if [ $? -eq 0 ]
then
    failure "Added user with invalid username"
fi

cat <<EOT
newpass
newpass
EOT | tds user password admin
if [ $? -ne 0 ]
then
    failure "Password change failed"
fi

cat <<EOT
newpass
newpass
EOT | tds user password nosuchuser
if [ $? -ne 0 ]
then
    failure "Password change succeeded for non-existing user"
fi
