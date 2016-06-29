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
import commandtransfer2

start_time = time.time()


def startup():
    commandtransfer2.reset()



""" Startup """
#startup()

while 1:
      x=0
      while x < 50:
        commandtransfer2.travel(180,100,0)
        time.sleep(0.1)
        x +=1
      while x < 100:
        commandtransfer2.travel(0,100,0)
        time.sleep(0.1)
        x +=1
      while x < 150:
        commandtransfer2.travel(0,0,60)
        time.sleep(0.1)
        x +=1
      while x < 200:
        commandtransfer2.travel(0,0,-60)
        time.sleep(0.1)
        x +=1
      while x < 50:
        commandtransfer2.translate(100,0,0)
        time.sleep(0.1)
        x +=1
      while x < 100:
        commandtransfer2.translate(0,100,0)
        time.sleep(0.1)
        x +=1
      while x < 100:
        commandtransfer2.translate(0,0,30)
        time.sleep(0.1)
        x +=1
      while x < 150:
        commandtransfer2.rotate(100,0,0)
        time.sleep(0.1)
        x +=1
      while x < 200:
        commandtransfer2.rotate(0,100,0)
        time.sleep(0.1)
        x +=1
      while x < 250:
        commandtransfer2.rotate(0,0,30)
        time.sleep(0.1)
        x +=1