#!/usr/bin/expect -f
#copy to remote-pc
spawn scp -o "StrictHostKeyChecking no" cat/s21_cat grep/s21_grep cicd@192.168.1.2:~
expect -exact "\rcicd@192.168.1.2's password: "
send -- "123\r"
expect eof

# enter on remote-pc
spawn ssh -o "StrictHostKeyChecking no" cicd@192.168.1.2
expect -exact "\rcicd@192.168.1.2's password: "
send -- "123\r"
expect "\rRun 'do-release-upgrade' to upgrade to it."
send -- "./move.sh\r"
expect eof