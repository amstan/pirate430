
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial

class _safeSerial(serial.Serial):
	"""Seems like whenever serial.Serial is interrupted while it's waiting:
	   File "/usr/lib/python3.3/site-packages/serial/serialposix.py", line 511, in write
	   _, ready, _ = select.select([], [self.fd], [], None)
	   Then next operation(a read or write) will freeze the device.
	   The workaround is to close the port and reopen it when it gets interrupted.
	   This could be a bug in the embedded code.
	"""
	
	def read(self,*args):
		try:
			return serial.Serial.read(self,*args)
		except:
			self.close()
			self.open()
			raise
	
	def write(self,*args):
		try:
			return serial.Serial.write(self,*args)
		except:
			self.close()
			self.open()
			raise

class SPI(object):
	def __init__(self,port="/dev/ttyACM0",baud=9600,*args):
		self.port=port
		self._serial=_safeSerial(self.port,baud,rtscts=True,*args)
		
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
			raise IOError("Echo test to the pirate430 failed. %r!=%r" % (tosend,reply))
	
	def set_ws2812(self,data):
		length=len(data)
		length=[length//256,length%256]
		self._serial.write(b'w'+bytes(length)+bytes(data))
	
	@property
	def green_led(self):
		return self._green_led
	@green_led.setter
	def green_led(self,value):
		value=bool(value)
		self._serial.write(bytes([ord('g'),value]))
		self._green_led=value
	
	@property
	def red_led(self):
		return self._red_led
	@red_led.setter
	def red_led(self,value):
		value=bool(value)
		self._serial.write(bytes([ord('r'),value]))
		self._red_led=value
	led=red_led
	
	@property
	def cs(self):
		return self._cs
	@cs.setter
	def cs(self,value=0):
		value=(~value & 0xff)
		self._serial.write(bytes([ord('c'),value]))
		self._cs=value
	release_cs=cs.fset
	
	def xfer(self,tosend,cs=(1<<0)):
		self.cs=cs
		
		self._serial.write(bytes([ord('d'),tosend]))
		reply=self._serial.read(1)[0]
		
		self.release_cs()
		
		return reply

class SPIDevice(object):
	def __init__(self,spi,cs=0):
		self.spi=spi
		self.cs=cs
		self.cs_mask=(1<<cs)
	
	def xfer(self,data):
		return self.spi.xfer(data,cs=self.cs_mask)
	
	def __repr__(self):
		return "SPIDevice(%r, cs=%r)" % (self.spi.port,self.cs)

if __name__=="__main__":
	s=SPI()
	
	def test_ws2812(color=[255,255,255],count=30*4):
		data=[255]*(count*3)
		for i in range(count):
			data[(i*3)+1]=color[0]
			data[(i*3)]=color[1]
			data[(i*3)+2]=color[2]
		s.set_ws2812(data)
	
	def test_random():
		import random
		while 1:
			x=random.randint(0,255)
			y=s.xfer(x)
		print (x==y,x,y)