#!/usr/bin/expect -f
set timeout -1

spawn ./mouseHelper.sh

expect "georgios@georgios-Lenovo-H50-50:~/Dokumente/g600$"
send -- "0\r"

expect eof
