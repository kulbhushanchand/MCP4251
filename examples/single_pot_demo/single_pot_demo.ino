#include <MCP4251.h>

MCP4251 digitalPot(53, 98600.0, 113.5, 98600.0, 130.0);

#define buttonPin_1 2
bool potNum = 1;

uint16_t pos, data;
float calcRes, calcPos;

void setup()
{
    Serial.begin(9600);
    digitalPot.begin();
  
    pinMode(buttonPin_1, INPUT_PULLUP);
   
}

void loop()
{

pos = 256;
digitalPot.DigitalPotSetWiperPosition(potNum, pos);
Serial.print("Position = ");
Serial.println(pos);




while(1)
{
 


}

}