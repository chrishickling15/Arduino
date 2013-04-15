/*
    Author: Chris Hickling
    This program is designed as a simple dot avoid game.
    This version allows for a person to control the user
    with a Wii Nunchuk.
*/

//import libraries
#include <Wire.h>
#include <nunchuck_funcs.h>


int loop_cnt=0;

//declare variables for x and y axis of the joystick
byte xjoy,yjoy;
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
  //declare and initialize variables for starting positions and game speed
  int userX=3;
  int userY=1;
  int compX=1;
  int compY=7;
  int timer=1000;
  
  //loop while user and comp (x,y) coordinates are not the same
  while (compX != userX || compY != userY){
    clearLeds();//clear all LEDs
    nunchuck_get_data(); //Read data from numchuck
    xjoy = nunchuck_joyx();    // X axis of Joystick (Between aprox 35-220)
    yjoy = nunchuck_joyy();    // Y axis of Joystick (Between aprox 35-220)
    
    if (xjoy > 140 && userX < 5){//controls user's rightward movement
       userX++;
    }//end if
    if (xjoy < 110 && userX > 1){//controls user's leftward movement
       userX--;
    }//end if   
    dotDisplay(compX, compY, userX, userY, timer);//display user and comp
    compY--;//move computer closer to the baseline
    
    //if computer gets to the baseline, reset computer at the top of the
    //screen and increase the difficulty.
    if (compY<1){
      compY=7;
      compX=random(5)+1;
      //
      if (timer > 200){
        timer=timer-10;
      }//end inner if
    }//end outer if
  }//end while
  allLeds();
}//end of loop

//this function will clear all of the LEDs
void clearLeds(){
  for(int i=1;i<8;i++){
    digitalWrite(row[i], LOW);
  }//end for
  for(int i=1;i<6;i++){
    digitalWrite(col[i], HIGH);
  }//end for
}//end function

//this function displays user and computer positions
void dotDisplay(int cX, int cY, int uX, int uY, int time){
  //iterate more is time is greater, making a harder difficulty
  for (int i=0;i<time;i++){
    digitalWrite(row[cY], HIGH);
    digitalWrite(col[cX], LOW);
    clearLeds();//clear all LEDs
    digitalWrite(row[uY], HIGH);
    digitalWrite(col[uX], LOW);
    clearLeds();//clear all LEDs
  }//end for
}//end dotDisplay

//this function will light up all LEDs for 5 seconds or so
void allLeds(){
  for(int i=1;i<8;i++){
    digitalWrite(row[i], HIGH);
  }
  for(int i=1;i<6;i++){
    digitalWrite(col[i], LOW);
  }
  delay(5000);//pause for 5 seconds or so
}
