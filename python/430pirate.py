#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial

class spi(object):
	def __init__(self,port="/dev/ttyACM0",baud=9600,*args):
		self.port=port
		self._serial=serial.Serial(self.port,baud,*args)
		
		self.sync()
		
		self.red_led=True
		self.test()
		
		self.green_led=True
		self.red_led=False
	
	def sync(self,count=10):
		self._serial.flush()
		self._serial.write(b"s"*count);
	
	def test(self,char='a'):
		tosend=char[0].encode("ascii")
		self._serial.write(b'e'+tosend)
		reply=self._serial.read(1)
		if tosend!=reply:
			raise IOError("Echo test to the 430pirate failed. %r!=%r" % (tosend,reply))
	
	@property
	def green_led(self):
		return self._green_led
	@green_led.setter
	def green_led(self,value):
		value=bool(value)
		self._green_led=value
		self._serial.write(bytes([ord('g'),value]))
	
	@property
	def red_led(self):
		return self._red_led
	@red_led.setter
	def red_led(self,value):
		value=bool(value)
		self._red_led=value
		self._serial.write(bytes([ord('r'),value]))
	led=red_led
	
	@property
	def cs(self):
		return self._cs
	@cs.setter
	def cs(self,value=0xff):
		self._serial.write(bytes([ord('c'),value]))
	release_cs=cs.fset
	
	def xfer(self,tosend,cs=1):
		self.cs=(~cs & 0xff)
		
		self._serial.write(bytes([ord('d'),tosend]))
		reply=self._serial.read(1)[0]
		
		self.cs=0xff
		
		return reply

if __name__=="__main__":
	s=spi()