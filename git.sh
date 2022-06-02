#!/usr/bin/expect -f
set timeout -1

spawn ./gitHelper.sh

expect "Username for 'https://github.com': "
send -- "Georgios24816\r"
expect "Password for 'https://Georgios24816@github.com':"
send -- "ghp_sXejLA7iIQWv1jv"
send -- "ytBciEKb5WpRB5y2iAyrO\r"


expect eof
