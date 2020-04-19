#include "ArduinoBLE.h"
#include "GDXLib.h"

#define TWO_LINE_DISPLAY //comment out for no DISPLAY
//#define C_F_VERSION //C and F temperature
#define LEDS //

GDXLib GDX;
static char strUnits[16];
int t=0; //loop counter

void setup()
{
  //this version will not run faster than 6.6 readings a second using GDX-ACC
  // note the ao sample period for GDX-ACC=500ms
  // Initialize the debug serial port
  Serial.begin(9600);
  char strBuffer[64];//I changed to 64 
 #if defined LEDS
    //  Initialize the digital output lines
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    for (int i = 2; i < 10; i++) // cycle through the LEDs
      {
        digitalWrite(i, HIGH);
        delay (100);
        digitalWrite(i, LOW);
        delay (100);
      }          
  #endif //LEDS
  #if defined TWO_LINE_DISPLAY
    CharDisplayInit();
    delay (2000);
  #endif //TWO_LINE_DISPLAY
  
  #if defined C_F_VERSION
    Serial.print  ("special version  ");
    Serial.println("C and F temp only");
    CharDisplayPrintLine(1, "special version ");
    CharDisplayPrintLine(2, "C & F temp only ");
    delay (2000);
  #endif //C_F_VERSION
  
  Serial.println(" Looking for"); 
  #if defined TWO_LINE_DISPLAY
    CharDisplayPrintLine(1, "Looking for ");
  #endif //TWO_LINE_DISPLAY
  
  //char sensorName[64]="             ";
  // char sensorName[64]="GDX-ST 0P1000S9";
  // char sensorName[64]="GDX-FOR 072001P5"
  char sensorName[64]="GDX-ACC 0H101767";
       
  if (sensorName[5] == ' ') //if no specific sensor seleted
  {
    Serial.println("  any GDX sensor ");
    #if defined TWO_LINE_DISPLAY
      CharDisplayPrintLine(2, "any GDX sensor");
      delay(2000);
    #endif //TWO_LINE_DISPLAY
    GDX.GoDirectBLE_Begin();
  }
  else
  {
    Serial.println(sensorName);
    #if defined TWO_LINE_DISPLAY
      CharDisplayPrintLine(2, sensorName);
      delay(2000);
    #endif //TWO_LINE_DISPLAY
    GDX.GoDirectBLE_Begin(sensorName,1, 150);//specify channel and period here also
  }
  
  GDX.autoID();// this is the routine to get device info
  Serial.print("Found: ");
  Serial.println(GDX.deviceName());
  #if defined TWO_LINE_DISPLAY
    CharDisplayPrintLine(1, "Found ");
    CharDisplayPrintLine(2, GDX.deviceName());
    delay(1000);
  #endif //TWO_LINE_DISPLAY

  Serial.print("ChannelName: ");
  Serial.println(GDX.channelName());
  Serial.print("ChannelUnits: ");
  // Cache the unit string and try to remap special UTF8
  // characters to ones that we can display.
  sprintf(strUnits, "%s", GDX.channelUnits());
  ConvertUTF8ToASCII(strUnits);
  Serial.print("strUnits ");
  Serial.println(strUnits);
 
  Serial.println ("Data Table:");
}
 void loop()
{
  float channelReading =GDX.readSensor();
  char strBuffer[64];
  sprintf(strBuffer, "--- %s", strUnits);//?!!!!
  sprintf(strBuffer, "%.2f %s", channelReading,strUnits);
  //sprintf(strBuffer, "%.2f %s", channelReading,strUnits);//crash??!!!!!

  #if defined C_F_VERSION
      t++;
     float t2=t/2.0;//used to determine every other time through the loop
     if (t2==int(t/2))// every other time switch to F temperature
        {
          channelReading= channelReading*1.8+32;//convert C to F degrees  HACK
          //sprintf(strBuffer, "%.2f %s", channelReading,"deg F");// causes crash
          sprintf(strBuffer, "%.2f %s", channelReading,"deg F");   
        }
  #endif //C_F_VERSION
  Serial.println(GDX.channelName());
  Serial.println(strBuffer);
  #if defined TWO_LINE_DISPLAY
    CharDisplayPrintLine(1, GDX.channelName());
    CharDisplayPrintLine(2,strBuffer);
  #endif // TWO_LINE_DISPLAY
  delay(150);
   #if defined LEDS
    for (int i = 2; i < 10; i++) // turn off the LEDs
      {
        digitalWrite(i, LOW);
      }   
   float maxsignal =10;
   for (int dPin = 2; dPin<10;dPin++)
      {  //turn off all LEDs
         digitalWrite(dPin, LOW);
      }    
    if (channelReading >maxsignal*1/10 )//Check to see if above threshold 2
       {
        digitalWrite(2, HIGH);
       }
    if (channelReading >maxsignal*2/10 )//Check to see if above threshold 4
       {
        digitalWrite(3, HIGH);
       }
    if (channelReading >maxsignal*3/10 )//Check to see if above threshold 6
       {
        digitalWrite(4, HIGH);
       }
    if (channelReading >maxsignal*4/10 )//Check to see if above threshold 8
       {
        digitalWrite(5, HIGH);
       }
    if (channelReading >maxsignal*5/10  )//Check to see if above threshold 10
       {
        digitalWrite(6, HIGH);
       }
    if (channelReading >maxsignal*6/10  )//Check to see if above threshold 12
       {
        digitalWrite(7, HIGH);
       }
    if (channelReading >maxsignal*7/10  )//Check to see if above threshold 12
       {
        digitalWrite(8, HIGH);
       }
    if (channelReading >maxsignal*8/10  )//Check to see if above threshold 12
       {
        digitalWrite(9, HIGH);
       }
   /*comment the lines below and see if a crash happens: 
    if (channelReading >maxsignal*9/10  )//Check to see if above threshold 14
       {
         for (int jj = 2; jj<7;jj++) // flash LEDs for max force
           {
            digitalWrite(9, HIGH);
            delay (200);
         digitalWrite(9, LOW);
            delay (200);
           }

     delay(50);
       }
    */
  #endif //LEDS
}

void CharDisplayPrintLine(int line, const char* strText)
{
  uint8_t lineCode = 128;
  if (line == 2) lineCode = 192;

  // Force a field width of 16.
  // Left justified with space padding on the right.
  char strBuffer[32];
  sprintf(strBuffer, "%-16.16s",  strText);

  // For debug -- prints what goes to the display
  //Serial.print("[");
  //Serial.print(strBuffer);
  //Serial.println("]");
  
  Serial1.write((uint8_t)254); 
  Serial1.write(lineCode);  
  Serial1.write(strBuffer, 16);
}

void CharDisplayInit()
{
  Serial1.begin(9600);
  delay(500);
  
  // Clear the screen
  Serial1.write((uint8_t)254);
  Serial1.write((uint8_t)1);

  // Move cursor to beginning of line1
  Serial1.write(254); 
  Serial1.write(128);  
}

//=============================================================================
// ConvertUTF8ToASCII() Function
// Very limited -- only supports a few translations
//=============================================================================
void ConvertUTF8ToASCII(char* s)
{ 
  unsigned int k = 0;
  unsigned int len = strlen(s);
  byte c;

  for (unsigned int i = 0; i < len; i++)
  {
    c = s[i];    
    if (c == 0xC2)
    {
      i++; // skip to the next character
      c = s[i];            
      if      (c == 0xB5) c = 'u';  // micro
      else if (c == 0xB0) c = 0xDF; // degrees (specific for 16x2 LCD character set)
      else if (c == 0xB2) c = '2';  // squared          
    }
    
    s[k] = c;
    k++;
  }
  s[k] = 0;
}
