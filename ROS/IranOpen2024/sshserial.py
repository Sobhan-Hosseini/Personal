"""
  Serial Communication between Jetson nx and ESP32
  Authors: Kian Khatibi - Shahriar Karimi Makhsous - Seyed Sobhan Hosseini
  Date: Feb 29, 2024
"""
#!/usr/bin/env python3
import time
import rospy
from std_msgs.msg import Int32MultiArray,String
import serial
import math


angle = 0
distance = 0
LOR=0
sign_send = 19
signDistanceSend = -1
tl_class_send = 19
tl_distance_send = -1
sign_class = 19
sign_distance = -1
crossWalk_angle = 90
tl_class,tl_distance =19,-1
crossWalk_distance = -1
crossWalkFlag = "123456"
signQ=[19,19,19,19]
tlQ=[19,19,19,19]
crossFlag = "0"
whichLane = 0
port_isOpen = False

def fifo(x,q):
    if len(q)>0:
        for i in range(len(q)-1):
            q[len(q)-2-i+1]=q[len(q)-2-i]
        q[0]=x
    return q

             
def lineFilter(msg):
    if len(msg.data) <2:
        return False
    if msg.data[0] is None or msg.data[1] is None:
        return False
    if(msg.data[0] < -300 or msg.data[0] > 300):    #line distance(d)
        return False
    if(msg.data[1] < -60 or msg.data[1] > 60):    #line angle(p)
        return False    
    return True

def signFilter(msg):
    if(msg.data[1] < -1 or msg.data[1] > 999):   #sign distance
        return False
    if(msg.data[0] < 0 or msg.data[0] > 19):    #sign class(0 - 16)
        return False

    return True

def tlFilter(msg):
    if(msg.data[3] < -1 or msg.data[3] > 999):   #sign distance
        return False
    if(msg.data[2] < 0 or msg.data[2] > 19):    #sign class(0 - 16)
        return False

    return True


def crossWalkFilter(msg):
    if(msg.data[0] < -99 or msg.data[0] > 99):    #crossWalk angle
        return False
    if(msg.data[1] < 0 or msg.data[1] > 99999):   #crossWalk distance
        return False

    return True

def serial_write():
    global output_data
    # seriData=str("%4d\0" % distance)+str("%4d\0" % angle)+str("%6d\0" % l)+str("%1d\0" % 0)+str("%2d\0" % signNo)+str("%4d\0" % signD)+str("%1d\0" % lightS)+str("%4d"% crossNo)
    Port.flush()
    output_data =  str("k%4da" % distance) + str("%3da" % angle) + str("%2da" % sign_send) + str("%3da" % signDistanceSend)+str("%2da" % tl_class_send) + str("%3da" % tl_distance_send)+str("%3da" % crossWalk_distance)+str("%3da" % crossWalk_angle)+str("%1da" % whichLane)+str("%1d"%LOR) + 'k' 
    print("jetson serial data:",output_data)
    i = Port.write(bytes(output_data,'ASCII'))
    # print("bytes sent: " + str(i))

def line_callback(msg):
    global angle , distance, whichLane,LOR
    if(lineFilter(msg)):    #setup parameters
        angle = msg.data[1]
        distance = msg.data[0]
        whichLane = msg.data[2]
    LOR=rospy.get_param("LOR")
    serial_write()

    
def sign_callback(msg):
    global tl_class_send,tl_distance_send,signDistanceSend,sign_send,signQ,tlQ
    if(signFilter(msg)): 
        sign_class = msg.data[0]
        sign_distance= msg.data[1]
        sign_send=19
        signDistanceSend = -1
        if sign_class!=19:
            signQ=fifo(sign_class,signQ)
            if all(ele == signQ[0] for ele in signQ) and sign_distance<55:
                sign_send=sign_class
                signDistanceSend=sign_distance

    if (tlFilter(msg)):
        tl_class=msg.data[2]
        tl_distance = msg.data[3]
        tl_class_send = 19
        tl_distance_send = -1
        if tl_class!=19:
            tlQ=fifo(tl_class,tlQ)
            if all(elle==tlQ[0]for elle in tlQ) and tl_distance<100:
                tl_distance_send=tl_distance
                tl_class_send=tlQ[0]

def crossWalk_callback(msg):
    global crossWalk_angle , crossWalk_distance
    if(crossWalkFilter(msg)):
        crossWalk_angle = msg.data[0]
        crossWalk_distance = msg.data[1]/100

def serial_read():
    global crossFlag,pubLine
    # input_string = ""
    # Port.flush()
    try:
        input_data = Port.readline()
        input_string = str(input_data, encoding="ascii")
        if "114" in input_string:
            crossFlag = "1"
            pubLine.publish(crossFlag)
        elif "101" in input_string:
            crossFlag = "0"
            pubLine.publish(crossFlag)
    # try:
        
    
    except :
        print("-----------error--------")

    
        

def main(args=None):
    global pubLine

    print("------------------------serial started------------------------------")
    rospy.init_node("serial")
    rate = rospy.Rate(30)
    pubLine = rospy.Publisher("crosWalk_Flag", String, queue_size = 10)
    # pubCross = rospy.Publisher("crossWalk_Flag", String, queue_size=10)
    rospy.Subscriber( "linespd" , Int32MultiArray ,line_callback)
    rospy.Subscriber( "crossWalkData" , Int32MultiArray ,crossWalk_callback)
    rospy.Subscriber( "signnd" , Int32MultiArray ,sign_callback)
    while True:
        global Port
        Port=serial.Serial()
        Port.baudrate = 115200
        Port.bytesize = serial.EIGHTBITS
        Port.parity = serial.PARITY_NONE
        Port.stopbits = serial.STOPBITS_ONE
        Port.timeout = 0.008
        Port.port="/dev/ttyTHS0"
        try:
            print("trying to connect to THS0")
            Port.open()
            print("Serial is connected")
            port_isOpen = True
            break
        except:
            port_isOpen = False
            print('------------error in opening serial------------')
            time.sleep(1)

    while(port_isOpen) and not rospy.is_shutdown():
        serial_read()


if __name__ == '__main__':
    main()
    print('out of while')