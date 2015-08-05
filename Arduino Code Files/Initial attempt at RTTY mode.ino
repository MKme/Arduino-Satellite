// code adapted from www.timzaman.com
/* 
My Youtube Channel  : http://www.youtube.com/user/Shadow5549
Erics DIY Satellite Project
V1
 
*/
#include <RTTY.h>

int batt1 = 7.4;// Initialize & Globalize Battery One Level
char buffer [50];

void setup(){
Serial.begin(9600);
RTTY.attach(2,3);  
}

void loop()
{
  Batt();
  SendData();
}

void SendData(){
  RTTY.tx("Aurora-1 Satellite!");
  RTTY.tx("OS Status:OK! ");
  RTTY.tx("IntTemp-9C ");
  RTTY.tx("Telemetry-OK ");
  RTTY.tx("Downlink:Live ");
  RTTY.tx("Reboot Count-4 ");
  RTTY.tx("PanelV-5.2 ");
  RTTY.tx("Batt1=!");
  dtostrf(batt1, 2, 2, buffer);
  RTTY.tx(buffer);
  Serial.println (buffer);
  RTTY.tx("c\n");
}
void Batt(){
  //input battery check code here
}

