#!/usr/bin/expect -f
set timeout -1

spawn ./gitHelper.sh

expect "Username for 'https://github.com': "
send -- "Georgios24816\r"
expect "Password for 'https://Georgios24816@github.com':"
send -- "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\r"

expect eof
