"""
  Getting Data from RPLIDAR
  Author: Matin Jamshidi
  Date: Spring 2024
"""
from rplidar import RPLidar
import cv2
import numpy as np
import math

image = np.zeros((2001, 3601, 3), dtype=np.uint8)


x_vals = []
y_vals = []
angle_list=[]
depth_list=[]
lidar = RPLidar('/dev/tty.usbserial-0001')  #port address
info = lidar.get_info()
print("info= " , info)
health = lidar.get_health()
print("healteh=" , health)

u=0

for scan in enumerate(lidar.iter_scans()):
    print("data" , scan[1])
    for val in scan[1]:
        # print("val = " , val)
        teta=0
        depth =0
        j=0
        for val2 in val:
            j+=1
            if j ==2:
                teta = round(val2)
            if j ==3:
                depth = val2

        if( teta in angle_list):
            depth_list[angle_list.index(teta)] = depth
        else:
            # if 100>teta>0 or 360>teta>260 :
            angle_list.append(teta)
            depth_list.append(depth)

        a=len(angle_list)
        b=0    
        x_vals=[]
        y_vals=[]      
        while(b<a):
             r_teta = math.radians(angle_list[b])
             sin_value = math.sin(r_teta)
             cos_value = math.cos(r_teta)
             x_vals.append(depth_list[b] * cos_value)
             y_vals.append(depth_list[b] * sin_value)
             b+=1



    if(u%10)==0:     
        image[:] = (0, 0, 0)
        counter=0
        counterval=len(x_vals)
        while(counter<counterval):
            cv2.circle(image , (int(x_vals[counter]/3)+1801 , int(y_vals[counter]/3)+1001)  , 0 , (255,255,255) , 5)
            counter+=1
        cv2.imshow("frame" , image)
        cv2.waitKey(1)

    u+=1
    if(u>800):
         break

print("xval=" , len(x_vals))
print("yval=" , len(y_vals))


print("end =========================")
lidar.stop()
lidar.disconnect()