"""
  Raspberry pi LED controller
  Authors: Seyed Sobhan Hosseini - Kian Khatibi
  Date: Thursday, August 22, 2024, 10:52:48 PM
"""

import serial
import time
import RPi.GPIO as GPIO

global portIsOpen 

def setup_pins(led_pins) :
	GPIO.setmode(GPIO.BCM)
	for pin in led_pins :
		GPIO.setup(pin, GPIO.OUT)
		GPIO.output(pin, GPIO.LOW)
		
def decimal_to_binary(n) :
	if 0 <= n <= 255 :
		binary_value = format(n, '08b')
		return [int(bit) for bit in binary_value]
	else :
		raise ValueError("Number out of range")
		
def display_binary(led_pins, binary_digits) :
	for pin, bit in zip(led_pins, binary_digits) :
		GPIO.output(pin, GPIO.HIGH if bit else GPIO.LOW)
	
led_pins = [2, 3, 4, 5, 6, 7, 8, 9]

setup_pins(led_pins)		

while True :
	global Port
	Port = serial.Serial()
	Port.baudrate = 115200
	Port.bytesize = serial.EIGHTBITS
	Port.parity = serial.PARITY_NONE
	Port.stopbits = serial.STOPBITS_ONE
	Port.timeout = 0.008
	Port.port = "/dev/ttyS0"
	
	try:
		print("trying")
		Port.open()
		print("connected")
		portIsOpen = True
		break
	except:
		portIsOpen = False
		time.sleep(1)
		
while(portIsOpen) :
	Port.flush()
	input_data = Port.readline()
	if len(input_data) > 0 :
		decimal_data = ord(input_data)
		print(decimal_data)
		try :
			binary_digits = decimal_to_binary(decimal_data)
			display_binary(led_pins, binary_digits)
		except :
			GPIO.cleanup()
			
	
