#!/usr/bin/env sh

cd /tmp
mkdir tds_request_tests_rename
cd tds_request_tests_rename
touch file.txt
mkdir directory
mkdir non_empty
cd non_empty
touch file_in_non_empty
cd ..
mkdir lock_dir

tds init
printf "Blocker\nPasswd\nPasswd\n" | tds user add >/dev/null

tds run >server_log 2>&1 &
TDS_PID=$!
echo "TDS_PID = $TDS_PID"

echo "Test renaming regular file"
printf "auth\nlogin:admin\npasswd:admin\n\nrename\noname:'file.txt'\nnname:'file2.txt'\n\nrm\npath:file2.txt\n\nexit\n\n" | netcat localhost 37666
echo "Test renaming empty directory"
printf "auth\nlogin:admin\npasswd:admin\n\nrename\noname:directory\nnname:directory2\n\nrm\npath:directory2\n\nexit\n\n" | netcat localhost 37666
echo "Test renaming non-empty directory"
printf "auth\nlogin:admin\npasswd:admin\n\nrename\noname:non_empty\nnname:non_empty2\n\nrm\npath:non_empty2\n\nexit\n\n" | netcat localhost 37666

echo "Test renaming occupied directory"
printf "auth\nlogin:Blocker\npasswd:Passwd\n\ncd\npath:lock_dir\n\n" | netcat localhost 37666 &
BLOCKER_PID=$!
printf "auth\nlogin:admin\npasswd:admin\n\nrm\npath:lock_dir\n\nexit\n\n" | netcat localhost 37666
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
rm -rf /tmp/tds_request_tests_rename
