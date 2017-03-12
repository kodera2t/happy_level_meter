/// Happy amplifier firmware
/// input:A0 and A1
/// Dec. 2016 kodera2t
/// waveform display mode added
/// display mode of the last time is stored in EEPROM version

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define OLED_RESET 7
Adafruit_SSD1306 display(OLED_RESET);

int mode,modeflag,oldxval,oldyval;
unsigned long ct;
  int xpos,ypos;   
  int cn1,averagenum;
  unsigned long int offset_L=0, offset_R=0;
  
void setup()   {

  attachInterrupt(0,mode_setting,CHANGE);
  pinMode(2, INPUT);             
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done
  ////////////////////////////////////
//  display.clearDisplay();
  display.setRotation(0);
  display.clearDisplay();
  display.setTextSize(1);
////////draw axis and tics///////
//  display.drawCircle(16,16,16,WHITE);
//  for(int j=30;j<180;j=j+30){
//    for(int i=13;i<16;i++){
//    xpos=16-i*cos(j*3.1415/180);
//    ypos=16-i*sin(j*3.1415/180);
//    display.drawPixel(xpos,ypos,WHITE);
//    }
//  }
//    display.drawCircle(78,16,16,WHITE);
//  for(int j=30;j<180;j=j+30){
//    for(int i=13;i<16;i++){
//    xpos=78-i*cos(j*3.1415/180);
//    ypos=16-i*sin(j*3.1415/180);
//    display.drawPixel(xpos,ypos,WHITE);
//    }
//  }
////////////////////////////////
//  display.display();
//  mode=0;
  mode=EEPROM.read(0);
  modeflag=1;
//  averagenum=100;
//  for(int i=0;i<100;i++){
//  offset_L=offset_L+(analogRead(A0)-512);
//  offset_R=offset_R+(analogRead(A1)-512);
//  delay(10);
//  }
//  offset_L=offset_L/100;
//  offset_R=offset_R/100;

}


void loop() {
  float readvalue1,readvalue2;
  double left,right;
  float voltage;
  left=0;
  right=0;
  for(int i=0;i<averagenum;i++){
  if(mode !=3){
  readvalue1=(abs((analogRead(A0)-512))*200-200);
  readvalue2=(abs((analogRead(A1)-512))*200-200);
  }else if(mode ==4){
    ;
  }else{
  readvalue1=((analogRead(A0)-512));
  readvalue2=((analogRead(A1)-512));    
  }
  voltage=3.3*readvalue1/1024.0;
  left=left+readvalue1;
  right=right+readvalue2;
  }
  readvalue1=left/averagenum;
  readvalue2=right/averagenum;
  switch(mode){
  case 0:
  averagenum=50;
  //drawaxis();
  drawneedle(readvalue1,readvalue2);
  eraseneedle(readvalue1,readvalue2);
  break;
  case 1:
  averagenum=50;
  drawaxis();
  drawneedle(readvalue1,readvalue2);
    eraseneedle(readvalue1,readvalue2);
  showvalue(readvalue1,readvalue2);  
  break;
  case 2:
  averagenum=50;
  showbar(readvalue1,readvalue2);
  break;  
  case 3:
  averagenum=1;
  plotgrh(readvalue1, readvalue2);
  break;
//  case 4:
//    if(modeflag==1){
//    display.clearDisplay();
//    display.display();
//    cn1=0;
//    modeflag=0;
//  }
//  break;
  }
}

void drawaxis(){
  int xpos,ypos;
  if(modeflag==1){
      //testscrolltext();
  // Clear the buffer.
  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(32,32);
  display.setTextSize(1);
//////draw axis and tics///////
  display.drawCircle(32,32,32,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=30;i<32;i++){
    xpos=32-i*cos(j*3.1415/180);
    ypos=32-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
    display.drawCircle(96,32,32,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=30;i<32;i++){
    xpos=96-i*cos(j*3.1415/180);
    ypos=32-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
////////////////////////////////
  display.display();

    modeflag=0;
  }

  
}

void drawneedle(int i,int j){
  int xpos2,ypos2;

  xpos2=32-25*cos(i/1024.0*3.1415);
  ypos2=32-25*sin(i/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,32,32,WHITE);
  xpos2=96-25*cos(j/1024.0*3.1415);
  ypos2=32-25*sin(j/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,96,32,WHITE);
  display.display();  
}

void eraseneedle(int i,int j){
  int xpos3,ypos3;

  xpos3=32-25*cos(i/1024.0*3.1415);
  ypos3=32-25*sin(i/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,32,32,BLACK);
  xpos3=96-25*cos(j/1024.0*3.1415);
  ypos3=32-25*sin(j/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,96,32,BLACK);
//  display.display();  
}

void showvalue(int i,int j){
    float voltage=abs(3.3*i/1024.0);
      display.fillRect(52,0,24,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(52,0);
    display.print(voltage); 
    voltage=abs(3.3*j/1024.0);
    display.fillRect(52,8,24,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(52,8);
    display.print(voltage);     
    //display.print("V");
}

void showbar(int i,int j){
  i=abs(i);
  j=abs(j);
  if(modeflag==1){
    display.clearDisplay();
    modeflag=0;
  }
int xval,yval;
  xval=i*96/512;
  yval=j*96/512;
 display.fillRect(0,0,oldxval,16,BLACK);
 display.fillRect(0,16,oldyval,16,BLACK);
 // display.clearDisplay();
  display.fillRect(0,0,xval,16,WHITE);
  display.fillRect(0,16,yval,16,WHITE);
  display.display();
  oldxval=xval;
  oldyval=yval;
  
}



void mode_setting(){
  int sw,k,pt;
  ct=millis();
  sw=digitalRead(2);
if(sw==LOW && (ct-pt)>100){
  modeflag=1;
  mode=mode+1;
}
  if(mode>3){
    mode=0;
  }
  EEPROM.write(0,mode);
}

void plotgrh(int i, int j){

  if(modeflag==1){
    display.clearDisplay();
    display.drawLine(64,0,64,32,WHITE);
    display.display();
    cn1=0;
    
    modeflag=0;
  }
  cn1=cn1+1;
  display.drawPixel(cn1,16+i*2,WHITE);
  display.drawPixel(cn1+64,16+j*2,WHITE);

  if(cn1==62){
    display.display();
    cn1=0;
    display.fillRect(0,0,128,32,BLACK);
  }
  
}







