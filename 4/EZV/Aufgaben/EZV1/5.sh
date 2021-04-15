#activate GPIO 21
echo 21 > /sys/class/gpio/export

#set pin as outputs
echo out > /sys/class/gpio/gpio21/direction

while true
do
    echo 1 > /sys/class/gpio/gpio21/value
    echo 0 > /sys/class/gpio/gpio21/value
done