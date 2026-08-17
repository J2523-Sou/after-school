import time
import math
import os
import sys
import socket #Wifi library
import pygame #Dualsense input library

def deadzone(x, y, dz):
    dist = math.sqrt(x*x + y*y)
    if dist < dz:
        return 0.0, 0.0
    return x, y
    
def stick(axis):
    i = round((axis + 1) * 127.5) #& 0xFF #0 to 255
    return i

#variable
DEAD = 0.05

#Raspberry Pi IP
SERVER_IP = os.environ.get("ROBOT_SERVER_IP", "172.21.6.20")  # MATSUE接続時

PORT = int(os.environ.get("ROBOT_SERVER_PORT", "5001"))
axis = [0, 0, 0, 0, 0, 0]


def connect_socket(host, port, retries=5, interval=1.0):
    for attempt in range(1, retries + 1):
        try:
            sock = socket.create_connection((host, port), timeout=3.0)
            print("Raspi Connected!!")
            sock.sendall(b"Hello Raspi!\n")
            return sock
        except OSError as exc:
            if attempt == retries:
                print(f"Could not connect to {host}:{port}: {exc}")
                return None
            print(f"Connection failed ({attempt}/{retries}), retrying...")
            time.sleep(interval)

#Wifi
print("connecting...")
s = connect_socket(SERVER_IP, PORT)
if s is None:
    sys.exit(1)

#Dualsense
pygame.init()
pygame.joystick.init()
if pygame.joystick.get_count() == 0:
    print("Where is Dualsense!?")
    exit()

joy = pygame.joystick.Joystick(0)
joy.init()


print("DualSense detected:", joy.get_name())

#  0: Cross   1: Circle  2: Square  3: Triangle  4: Create 5: Home
#  6: Option  7: Lsw     8: Rsw     9: L1       10: R1     11: Up
# 12: Down   13: Left   14: Right  15: Ult      16: Mic

#Main loop
while True:
    pygame.event.pump() #update
    data1 = 0b00000000
    data2 = 0b00000000
    data3 = 0b00000000
    
    #Stick
    #0:LstX 1:LstY 2:RstX 3:RstY 4:L2 5:R2
    for i in range(6):
        axis[i] = joy.get_axis(i)
    axis[0], axis[1] = deadzone(axis[0], axis[1], DEAD)
    axis[2], axis[3] = deadzone(axis[2], axis[3], DEAD)
    
    #Button
    #data1
    if joy.get_button(0):  data1 |= 0b00000001 #Cross
    if joy.get_button(1):  data1 |= 0b00000010 #Circle
    if joy.get_button(2):  data1 |= 0b00000100 #Square
    if joy.get_button(3):  data1 |= 0b00001000 #Triangle
    if joy.get_button(4):  data1 |= 0b00010000 #Creat   e
    if joy.get_button(5):  data1 |= 0b00100000 #Home
    if joy.get_button(6):  data1 |= 0b01000000 #Option
    if joy.get_button(7):  data1 |= 0b10000000 #Lst
    data1 &= 0xFF
    #data2
    if joy.get_button(8):  data2 |= 0b00000001 #Rst
    if joy.get_button(9):  data2 |= 0b00000010 #L1
    if joy.get_button(10): data2 |= 0b00000100 #R1
    if joy.get_button(11): data2 |= 0b00001000 #Up
    if joy.get_button(12): data2 |= 0b00010000 #Down
    if joy.get_button(13): data2 |= 0b00100000 #Left
    if joy.get_button(14): data2 |= 0b01000000 #Right
    if joy.get_button(15): data2 |= 0b10000000 #Ult
    data2 &= 0xFF
    #data3
    if joy.get_button(16): data3 |= 0b00000001 #Mute
    if axis[4] > -0.5:     data3 |= 0b00000010 #L2
    if axis[5] > -0.5:     data3 |= 0b00000100 #R2
    data3 &= 0xFF 

    data4 = stick(axis[0])
    data5 = stick(axis[1])
    data6 = stick(axis[2])
    data7 = stick(axis[3])
    
    
    print("Data1:", data1," Data2:", data2, " Data3:", data3, " Data4:", data4, " Data5:", data5, " Data6:", data6, " Data7:", data7)
    packet = bytes([0xAA, data1, data2, data3, data4, data5, data6, data7])
    s.sendall(packet)
    

    time.sleep(0.02)
    
s.close()