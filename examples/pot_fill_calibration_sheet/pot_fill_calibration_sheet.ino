#include <MCP4251.h>

MCP4251 digitalPot(53, 100300.0, 113.0, 100300.0, 113.0);


#define buttonPin_1 2

bool po2res = LOW;
bool res2pos = HIGH;
bool potNum = 1;
uint16_t pos, calcPos;
float setRes, calcRes;

volatile int buttonPin_1_State = LOW;

unsigned long buttonPin_1_CurrentTime = 0;
unsigned long buttonPin_1_LastTime = 0;

void setup()
{
    Serial.begin(9600);
    digitalPot.begin();
    pinMode(buttonPin_1, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin_1), buttonPin_1_ISR, RISING);

    digitalPot.DigitalPotSetWiperPosition(potNum, 0);
    delay(1000);
    digitalPot.DigitalPotSetWiperPosition(potNum, 256);
    delay(1000);

    if (po2res)
    {
        pos = 0;
        digitalPot.DigitalPotSetWiperPosition(potNum, pos);
        calcRes = digitalPot.DigitalPotPositionToResistance(potNum, pos);
        Serial.print("Wiper Position = ");
        Serial.println(pos);
        Serial.print("Resistance Computed = ");
        Serial.println(calcRes);
    }
    else if(res2pos)
    {
        setRes = 0.0;
        calcPos = digitalPot.DigitalPotResistanceToPosition(potNum, setRes);
        digitalPot.DigitalPotSetWiperPosition(potNum, calcPos);
        Serial.print("Set Resistance = ");
        Serial.println(setRes);
        Serial.print("Position Computed = ");
        Serial.println(calcPos);
    }
    
}

void loop()
{

    if (buttonPin_1_State)
    {
        if (po2res)
        {
            pos = pos + 10;
            digitalPot.DigitalPotSetWiperPosition(potNum, pos);
            calcRes = digitalPot.DigitalPotPositionToResistance(potNum, pos);
            Serial.print("Wiper Position = ");
            Serial.println(pos);
            Serial.print("Resistance Computed = ");
            Serial.println(calcRes);
        }
        else if(res2pos)
        {
            setRes = setRes + 2000.0;
            calcPos = digitalPot.DigitalPotResistanceToPosition(potNum, setRes);
             digitalPot.DigitalPotSetWiperPosition(potNum, calcPos);
            Serial.print("Set Resistance = ");
            Serial.println(setRes);
            Serial.print("Position Computed = ");
            Serial.println(calcPos);
        }
    }
    buttonPin_1_State = LOW;
}

void buttonPin_1_ISR()
{

    // detachInterrupt(digitalPinToInterrupt(buttonPin_1));
    //buttonPin_1_State = !buttonPin_1_State;   // digitalRead(buttonPin_1);

    buttonPin_1_CurrentTime = millis();

    if (buttonPin_1_CurrentTime - buttonPin_1_LastTime > 250)
    {
        buttonPin_1_State = HIGH;
        buttonPin_1_LastTime = buttonPin_1_CurrentTime;
    }
}
