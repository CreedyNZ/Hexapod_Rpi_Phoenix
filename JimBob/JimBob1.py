#!/usr/bin/env python

""" 
  Core code for Hexapod         
  Copyright (c)  2015 Andrew Creahan.  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
"""

import sys, time, os
import serial     
import RPi.GPIO as GPIO
import SerialOut
import config

start_time = time.time()
delay = 0.1  # set rest time between command sends
GPIO.setmode(GPIO.BCM)

def startup():
    SerialOut.reset()

def reset():
  pin = 21
  GPIO.setup(pin, GPIO.OUT)
  GPIO.output(pin, GPIO.HIGH)
  time.sleep(0.12)
  GPIO.output(pin, GPIO.LOW)
  time.sleep(0.12)
  GPIO.output(pin, GPIO.HIGH)
#Travel = t then angle (0 - 360),speed (0 - 120),rotate (-100 - 100), repeat (0+)
#Rotate = r then left,right,up, repeat
#Translate = r then left,right,up, repeat

walktest = [('t',0,75,0,30),('t',180,75,0,30),('t',90,100,0,20),('t',270,100,0,20),('t',45,50,0,30),('t',135,50,0,30),('t',0,0,50,10),('t',0,0,-50,30)]

""" Startup """
#startup()

#Travel angle,speed,rotate
#Rotate left,right,up
#Translate left,right,up
"""
class Move:
    Class to create movements
    def __init__(self, movearray):
        self.movecount = 0
        self.movearray = movearray

    def moves(self):
          #create move commands for sending to the Arbotix_M
          for k in self.movearray:   
             movearray[k,1] = atrib[k] + 128
          checksum = 0
          self.ser.write(chr(255))
          for k in self.commandtypes:   
             self.ser.write(chr(atrib[k]))
             checksum += int(atrib[k])
          checksum = (255 - (checksum%256))
          print checksum
          self.ser.write(chr(checksum))

          response = serout.read()
          print (">>" ,response)
          self.movecount += 1
          print self.movecount
          print'*'
          print atrib['i_ComMode']

          move_time = time.time()
        
stdpkt  = Move(config.stdatrib, config.addatrib)  #create standard packet 
g=0"""
reset()
config.atrib['i_Gait'] = 0
SerialOut.wait(25)

while config.atrib['i_Gait'] < 5:
      x=0
      while x < 50:
        SerialOut.travel(180,100,0)
        time.sleep(0.1)
        x +=1
      while x < 100:
        SerialOut.travel(0,100,0)
        time.sleep(0.1)
        x +=1
      config.atrib['i_Gait']+=1
      SerialOut.wait(25)
      """while x < 75:
        SerialOut.travel(0,0,60)
        time.sleep(0.1)
        x +=1
      while x < 90:
        SerialOut.travel(0,0,-60)
        time.sleep(0.1)
        x +=1
      while x < 100:
        SerialOut.travel(45,100,0)
        time.sleep(0.1)
        x +=1
      while x < 125:
        SerialOut.travel(270,100,0)
        time.sleep(0.1)
        x +=1
      config.atrib['i_Gait'] = 0
      while x < 135:
        SerialOut.translate(0,-100,0)
        time.sleep(0.1)
        x +=1
       """ 
      while x < 155:
        SerialOut.translate(0,0,90)
        time.sleep(0.1)
        x +=1
      while x < 175:
        SerialOut.translate(0,0,-90)
        time.sleep(0.1)
        x +=1
        """
      while x < 195:
        SerialOut.stand()
        time.sleep(0.1)
        x +=1
      while x < 200:
        SerialOut.rotate(100,0,0)
        time.sleep(0.1)
        x +=1
      while x < 220:
        SerialOut.rotate(0,100,0)
        time.sleep(0.1)
        x +=1
      while x < 250:
        SerialOut.rotate(0,0,30)
        time.sleep(0.1)
        x +=1"""