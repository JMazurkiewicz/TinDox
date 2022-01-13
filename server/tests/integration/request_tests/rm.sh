#!/usr/bin/env sh

cd /tmp
mkdir tds_request_tests_rm
cd tds_request_tests_rm
touch file.txt
mkdir empty_dir
mkdir not_empty_dir
cd not_empty_dir
touch makes_dir_not_empty
cd ..
mkdir lock_dir

tds init
printf "Blocker\nPasswd\nPasswd\n" | tds user add >/dev/null

tds run --debug >server_log 2>&1 &
TDS_PID=$!
echo "TDS_PID = $TDS_PID"

echo "Test removing regular file"
printf "auth\nlogin:admin\npasswd:admin\n\nrm\nname:'file.txt'\n\nexit\n\n" | netcat localhost 37666
echo "Test removing empty directory"
printf "auth\nlogin:admin\npasswd:admin\n\nrm\nname:empty_dir\n\nexit\n\n" | netcat localhost 37666
echo "Test removing non-empty directory"
printf "auth\nlogin:admin\npasswd:admin\n\nrm\nname:not_emptydir\n\nexit\n\n" | netcat localhost 37666

echo "Test removing occupied directory"
printf "auth\nlogin:Blocker\npasswd:Passwd\n\ncd\npath:lock_dir\n\n" | netcat localhost 37666 &
BLOCKER_PID=$!
printf "auth\nlogin:admin\npasswd:admin\n\nrm\nname:lock_dir\n\nexit\n\n" | netcat localhost 37666
kill $BLOCKER_PID
wait $BLOCKER_PID

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
rm -rf /tmp/tds_request_tests_rm
