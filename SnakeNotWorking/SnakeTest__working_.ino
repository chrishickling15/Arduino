/*
Author: Chris Hickling
   This program is designed to be a simple snake game on an 
   Arduino Uno-powered 7x5 LED matrix. This version allows for someone
   to control the user snake with a Wii Nunchuk.
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
int userX[35];
int userY[35];

int timer;//gamespeed timer
int length;//snake length
String dir;//direction of snake
boolean snake;//snake is either true or false. alive or dead

void setup(){  //start of setup
  ///set up a random number generator
  randomSeed(analogRead(0));
  for(int i=0;i<12;i++){
    pinMode(pin[i],OUTPUT);//Set all Pins as output
  }
  nunchuck_setpowerpins();
  nunchuck_init();
}  //end of setup

void loop(){//main loop
  snake = true;
  length = 1;
  userX[0] = 3;
  userY[0] = 1;
  dir = "up";
  timer = 100;
  
  //loop while snake has not hit sides or itself
  while(snake = true){  
    //clear all leds
    clearLeds();
    //display appropriate LEDs
    dotDisplay();
    //snake movement
    snakeMove();
    //check to see if the snake is still alive
    snakeAlive();    
    //snake autogrow mode
    length++;    
  }//end while
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
    //interate based on the length of the snake so that it will light up the whole snake
    for (int j=0;j<length;j++){
      //clear LEDs
      clearLeds();
      //display user position
      digitalWrite(row[userY[j]], HIGH);
      digitalWrite(col[userX[j]], LOW);
      //clear
      clearLeds();
    }//end inner for
  }//end outer for
}//end dotDisplay

//this method will read data from the wiichuk for user control
void userControl(){ 
  nunchuck_get_data(); //Read data from numchuck
  xjoy = nunchuck_joyx();    // X axis of Joystick (Between aprox 35-220)
  yjoy = nunchuck_joyy();    // Y axis of Joystick (Between aprox 35-220)
  //if statements to control snake direction
  if (xjoy > 140){
     dir="right";
  }//end if
  if (xjoy < 110){
     dir="left";
  }//end if 
  if (yjoy > 140){
     dir="up";
  }//end if
  if (yjoy < 110){
     dir="down";
  }//end if
}//end userControl

//this function will light up all LEDS and pause for about 5 seconds
void allLeds(){
  for(int i=1;i<8;i++){
    digitalWrite(row[i], HIGH);
  }//end for
  for(int i=1;i<6;i++){
    digitalWrite(col[i], LOW);
  }//end for
  delay(5000);
}//end allLeds

//this function will move the snake in a certain direction
void snakeMove(){  
  //for every unit of length, pass value down the array
  for (int i = 0;i<length;i++){
    userX[i+1] = userX[i];
    userY[i+1] = userY[i];
  }//end for  
  //change position of snake head based on desired direction
  if (dir="up"){
    userY[0]++;
  }//end if  
  if (dir="down"){
    userY[0]--;
  }//end if
  if (dir="right"){
    userX[0]++;
  }//end if  
  if (dir="left"){
    userX[0]--;
  }//end if
}//end snakeMove 


//this function will check to see if the snake has hit a wall or itself
void snakeAlive(){
  //check if snake hit the wall
  if (userX[0] < 1 || userX[0] > 5 || userY[0] < 1 || userY[0] > 7){
    snake = false;
  }//end if
  //for loop to check to see if snake hit itself. each iteration will check the head against 
  //a different part of the snake's body
  for (int i=0;i<length;i++){    
    if ((userX[0] == userX[i]) && (userY[0] == userY[i])){
      snake = false;
    }//end if
  }//end for
}//end of snakeAlive
