#!/usr/bin/env sh

cd /tmp
mkdir tds_request_tests_cd
cd tds_request_tests_cd
mkdir one
mkdir two
touch file.txt

tds init
tds run --debug >server_log 2>&1 &
TDS_PID=$!
echo "TDS_PID = $TDS_PID"

echo "Testing cd to 'one' and back"
printf "auth\nlogin:admin\npasswd:admin\n\ncd\npath:one\n\ncd\npath:..\n\nlogout\n\nexit\n\n"  | netcat localhost 37666
echo "Testing cd to 'two' and '.'"
printf "auth\nlogin:admin\npasswd:admin\n\ncd\npath:two\n\ncd\npath:'.'\n\nlogout\n\nexit\n\n" | netcat localhost 37666
echo "Testing cd to invalid directory"
printf "auth\nlogin:admin\npasswd:admin\n\ncd\npath:three\n\nlogout\n\nexit\n\n"               | netcat localhost 37666
echo "Testing cd to file"
printf "auth\nlogin:admin\npasswd:admin\n\ncd\npath:file.txt\n\nlogout\n\nexit\n\n"            | netcat localhost 37666

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
rm -rf /tmp/tds_request_tests_cd
