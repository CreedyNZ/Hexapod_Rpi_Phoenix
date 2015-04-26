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

#serin = serial("/dev/ttyUSB0", 38400, False, False)
serout = serial("/dev/ttyAMA0", 38400, False, False)
start_time = time.time()

class sendpacket:

     def __init__(self,pktkey,commandtypes):
          self.movecount = 0
          self.pktkey = pktkey
          self.commandtypes = commandtypes

     def sendpkt(self):
          #create packets for sending to the Arbotix_M
          checksum = 0
          serout.write(\xff)
          serout.write(self.pktkey)
          for k in self.commandtypes:   
             serout.write(atrib[k])
             checksum += int(atrib[k])
          checksum = (255 - (chksum%256))
          serout.write(checksum)
          print(checksum)
          movecount += 1
          move_time = time.time() 
             
     
stdpkt  = sendpacket('\xff',config.stdatrib)  #create standard packet

def reset():
     for k in atrib:
       atrib[k] = 0
     atrib['i.BodyYOffset'] = 35
     fullpkt.createpkt
     


def travel(angle,rotate): #calculate travel related commands
     atrib['i_ComMode'] = 1;  
     if(Balance) atrib['i_ComMode'] += 16;
     if(DOUBLE_T) atrib['i_ComMode'] += 64;
     if(DOUBLE_H) atrib['i_ComMode'] += 128;
     anglerad =  math.radians(angle) 
     atrib['i_leftV'] = int(math.cos (anglerad) * 100) + 128
     atrib['i_leftH'] = int(math.sin (anglerad) * 100) + 128
     atrib['i_RightV'] = rotate + 128
     atrib['i_RightH']= 0 + 128
     atrib['buttons']= 0
     atrib['ext']= 0
     print atrib['i_ComMode']
     stdpkt.createpkt()
    
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
       

def commit(pkt): #input is the array of commands to be sent.
    print pkt
    for i in range (len(pkt)): 
     ser.write chr(pkt[i]) #Write packet to serial
     print pkt[i]
    return -1

def readStatus():
    number = bus.read_byte(address)
    return number
    
                       