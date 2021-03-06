#include "LedControl.h"




// https://labs.bsoares.com/ping-pong-com-matriz-de-leds-8x8-ee13cce39007 to continue the code
// change x and y because it's 90° wrong





// going right is x coordinates and going down is y coordinates

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);
unsigned long delaytime=500;


const int leftJoy= 4; //left joystick
const int rightJoy= 5;
int leftracket = 2;   //corresponding to the racket on the screen
int rightracket = 2;
int data = 500;
int data2 = 500;
int scoreleft =0;   //the score of the left side
int scoreright = 0;
int x = 1;

int xball;
int yball;
int angle = 180;
int racketheight=2;
int racketwidth=1;
int balldiameter=1;
byte smile01[] = {
    B00111100,
    B01000010,
    B10010101,
    B10100001,
    B10100001,
    B10010101,
    B01000010,
    B00111100
  };
byte smile02[] = {
    B00111100,
    B01000010,
    B10010101,
    B10010001,
    B10010001,
    B10010101,
    B01000010,
    B00111100
  };
byte smile03[] = {
    B00111100,
    B01000010,
    B10100101,
    B10010001,
    B10010001,
    B10100101,
    B01000010,
    B00111100
  };

byte zero[]={      // Used to display a '0' for when displaying the score
     B00000000,
     B00000000,
     B00111100,
     B01000010,
     B01000010,
     B01000010,
     B00111100,
     B00000000  
   };

  
 byte one[]={       // Used to display a '1' for when displaying the score
  B00000000,
  B00000000,
  B10001000,
  B10000100,
  B11111110,
  B10000000,
  B10000000,
  B00000000
 };
 byte two[]={        // Used to display a '2' for when displaying the score
  B00000000,
  B01100010,
  B01010001,
  B01010001,
  B01001001,
  B01000110,
  B00000000,
  B00000000
};

byte three[]={      // Used to display a '3' for when displaying the score
  B00000000,
  B00100110,
  B01000001,
  B01001001,
  B00110110,
  B00000000,
  B00000000,
  B00000000
};

byte four[]={        // Used to diplay a '4' for when displaying the score
  B00000000,
  B00000000,
  B00001111,
  B00001000,
  B00001000,
  B11111111,
  B00000000,
  B00000000
};
byte five[]={        // Used to diplay a '5' for when displaying the score
  B00000000,
  B00100111,
  B00100101,
  B00011001,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

void setSprite(byte *sprite){     //used todisplay all the rows correctly when calling score
    for(int r = 0; r < 8; r++){
        lc.setRow(0, r, sprite[r]);
    }
}

void startingGame(){    //counter before a game starts with a sound

  
  lc.clearDisplay(0);
  setSprite(three);
  delay(700);
  lc.clearDisplay(0);
  setSprite(two);
  delay(700);
  lc.clearDisplay(0);
  setSprite(one);
  delay(700);
  lc.clearDisplay(0);
  setSprite(zero);
  delay(700);
  lc.clearDisplay(0);
  tone(2,660,100);
  delay(150);
  tone(2,660,100);
  delay(300);
  tone(2,510,100);
  delay(100);
  tone(2,660,100);
  delay(300);
  tone(2,770,100);
  delay(550);
  noTone(2);

}
void ballPosition(){      //moving the ball in x,y
  lc.setLed(0,xball,yball,true);
  delay(175);
  lc.setLed(0,xball,yball,false);
}
void ballStarting(){    //random placement of the ball (4 dots at the center of the matrix)
  yball = rand() % 2+3;
  xball = rand() % 2+3;
  lc.setLed(0,xball,yball,true);
  delay(175);
  //lc.setLed(0,xball,yball,false);
  
  
}

void calcAngleIncrement(){    //it checks the angle of the ball and in function of it it orders to move the ball
  
  if (angle==0 || angle == 360|| angle==-360){
     xball+=1;
  }
  else if (angle == 45|| angle==-315)  {
     xball += 1;
     yball += 1;
  }
  else if (angle == 135|| angle==-225)  {
     xball -= 1;
     yball += 1;
  }
  else if (angle == 180|| angle==-180)  {
     xball -= 1;
  }
  else if (angle == 225|| angle==-135)  {
     xball -= 1;
     yball -= 1;
  }
  else if (angle == 315|| angle==-45)  {
     xball += 1;
     yball -= 1;
  }
  else if (angle == 90|| angle ==-270)  {
     yball += 1;
  }
  else if (angle == 270|| angle == -90)  {
     yball -= 1;
  }
}
void wallCollisons(){   // creating a simple hitbox for vertical walls and for horizontal walls it will add a point to the ennemy if the ball didn't touch the racket first
  
  if(yball==0||yball==7){   //vertical walls
    angle = -1*angle;
  }
  else if(xball<0||xball>7){    //horizontal walls
    tone(2,600,150);
    delay(100);
    tone(2,200,300);
    delay(100);
    noTone(2);
    leftracket = 2;
    rightracket = 2;

    
    if (yball<=0||yball>=7){
      angle = 180;
    }
    if (xball<0){     //rightside wins
      angle = 0;
      scoreright++;
      switch (scoreright){
        case 0:
          setSprite(zero);
          break;
        case 1:
          setSprite(one);
          break;
        case 2:
          setSprite(two);
          break;
        case 3:
          setSprite(three);
          break;
        case 4:
          setSprite(four);
          break;
        case 5:
          setSprite(five);
          break;
      }
      delay(1000);
      lc.clearDisplay(0);
      ballStarting();
      Rackleft();
      Rackright();
    }
    else if (xball>7){      //leftside wins
      angle = 180;
      scoreleft++;
      switch (scoreleft){
        case 0:
          setSprite(zero);
          break;
        case 1:
          setSprite(one);
          break;
        case 2:
          setSprite(two);
          break;
        case 3:
          setSprite(three);
          break;
        case 4:
          setSprite(four);
          break;
        case 5:
          setSprite(five);
          break;
      }
      
      delay(1000);
      lc.clearDisplay(0);
      ballStarting();
      Rackleft();
      Rackright();
    }
    if (scoreleft ==5 || scoreright ==5){   //it means a win
      delay(300);
      lc.clearDisplay(0);
      for (int i=0 ; i<5;i++){
        setSprite(smile01);
        delay(20);
        setSprite(smile02);
        delay(20);
        setSprite(smile03);
        delay(20);
        setSprite(smile02);
        delay(20);
      }
      gameOver();
      delay(100);
      setup();
      scoreleft =0;
      scoreright = 0;
    }
    delay(1000);
  }
  if (-360>=angle){     //used to keep the angle between 360 and -360
      angle = 360+angle;
    }
  else if(angle>=360){
    angle = angle-360;
  }  
  
}
void Rackleft(){    //moving the left racket with a joystick
  data = analogRead(leftJoy);
  if (data>=600){     //analogRead is between 0 and 1022 stg like that, and 511 is the middle
    if(7>leftracket+1){
      lc.setLed(0,0,leftracket++,false);// turning off the previous led and then turning on the led further
      for (int i =0; i<racketheight;i++){
        lc.setLed(0,0,leftracket+i,true);
        
        }
        
    }
    
  }
  else if (data<= 400){   // doing the same but when moving the joystick the other way
    if(0<leftracket){
      lc.setLed(0,0,1+leftracket--,false);
      
      for (int i =0; i<racketheight;i++){
        
        lc.setLed(0,0,leftracket+i,true);
        
        
        }
        
      }

  }
  else{     //when we still haven't touche the joystick, it will be position at "leftracket pos"
    for (int i =0; i<racketheight;i++){
      lc.setLed(0,0,leftracket+i,true);
    }
    
  }


}
void Rackright(){     //same but for the right racket
  data2 = analogRead(rightJoy);
  if (data2>=600){
    if(7>rightracket+1){
      lc.setLed(0,7,rightracket++,false);
      for (int i =0; i<racketheight;i++){
        lc.setLed(0,7,rightracket+i,true);
        
        }
        delay(2);
    }
    
  }
  else if (data2<= 400){
    if(0<rightracket){
      lc.setLed(0,7,1+rightracket--,false);
      
      for (int i =0; i<racketheight;i++){
        
        lc.setLed(0,7,rightracket+i,true);
        
        
        }
        delay(2);
      }

  }
  else{
    for (int i =0; i<racketheight;i++){
      lc.setLed(0,7,rightracket+i,true);
    }
    delay(2);
  }
 
  
}
void racketCollisions(){ //redirecting the ball when it touches a racket
  if (xball ==1){   //collision for leftracket
    if (yball == leftracket+2 && angle == 225 || yball == leftracket+2 && angle == -135 ){    //top left corner
      angle = 45;
    }
    else if (yball == leftracket+1 ){   //collisions for top left
      if ( angle == 225 || angle == -135){
        angle = 0;
      }
      else{
        angle = 45;
      }
    }
    else if(yball == leftracket ){    //bottom
      if(angle==135||angle==-225){
        angle = 0;
      }
      else{
        angle=-45;
      }
    }
    else if (yball == leftracket-1 && angle == 135 || yball == leftracket-1 && angle == -225){ //bottom corner
      angle = -45;
    }
  }
  else if (xball==6){   //collsion for rightracket
    if (yball == rightracket+2 && angle == 315 || yball == rightracket+2 && angle == -45 ){    //top left corner
      angle = 225;
    }
    else if (yball == rightracket+1 ){   //top right
      if ( angle == 315 || angle == -45){
        angle = 180;
      }
      else{
        angle = 225;
      }
    }
    else if(yball == rightracket ){   //bottom right
      if(angle==45||angle==-315){
        angle = 180;
      }
      else{
        angle=135;
      }
    }
    else if (yball == rightracket-1 && angle == 45 || yball == rightracket-1 && angle == -315){
      angle = 135;    //bottom corner
    }
  }
  
 
  
}
void gameOver(){    // sound when a player loses 5 times
  tone(2,523,200);
  delay(1000);
  tone(2,523,200);
  delay(1000);
  tone(2,523,200);
  delay(1000);
  tone(2,659,700);
  delay(700);
  tone(2,784,500);
  delay(500);
   tone(2,523,200);
  delay(1000);
   tone(2,523,200);
  delay(1000);
  tone(2,659,700);
  delay(700);
  tone(2,784,500);
  delay(800);
  tone(2,784,400);
  delay(400);
  tone(2,884,200);
  delay(200);
  tone(2,784,200);
  delay(200);
  tone(2,687,200);
  delay(200);
  tone(2,659,200);
  delay(200);
  tone(2,579,200);
  delay(200);
  tone(2,519,400);
  delay(400);
  
  noTone(2);
}
void setup() {
  
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,4);
  startingGame();
  pinMode(leftJoy,INPUT);
  ballStarting();
  
  
}



void loop()
{
  
  racketCollisions();
  wallCollisons();
  ballPosition();
  calcAngleIncrement(); 
  Rackleft();
  Rackright();
  
}
