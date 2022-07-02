#activate GPIO pins 22 and 27
echo 22 > /sys/class/gpio/export
echo 27 > /sys/class/gpio/export

#set both pins as outputs
echo in > /sys/class/gpio/gpio22/direction
echo in > /sys/class/gpio/gpio27/direction

#invert the logic of both pins
echo 1 > /sys/class/gpio/gpio22/active_low
echo 1 > /sys/class/gpio/gpio27/active_low

#print the states
cat /sys/class/gpio/gpio22/value
cat /sys/class/gpio/gpio27/value

echo "press the buttons!"

#wait for user to press button
sleep 5

#print the states again
cat /sys/class/gpio/gpio22/value
cat /sys/class/gpio/gpio27/value