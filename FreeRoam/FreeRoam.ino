/*
Author: Chris Hickling

This program is designed to test a free roaming dot
on a 7x5 LED matrix. It is controlled with a Wii Nunchuk.
*/

#include <Wire.h>
#include <nunchuck_funcs.h>


int loop_cnt=0;

byte accx,accy,accz,zbut,cbut,xjoy,yjoy;
//pin[xx] in program is pin nn on Arduino
int pin[13]={-1,1,2,3,4,5,6,7,8,9,10,11,12};
//set up which pin[] are columns and which are rows
int col[6]={0,pin[11],pin[8],pin[7],pin[3],pin[5]};
int row[8]={0,pin[9],pin[4],pin[10],pin[6],pin[1],pin[12],pin[2]};

void setup()
{  //start of setup
  ///set up a random number generator
  randomSeed(analogRead(0));
  for(int i=0;i<13;i++){
    pinMode(pin[i],OUTPUT);//Set all Pins as output
  }
    nunchuck_setpowerpins();
    nunchuck_init();
}  //end of setup

void loop(){//main loop
  //clear all leds
  clearLeds();
  int userX=3;
  int userY=1;
  int timer=1000;
  
  /*
  //button input mode
  int left=analogRead(1);
  int right=analogRead(5);
  */
  while (true){
    clearLeds();
    nunchuck_get_data(); //Read data from numchuck
    xjoy = nunchuck_joyx();    // X axis of Joystick (Between aprox 35-220)
    yjoy = nunchuck_joyy();    // Y axis of Joystick (Between aprox 35-220)
    if (xjoy > 140 && userX < 5){
       userX++;
    }
    if (xjoy < 110 && userX > 1){
       userX--;
    }  
    if(yjoy > 140 && userY < 7){
       userY++;
    } 
    if(yjoy < 110 && userY > 1){
      userY--;
    }
    dotDisplay(userX, userY, timer);
    
  }
}//end of loop

void clearLeds(){//clear all leds
  for(int i=1;i<8;i++){
    digitalWrite(row[i], LOW);
  }
  for(int i=1;i<6;i++){
    digitalWrite(col[i], HIGH);
  }
}//end function

void dotDisplay(int uX, int uY, int time){
  for (int i=0;i<time;i++){
    digitalWrite(row[uY], HIGH);
    digitalWrite(col[uX], LOW);
    clearLeds();
  }
}

