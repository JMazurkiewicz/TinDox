#!/usr/bin/env sh

cd /tmp
mkdir tds_request_tests_auth
cd tds_request_tests_auth

tds init
printf "Login1\nPasswd1\nPasswd1\n" | tds user add >/dev/null
printf "Login2\nPasswd2\nPasswd2\n" | tds user add >/dev/null

tds run --debug >server_log 2>&1 &
TDS_PID=$!
echo "TDS_PID = $TDS_PID"

echo "Testing 'admin' user"
printf "auth\nlogin:admin\npasswd:admin\n\nexit\n\n"    | netcat localhost 37666
echo "Testing 'Login1' user"
printf "auth\nlogin:Login1\npasswd:Passwd1\n\nexit\n\n" | netcat localhost 37666
echo "Testing 'Login2' user"
printf "auth\nlogin:Login2\npasswd:Passwd2\n\nexit\n\n" | netcat localhost 37666
echo "Testing bad user"
printf "auth\nlogin:bad\npasswd:bad\n\nexit\n\n"        | netcat localhost 37666

kill -INT `pgrep tds`
wait $TDS_PID
TDS_STATUS=$?
if [ $TDS_STATUS -ne 0 ]
then
    echo "TinDox server failed"
    exit 1
fi

echo "--------------------"
echo "Server log:"
cat server_log

cd -
rm -rf /tmp/tds_request_tests_auth
