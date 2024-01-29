#!/usr/bin/expect -f

set password "qwe"

spawn ssh amazomic@192.168.1.7 mkdir -p /usr/local/bin
expect "password:"
send "$password\r"
expect eof

spawn scp src/SimpleBash/cat/s21_cat src/SimpleBash/grep/s21_grep amazomic@192.168.1.7:/usr/local/bin
expect "password:"
send "$password\r"
expect eof

