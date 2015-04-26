//====================================================================
//Project Lynxmotion Phoenix
//Description: Phoenix, control file.
//The control input subroutine for the phoenix software is placed in this file.
//Can be used with V2.0 and above
//Configuration version: V1.0
//Date: 25-10-2009
//Programmer: Jeroen Janssen (aka Xan)
//             Kurt Eckhardt (aka KurtE) - converted to c ported to Arduino...
//
//Updated: Andrew Creahan to allow serial input from Rpi
//
//Hardware setup: Arbotix Commander version - Try to emulate most of PS2, but PS2 has 16 buttons and Commander 
// has 10. so some things may not be there, others may be doubled up.
// 
//NEW IN V1.0
//- First Release
//
//Walk method 1:
//- Left StickWalk/Strafe
//- Right StickRotate
//
//Walk method 2:
//- Left StickDisable
//- Right StickWalk/Rotate
//
//
// Quick and Dirty description of controls... WIP
// In most cases I try to mention what button on the PS2 things coorespond to..
// On/OFF - Turning the commander 2 on and off (PS2 start button)
// R1 - options (Change walk gait, Change Leg in Single Leg, Change GP sequence) (Select on PS2)
// R2 - Toggle walk method...  Run Sequence in GP mode
// R3 - Walk method (Not done yet) - (PS2 R3)
// L4 - Ballance mode on and off
// L5 - Stand/Sit (Triangle on PS2)
// L6+Right Joy UP/DOWN - Body up/down - (PS2 Dpad Up/Down)
// L6+Right Joy Left/Right - Speed higher/lower - (PS2 DPad left/right)
// Right Top(S7) - Cycle through options of Normal walk/Double Height/Double Travel) - (PS2 R1, R2)
// Left Top(S8) - Cycle through modes (Walk, Translate, Rotate, Single Leg) (PS2: Circle, X, L1, L2)

// Note: Left some descriptions of PS2 stuff, especially parts still left to Map/Implement.
//
//[Walk Controls]
//- selectSwitch gaits
//- Left Stick(Walk mode 1) Walk/Strafe
// (Walk mode 2) Disable
//- Right Stick(Walk mode 1) Rotate, 
//(Walk mode 2) Walk/Rotate
//- R1Toggle Double gait travel speed
//- R2Toggle Double gait travel length
//
//[Shift Controls]
//- Left StickShift body X/Z
//- Right StickShift body Y and rotate body Y
//
//[Rotate Controls]
//- Left StickRotate body X/Z
//- Right StickRotate body Y
//
//[Single leg Controls] - Need to check...
//- selectSwitch legs
//- Left StickMove Leg X/Z (relative)
//- Right StickMove Leg Y (absolute)
//- R2Hold/release leg position
//
//[GP Player Controls] - How to do sequences???
//- selectSwitch Sequences
//- R2Start Sequence
//
//====================================================================
// [Include files]
#if ARDUINO>99
#include <Arduino.h> // Arduino 1.0
#else
#include <Wprogram.h> // Arduino 0022
#endif
//[CONSTANTS]
#ifdef OPT_GPPLAYER
enum {
  WALKMODE=0, TRANSLATEMODE, ROTATEMODE, SINGLELEGMODE, GPPLAYERMODE, MODECNT};
#else
enum {
  WALKMODE=0, TRANSLATEMODE, ROTATEMODE, SINGLELEGMODE, MODECNT};
#endif
enum {
  NORM_NORM=0, NORM_LONG, HIGH_NORM, HIGH_LONG};


#define cTravelDeadZone 4      //The deadzone for the analog input from the remote

#define ARBOTIX_TO  1000        // if we don't get a valid message in this number of mills turn off


//=============================================================================
// I have included a modified version of the commander object here as I wish to
// decouple the usage of the joysticks from the names as well as on other robots
// I may not be using Serial as the the serial port I am communicating with
// So I also removed the Southpaw code.
//=============================================================================
/*
  Commander.h - Library for interfacing with ArbotiX Commander
 Copyright (c) 2009-2012 Michael E. Ferguson.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* bitmasks for buttons array */
#define BUT_R1      0x01
#define BUT_R2      0x02
#define BUT_R3      0x04
#define BUT_L4      0x08
#define BUT_L5      0x10
#define BUT_L6      0x20
#define BUT_RT      0x40
#define BUT_LT      0x80

/* bitmasks for Mode */
#define WALK        0x01 //Walk
#define ROTATE      0x02 //Rotate
#define TRANSLATE   0x04 //Translate
#define SINLEG      0x08 //Single leg
#define BALANCE     0x10  //Balance Mode
#define SIT         0x20   //Sit_Stand
#define DOUBLE_T    0x40  //Double Travel
#define DOUBLE_H    0x80  //Double Height
/* the Commander will send out a frame at about 30hz, this class helps decipher the output. */
class Commander
{    
public:
  Commander(); 
  void begin(unsigned long baud);
  int ReadMsgs();         // must be called regularly to clean out Serial buffer

  // joystick values are -125 to 125
  signed char RightV;      // vertical stick movement = forward speed
  signed char RightH;      // horizontal stick movement = sideways or angular speed
  signed char leftV;      // vertical stick movement = tilt    
  signed char leftH;      // horizontal stick movement = pan (when we run out of pan, turn body?)
  unsigned char cmdmode;  // Mode value

  // buttons are 0 or 1 (PRESSED), and bitmapped
  unsigned char buttons;  // 
  unsigned char ext;      // Extended function set
 
  
    // Hooks are used as callbacks for button presses -- NOT IMPLEMENT YET

private:
  // internal variables used for reading messages
  unsigned char vals[8];  // temporary values, moved after we confirm checksum
  int index;              // -1 = waiting for new packet
  int checksum;
};


//=============================================================================
// Global - Local to this file only...
//=============================================================================
Commander command = Commander();
unsigned long g_ulLastMsgTime;
short  g_sGPSMController;    // What GPSM value have we calculated. 0xff - Not used yet

#ifdef USEMULTI
//==============================================================================
//
// Lets define our Sub-class of the InputControllerClass
//
//==============================================================================
class CommanderInputController : 
public InputController
{
public:
  CommanderInputController();        // A reall simple constructor...

  virtual void     Init(void);
  virtual void     ControlInput(void);
  virtual void     AllowControllerInterrupts(boolean fAllow);

};


CommanderInputController g_CommanderController;


//==============================================================================
// Constructor. See if there is a simple way to have one or more Input
//     controllers. Maybe register at construction time
//==============================================================================
CommanderInputController::CommanderInputController()
{
  RegisterInputController(this);
}

#else
#define CommanderInputController InputController
// Define an instance of the Input Controller...
InputController  g_InputController;       // Our Input controller 
#endif



static short   g_BodyYOffset; 
static short   g_BodyYShift;
static byte    ControlMode;
static byte    HeightSpeedMode;
//static bool  DoubleHeightOn;
static bool    DoubleTravelOn;
static bool    WalkMethod;
byte           GPSeq;             //Number of the sequence

static byte    buttonsPrev;
static byte    extPrev;

// some external or forward function references.
extern void CommanderTurnRobotOff(void);

//==============================================================================
// This is The function that is called by the Main program to initialize
//the input controller, which in this case is the PS2 controller
//process any commands.
//==============================================================================

// If both PS2 and XBee are defined then we will become secondary to the xbee
void CommanderInputController::Init(void)
{
  g_BodyYOffset = 0;
  g_BodyYShift = 0;
  command.begin(38400);
  GPSeq = 0;  // init to something...

  ControlMode = WALKMODE;
  HeightSpeedMode = NORM_NORM;
  //    DoubleHeightOn = false;
  DoubleTravelOn = false;
  WalkMethod = false;
}

//==============================================================================
// This function is called by the main code to tell us when it is about to
// do a lot of bit-bang outputs and it would like us to minimize any interrupts
// that we do while it is active...
//==============================================================================
void CommanderInputController::AllowControllerInterrupts(boolean fAllow)
{
  // We don't need to do anything...
}

//==============================================================================
// This is The main code to input function to read inputs from the Commander and then
//process any commands.
//==============================================================================
void CommanderInputController::ControlInput(void)
{
  
  // See if we have a new command available...
  if(command.ReadMsgs() > 0){
    // If we receive a valid message than turn robot on...
    g_InControlState.fHexOn = true;

    // [SWITCH MODES]

    // Choose modes...
    
    switch (command.cmdmode) {
    case '10':    
      ControlMode = WALKMODE;
      break;
    case '20':    
      ControlMode = ROTATEMODE;
      break;
    case '30':    
      ControlMode = TRANSLATEMODE;
      break;
    default:
      ControlMode = WALKMODE;
      break;
      }
    
         
      if (ControlMode != SINGLELEGMODE)
        g_InControlState.SelectedLeg=255;
  
    //Serial.write(ControlMode);
    //Serial.write('\n');
    //[Common functions]
    //Switch Balance mode on/off 
    if (command.cmdmode & BALANCE) 
      g_InControlState.BalanceMode = 1;
    else   
      g_InControlState.BalanceMode = 0;
    
      
    //Stand up, sit down  
    if (command.cmdmode & SIT) 
        g_BodyYOffset = 0;
      else
        g_BodyYOffset = 35;

    // We will use L6 with the Right joystick to control both body offset as well as Speed...
    // We move each pass through this by a percentage of how far we are from center in each direction
    // We get feedback with height by seeing the robot move up and down.  For Speed, I put in sounds
    // which give an idea, but only for those whoes robot has a speaker
    //if (command.buttons & BUT_L6 ) {
    //  // raise or lower the robot on the joystick up /down
    //  // Maybe should have Min/Max
    //  g_BodyYOffset += command.leftV/25;
    //
    //  // Likewise for Speed control
    //  int dspeed = command.leftH / 16;   // 
    //  if ((dspeed < 0) && g_InControlState.SpeedControl) {
    //    if ((word)(-dspeed) <  g_InControlState.SpeedControl)
    //      g_InControlState.SpeedControl += dspeed;
    //    else 
    //      g_InControlState.SpeedControl = 0;
    //    MSound( 1, 50, 1000+g_InControlState.SpeedControl);  
    //  }
    //  if ((dspeed > 0) && (g_InControlState.SpeedControl < 2000)) {
    //    g_InControlState.SpeedControl += dspeed;
    //    if (g_InControlState.SpeedControl > 2000)
    //      g_InControlState.SpeedControl = 2000;
    //    MSound( 1, 50, 1000+g_InControlState.SpeedControl); 
    //  }
    //
    //  command.leftH = 0; // don't walk when adjusting the speed here...
    //}

    ////[Walk functions]
    //if (ControlMode == WALKMODE) {
    //  //Switch gates
    //  if (((command.buttons & BUT_R1) && !(buttonsPrev & BUT_R1))
    //    && abs(g_InControlState.TravelLength.x)<cTravelDeadZone //No movement
    //  && abs(g_InControlState.TravelLength.z)<cTravelDeadZone 
    //    && abs(g_InControlState.TravelLength.y*2)<cTravelDeadZone  ) {
    //    g_InControlState.GaitType = g_InControlState.GaitType+1;                    // Go to the next gait...
    //    if (g_InControlState.GaitType<NUM_GAITS) {                 // Make sure we did not exceed number of gaits...
    //      MSound( 1, 50, 2000);  
    //    } 
    //    else {
    //      MSound (2, 50, 2000, 50, 2250); 
    //      g_InControlState.GaitType = 0;
    //    }
    //    GaitSelect();
    //  }
    //
    //  //Double leg lift height
    //  if ((command.buttons & BUT_RT) && !(buttonsPrev & BUT_RT)) {
    //    MSound( 1, 50, 2000);  
    //    HeightSpeedMode = (HeightSpeedMode + 1) & 0x3; // wrap around mode
    //    DoubleTravelOn = HeightSpeedMode & 0x1;
    //    if ( HeightSpeedMode & 0x2)
    //      g_InControlState.LegLiftHeight = 80;
    //    else
    //      g_InControlState.LegLiftHeight = 50;
    //  }

      //Walking
    
        g_InControlState.TravelLength.x = -command.RightH;
        g_InControlState.TravelLength.z = -command.RightV;
     

      if (!DoubleTravelOn) {  //(Double travel length)
        g_InControlState.TravelLength.x = g_InControlState.TravelLength.x/2;
        g_InControlState.TravelLength.z = g_InControlState.TravelLength.z/2;
      }

      g_InControlState.TravelLength.y = -(command.leftH)/4; //Right Stick Left/Right 


    //[Translate functions]
    g_BodyYShift = 0;
    if (ControlMode == TRANSLATEMODE) {
      g_InControlState.BodyPos.x =  SmoothControl(((command.RightH)*2/3), g_InControlState.BodyPos.x, SmDiv);
      g_InControlState.BodyPos.z =  SmoothControl(((command.RightV)*2/3), g_InControlState.BodyPos.z, SmDiv);
      g_InControlState.BodyRot1.y = SmoothControl(((command.leftH)*2), g_InControlState.BodyRot1.y, SmDiv);

      //      g_InControlState.BodyPos.x = (command.RightH)/2;
      //      g_InControlState.BodyPos.z = -(command.RightV)/3;
      //      g_InControlState.BodyRot1.y = (command.leftH)*2;
      g_BodyYShift = (-(command.leftV)/2);
    }

    //[Rotate functions]
    if (ControlMode == ROTATEMODE) {
      g_InControlState.BodyRot1.x = (command.RightV);
      g_InControlState.BodyRot1.y = (command.leftH)*2;
      g_InControlState.BodyRot1.z = (command.RightH);
      g_BodyYShift = (-(command.leftV)/2);
    }

    //[Single leg functions]
    if (ControlMode == SINGLELEGMODE) {
      //Switch leg for single leg control
      if ((command.buttons & BUT_R1) && !(buttonsPrev & BUT_R1)) {
        MSound (1, 50, 2000);  
        if (g_InControlState.SelectedLeg<5)
          g_InControlState.SelectedLeg = g_InControlState.SelectedLeg+1;
        else
          g_InControlState.SelectedLeg=0;
      }

      g_InControlState.SLLeg.x= (byte)((int)command.RightH+128)/2; //Left Stick Right/Left
      g_InControlState.SLLeg.y= (byte)((int)command.leftV+128)/10; //Right Stick Up/Down
      g_InControlState.SLLeg.z = (byte)((int)command.RightV+128)/2; //Left Stick Up/Down

      // Hold single leg in place
      if ((command.buttons & BUT_RT) && !(buttonsPrev & BUT_RT)) {
        MSound (1, 50, 2000);  
        g_InControlState.fSLHold = !g_InControlState.fSLHold;
      }
    }


    //Calculate walking time delay
    g_InControlState.InputTimeDelay = 128 - max(max(abs(command.RightH), abs(command.RightV)), abs(command.leftH));

    //Calculate g_InControlState.BodyPos.y
    g_InControlState.BodyPos.y = max(g_BodyYOffset + g_BodyYShift,  0);

    // Save away the buttons state as to not process the same press twice.
    buttonsPrev = command.buttons;
    extPrev = command.ext;
    g_ulLastMsgTime = millis();
  } 
  else {
    // We did not receive a valid packet.  check for a timeout to see if we should turn robot off...
    if (g_InControlState.fHexOn) {
      if ((millis() - g_ulLastMsgTime) > ARBOTIX_TO)
        CommanderTurnRobotOff();
    }
  }
}

//==============================================================================
// CommanderTurnRobotOff - code used couple of places so save a little room...
//==============================================================================
void CommanderTurnRobotOff(void)
{
  //Turn off
  g_InControlState.BodyPos.x = 0;
  g_InControlState.BodyPos.y = 0;
  g_InControlState.BodyPos.z = 0;
  g_InControlState.BodyRot1.x = 0;
  g_InControlState.BodyRot1.y = 0;
  g_InControlState.BodyRot1.z = 0;
  g_InControlState.TravelLength.x = 0;
  g_InControlState.TravelLength.z = 0;
  g_InControlState.TravelLength.y = 0;
  g_BodyYOffset = 0;
  g_BodyYShift = 0;
  g_InControlState.SelectedLeg = 255;
  g_InControlState.fHexOn = 0;
}


//==============================================================================
// The below class code is based on the commander class by Michael Ferguson... 
// I included and updated for my own usage...  As I may not always use 
// Serial and I wish to decouple usage of joysticks from the names...
//==============================================================================

/*
  Commander.cpp - Library for interfacing with ArbotiX Commander
 Copyright (c) 2009-2012 Michael E. Ferguson.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

//==============================================================================
// Commander::Commander - Constructor
//==============================================================================
Commander::Commander(){
  index = -1;
}

//==============================================================================
// Commander::begin 
//==============================================================================
void Commander::begin(unsigned long baud){
  Serial.begin(38400);
}

//==============================================================================
// ReadMsgs
//==============================================================================

/* process messages coming from Commander 
 *  format = 0xFF RIGHT_H RIGHT_V LEFT_H LEFT_V BUTTONS EXT CHECKSUM */
int Commander::ReadMsgs(){
  while(Serial.available() > 0){
    digitalWrite(0, !digitalRead(0));
    if(index == -1){         // looking for new packet
      if(Serial.read() == 255){
        index = 0;
        checksum = 0;
      }
    }
    else{
      vals[index] = int((char) Serial.read());
      vals[index] = vals[index];
      checksum += (vals[index]);
      index++;
      digitalWrite(0, !digitalRead(0));
      if(index == 8){ // packet complete
        //Serial.write(checksum%256);
        if(checksum%256 != 255){
          // packet error!
          index = -1;
          return 0;
        }
        else{
          digitalWrite(0, !digitalRead(0));
          cmdmode = (signed char)( (int)vals[0] ); //  extra value to control command modes directly instead of scrolling through buttons
          leftV = (signed char)( (int)vals[1]-128 );
          leftH = (signed char)( (int)vals[2]-128 );
          RightV = (signed char)( (int)vals[3]-128 );
          RightH = (signed char)( (int)vals[4]-128 );
          buttons = vals[5];
          ext = vals[6];
        }
        index = -1;
        while (Serial.read() != -1)
          ;
        //XBeeSerial.flush();  ' Not the same on Arduino 1.0+
        return 1;
      }
    } 
  }
  return 0;
}
//==============================================================================
//==============================================================================

