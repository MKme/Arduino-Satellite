/* 
Erics DIY Satellite Project
My Youtube Channel  : http://www.youtube.com/user/Shadow5549
Website http://mkme.org

V1 Set up basic text tranmission
-Tuned speaker freq and speed

V2 Added watchdog function

V3- Added concept to hear the sound of space!  
Take sensor input such as radiation or light level and map to tone output
Currently Mapping Analog pin 0

V4 Toggle TX mode (CW vs Tone) with millis
Interval is used for length of time to send tone
Satellite now transmits full CW downlink then tone for duration of "Interval"

V5- Code cleanup.  Broke out into separate voids

V6- Commented out watchdog functions as NANO was getting stuck in endless watchdog loop
This is a confirmed bootloader problem here: http://forum.arduino.cc/index.php?topic=150419.0

********************************************
Notes for DEV:
Add code for external LED to indicate operational state
External LEDs could also flash an error code if system probelm
**********************************************

This code was adapted from:
Simple Arduino Morse Beacon
Written by Mark VandeWettering K6HX
Email: k6hx@arrl.net
http://brainwagon.org 
 
*/
int SPKpin = 3; //Needs to be PWM pin for spk/radio output
int Batt1 = 7.4;// Initialize & Globalize Battery One Level
int TXmode=LOW;//Toggle TX mode between CW and Tone
int val;//used for sensor to tone mapping
long previousMillis = 0; //millis used for time before toggle TX mode
long interval = 5000; // interval (milliseconds) for transmit tone mode (transmit mode toggle)

struct t_mtab { char c, pat; } ;
struct t_mtab morsetab[] = {
  	{'.', 106},
	{',', 115},
	{'?', 76},
	{'/', 41},
	{'A', 6},
	{'B', 17},
	{'C', 21},
	{'D', 9},
	{'E', 2},
	{'F', 20},
	{'G', 11},
	{'H', 16},
	{'I', 4},
	{'J', 30},
	{'K', 13},
	{'L', 18},
	{'M', 7},
	{'N', 5},
	{'O', 15},
	{'P', 22},
	{'Q', 27},
	{'R', 10},
	{'S', 8},
	{'T', 3},
	{'U', 12},
	{'V', 24},
	{'W', 14},
	{'X', 25},
	{'Y', 29},
	{'Z', 19},
	{'1', 62},
	{'2', 60},
	{'3', 56},
	{'4', 48},
	{'5', 32},
	{'6', 33},
	{'7', 35},
	{'8', 39},
	{'9', 47},
	{'0', 63}
} ;

#define N_MORSE  (sizeof(morsetab)/sizeof(morsetab[0]))
#define SPEED  (30)  //default 12 25 still stable 35 working
#define DOTLEN  (1200/SPEED)
#define DASHLEN  (3*(1200/SPEED))

void
dash()
{
  analogWrite(SPKpin, 128) ;//128 default
  delay(DASHLEN);
  analogWrite(SPKpin, 0) ;
  delay(DOTLEN) ;
}
void
dit()
{
  analogWrite(SPKpin, 128) ;//128 default
  delay(DOTLEN);
  analogWrite(SPKpin, 0) ;
  delay(DOTLEN);
}
void
send(char c)
{
  int i ;
  if (c == ' ') {
    Serial.print(c) ;
    delay(7*DOTLEN) ;
    return ;
  }
  for (i=0; i<N_MORSE; i++) {
    if (morsetab[i].c == c) {
      unsigned char p = morsetab[i].pat ;
      Serial.print(morsetab[i].c) ;

      while (p != 1) {
          if (p & 1)
            dash() ;
          else
            dit() ;
          p = p / 2 ;
      }
      delay(2*DOTLEN) ;
      return ;
    }
  }
  /* if we drop off the end, then we send a space */
  Serial.print("?") ;
}
void
sendmsg(char *str)
{
  while (*str)
    send(*str++) ;
  Serial.println("");
}
#include <avr/wdt.h>

void setup() {
  //wdt_disable();
 // wdt_enable (WDTO_8S);//watchdog
  pinMode(SPKpin, OUTPUT) ;
  Serial.begin(9600) ; //serial unnecessary except debugging as needed
  Serial.println("Eric's Arduino Satellite CW Downlink") ;
  Serial.println("by Eric William") ;
  Serial.println("www.mkmer.org") ;
  //Serial.println("") ;
  }

void loop() {
  //wdt_reset ();//Reset Watchdog
  //checksensors(); check sensor readings here
  decidemode();//decide mode of transmit: CW vs Tone depednding on interval
  //modeindicator(); add mode indicator LED etc here
  initiateTX();//Initiate the decided form of TX and proceeds to approprite TX void
  }

void decidemode(){
  unsigned long currentMillis = millis();//millis counter for length of time in tone transmit mode
  if(currentMillis - previousMillis > interval) {
   // save the last time toggled state
   previousMillis = currentMillis;
   // toggle TX state
   if (TXmode == LOW)
   TXmode = HIGH;
   else
   TXmode = LOW;
   }
  }
void initiateTX(){
  if (TXmode ==HIGH) TransmitMorse();//Send CW
  else TransmitTone(); //Tone transmit mode based on sensor reading
  }

void TransmitMorse(){//Make sure these are all CAPS or will not TX
   sendmsg("?????") ; //Preamble to begin receiving
   wdt_reset ();//Reset Watchdog
   sendmsg("AURORA1 SAT/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("OS,OK/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("SAFEMODE,NO/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("WATCHDOG,4/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("ITEMP,0C/ ") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("LUX,22/ ") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("TELEMETRY,OK/ ") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("ANTENNA,DEP/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("PV,520MV/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("BATT1,620MV/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("BUSA,4MA/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("RSSI,4DBM/") ;
   wdt_reset ();//Reset Watchdog
   sendmsg("WWW.MKME.ORG...") ; //All this code needs to be replaced with the INT to CHAR values from monitoring/systems
   //sendmsg (Batt1);
   delay(3000);  
   }
void Batt(){
  //input battery check/monitoring code here
  }
void Watchdog(){
  //input Watchdog code here
  }
void Solar(){
  //input Solar Cell monitoring & code here
  }
  
void TransmitTone(){//Transmit tone based on sensor value- so people can hear space!
  val=analogRead(0);//Sensor to use for tone
  val = map(val, 0, 1024, 100, 200); //(First two are input limits from the sensor- second 2 are the pwm out
  analogWrite(SPKpin, val) ;//Send PWM to create tone
  }  
