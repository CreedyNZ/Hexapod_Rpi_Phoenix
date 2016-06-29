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
import config
import math
atrib = config.atrib
Balance = 0
DOUBLE_T = 0
DOUBLE_H = 0

WALK = 1 
ROTATE = 2
TRANSLATE = 4
#define SINLEG      0x08 //Single leg
#define BALANCE     0x10  //Balance Mode
#define SIT         0x20   //Sit_Stand
#define DOUBLE_T    0x40  //Double Travel
#define DOUBLE_H    0x80  //Double Height

start_time = time.time()
movecount = 0
move_time = 0 

        
#serin = serial.Serial("/dev/ttyUSB0", 38400, False)
serout = serial.Serial("/dev/ttyAMA0", 38400, timeout=5)
serout.open()

class createpacket:

     def __init__(self,pktkey,commandtypes):
          self.movecount = 0
          self.pktkey = pktkey
          self.commandtypes = commandtypes

     def sendpkt(self):
          #create packets for sending to the Arbotix_M
          checksum = 0
          serout.write(chr(255))
          for k in self.commandtypes:   
             serout.write(chr(atrib[k]))
             checksum += int(atrib[k])
          checksum = (255 - (checksum%256))
          print checksum
          serout.write(chr(checksum))
          #response = serout.read()
          #print (response)
          self.movecount += 1
          print self.movecount
          atrib['i_ComMode']
          move_time = time.time()
             
     
stdpkt  = createpacket(255,config.stdatrib)  #create standard packet

def reset():
     serout.flush()
     


def travel(angle,speed,rotate): #calculate travel related commands
     atrib['i_ComMode'] = 1;  
     """if(Balance):
        atrib['i_ComMode'] += 21;
     if(DOUBLE_T):
        atrib['i_ComMode'] += 23;
     if(DOUBLE_H):
        atrib['i_ComMode'] += 25;"""
     anglerad =  math.radians(angle) 
     atrib['i_leftV'] = 128
     atrib['i_leftH'] = rotate + 128
     atrib['i_RightV'] = int(math.cos (anglerad) * speed) + 128
     atrib['i_RightH']= int(math.sin (anglerad) * speed) + 128
     atrib['buttons']= 0
     atrib['ext']= 0
     stdpkt.sendpkt()
     
def rotate(left,right,up): #calculate rotate related commands
     atrib['i_ComMode'] = 2;  
     """if(Balance):
        atrib['i_ComMode'] += 21;
     if(DOUBLE_T):
        atrib['i_ComMode'] += 23;
     if(DOUBLE_H):
        atrib['i_ComMode'] += 25; """
     atrib['i_leftV'] = up
     atrib['i_leftH'] = left
     atrib['i_RightV'] = 128
     atrib['i_RightH']= right
     atrib['buttons']= 0
     atrib['ext']= 0
     stdpkt.sendpkt()
     
def translate(left,right,up): #calculate translate related commands
     atrib['i_ComMode'] = 3;  
     """if(Balance):
        atrib['i_ComMode'] += 31;
     if(DOUBLE_T):
        atrib['i_ComMode'] += 33;
     if(DOUBLE_H):
        atrib['i_ComMode'] += 35;"""
     atrib['i_leftV'] = up
     atrib['i_leftH'] = left
     atrib['i_RightV'] = 128
     atrib['i_RightH']= right
     atrib['buttons']= 0
     atrib['ext']= 0
     stdpkt.sendpkt()
    
def move(cmd,angle,rotate,height,multiplier): #calculate movement related commands
    
    if cmd == 'translate':
       atrib['i_BodyPosx'] = int((math.cos (math.radians(angle))) * multiplier)
       atrib['i_BodyPosz'] = int((math.sin (math.radians(angle))) * multiplier)
       atrib['i_BodyRot1x'] += rotate
    if cmd == 'rotate':
       atrib['i_BodyPosx'] = int((math.cos (math.radians(angle))) * multiplier)
       atrib['i_BodyPosz'] = int((math.sin (math.radians(angle))) * multiplier)
       atrib['i_BodyRot1x'] += rotate
    movepkt.createpkt()
    
def other(gait,balance,leg,legheight): #calculate other commands
    
    atrib['i_SelectedLeg'] = 0
    atrib['i_LegLiftHeight'] = 0
    atrib['i_Flags'] = 0   
    othpkt.createpkt()
       



def readStatus():
    number = bus.read_byte(address)
    return number
    
                       