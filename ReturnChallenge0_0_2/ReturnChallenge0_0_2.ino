/*
Author: Chris Hickling
   This program is designed to be a simple return challenge on an 
   Arduino Uno-powered 7x5 LED matrix. This version allows for someone
   to control the user dot with a Wii Nunchuk.
*/

//import libraries
#include <Wire.h>
#include <nunchuck_funcs.h>


int loop_cnt=0;

byte xjoy,yjoy;

//pin[xx] in program is pin nn on Arduino
int pin[13]={-1,1,2,3,4,5,6,7,8,9,10,11,12};
//set up which pin[] are columns and which are rows
int col[6]={0,pin[11],pin[8],pin[7],pin[3],pin[5]};
int row[8]={0,pin[9],pin[4],pin[10],pin[6],pin[1],pin[12],pin[2]};

//declaration of user and comp (x,y) coordinates
int userX;
int const userY = 1;//user will always be located on the baseline
int compX;
int compY;
int timer;

void setup(){  //start of setup
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
  compY = 7;
  userX=3;//initial position of user of the x axis
  timer=2000;//set initial speed of game
  
  //iterate as long as comp dot does not cross baseline
  while(compY > 0){
    //computer's starting/restarting position has the (x,y) coordinates (random,7)
    compX=random(5)+1;
    compY=7;
    //game will start with a hit from the computer
    compHit();
    //if ball passes baseline, then it has been returned
    if (compY > 0){
      userReturn();
    }//end if 
    //if speed is not at maximum speed, speed up game
    if (timer > 200){
      timer = timer - 20;
    }//end if 
  }//end do-while
  allLeds();
}//end main loop

void clearLeds(){//clear all leds
  //clear all rows
  for(int i=1;i<8;i++){
    digitalWrite(row[i], LOW);
  }//end if
  ///clear all columns
  for(int i=1;i<6;i++){
    digitalWrite(col[i], HIGH);
  }//end if
}//end function

//display user position and ball position
void dotDisplay(){
  //before displaying user and computer dots, check for user input
  userControl();
  //iterate as many times as variable timer more iterations means a slower
  //must clear LEDS in between u an c to avoid extra, non-playable LEDS
  for (int i=0;i<timer;i++){
    //display comp position
    digitalWrite(row[compY], HIGH);
    digitalWrite(col[compX], LOW);
    //clear LEDs    
    clearLeds();
    //check user joystick
    if (i == timer/2){
      userControl();
    }//end if
    //display user position
    digitalWrite(row[userY], HIGH);
    digitalWrite(col[userX], LOW);
    //clear
    clearLeds();
  }//end for
}//end dotDisplay

//this function will read data from the wiichuk for user control
void userControl(){
  nunchuck_get_data(); //Read data from numchuck
  xjoy = nunchuck_joyx();    // X axis of Joystick (Between aprox 35-220)
  yjoy = nunchuck_joyy();    // Y axis of Joystick (Between aprox 35-220)
  if (xjoy > 140 && userX < 5){
     userX++;
  }//end if
  if (xjoy < 110 && userX > 1){
     userX--;
  }//end if 

}//end userControl

//this function controls a computer hit until the ball is returned or 
//the ball passes the user
void compHit(){
  //decrement compY until the ball hits user or ball passes user
  while((compX != userX || compY != userY) && compY > 0){
    dotDisplay();
    compY--;
  }//end while  
}//end compHit

//this function controls the user's return shot
void userReturn(){
  //ball will move away from baseline until it gets to the other end
  while(compY < 8){
    dotDisplay();
    compY++;
  }//end while
}//end userReturn

//this function will light up all LEDS and pause for about 5 seconds
void allLeds(){
  for(int i=1;i<8;i++){
    digitalWrite(row[i], HIGH);
  }
  for(int i=1;i<6;i++){
    digitalWrite(col[i], LOW);
  }
  delay(5000);
}
