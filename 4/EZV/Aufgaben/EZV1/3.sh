#activate GPIO pins 20 and 21
echo 20 > /sys/class/gpio/export
echo 21 > /sys/class/gpio/export

#set both pins as outputs
echo out > /sys/class/gpio/gpio20/direction
echo out > /sys/class/gpio/gpio21/direction

#set both pins high
echo 1 > /sys/class/gpio/gpio20/value
echo 1 > /sys/class/gpio/gpio21/value

#print the pin states (high)
cat /sys/class/gpio/gpio20/value
cat /sys/class/gpio/gpio21/value