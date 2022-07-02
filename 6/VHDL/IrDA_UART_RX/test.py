import serial
import time

ser = serial.Serial("/dev/ttyUSB1");

def isButtonPressed_blocking2(dev):
    arr = []
    sum = dev.in_waiting

    if (dev.in_waiting == 0):
        return False;

    else:
        while (dev.in_waiting > 0):
            sum += dev.in_waiting
            arr.append(dev.read(dev.in_waiting))
            #time.sleep(0.01)

        if (len(arr[0]) > 0):
            print(arr[0])

        return sum;
        
        
def isIdle(dev):
    if (dev.in_waiting == 0):
        time.sleep(0.1)
        if (dev.in_waiting == 0):
            return True;

    return False
        

while True:
    state = isButtonPressed_blocking2(ser)

    if (state):
        print("PRESSED : ", state)


ser.close();
