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



from SerialDriver import Driver
from auto import navigate

serin = Driver("/dev/ttyUSB0", 38400, False)
serout = Driver("/dev/ttyAMA0", 38400, False)
start_time = time.time()

def startup():
    serout.flush()
    moves = [[0,0,0,0,0,0,15],[0,0,99,0,0x20,0,400],[0,0,0,0,0,1,4],[0,0,0,0,0,2,4],[0,0,0,-10,0,0,9],[0,0,0,10,0,0,9],[0,0,0,0,0,1,5],[-5,0,0,0,0,0,30],[5,0,0,0,0,0,30],[0,0,0,0,0,5,5]]
    for cmd in moves:
        wait = cmd[6]
        for i in range(0, wait):
          rt_vrt = cmd[0]
          rt_hor = cmd[1]
          lt_vrt = cmd[2]
          lt_hor = cmd[3]
          buttn = cmd[4]
          auto = cmd[5]
          serout.sendPacket(rt_vrt,rt_hor,lt_vrt,lt_hor,buttn,auto)
          time.sleep(0.1)

def manual():
    cmd=serin.readPacket()
    if cmd != None:
        rt_vrt = cmd[0]
        rt_hor = cmd[1]
        lt_vrt = cmd[2]
        lt_hor = cmd[3]
        buttn = cmd[4]
        auto = 0
        serout.sendPacket(rt_vrt,rt_hor,lt_vrt,lt_hor,buttn,auto) 
    else:
        navigate()


""" Startup """

startup()

while serin != None:
    cmd=serin.readPacket()
    if cmd != None:
        manual()
    else:
        navigate()


 
