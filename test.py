import os
import time

def pressEnter():
    os.system("xdotool key KP_Enter")

def writeString(s):
    os.system('xdotool type "' + s + '"')

def writeStringEnter(s):
    writeString(s)
    pressEnter()

writeStringEnter("sudo su")
time.sleep(1)
writeStringEnter("0")
time.sleep(1)
writeStringEnter("cd Dokumente/g600")
time.sleep(0.01)
writeStringEnter("./mouse mouseConfig.txt &")
time.sleep(0.01)
writeStringEnter("exit")
time.sleep(0.01)
writeStringEnter("exit")
