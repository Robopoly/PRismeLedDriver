/*
 Title:        PRismeLedDriver
 Description:  Control 32 LEDs using two TLC5925I LED drivers.
 Author:       Karl Kangur <karl.kangur@gmail.com>
 Version:      2.0
 Website:      http://robopoly.ch
*/

uint8_t i = 0;
uint16_t ir, norm;

// pins to control the two TLC5925I
#define CLK 7
#define SDO 6
#define SDI 5
#define OE 4
#define LE 3

#define IR A8

// define the low and high light limis
#define LIGHT_LOW 150
#define LIGHT_HIGH 600

int leds;

void setup()
{
  pinMode(CLK, OUTPUT);
  pinMode(SDI, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(LE, OUTPUT);
  
  // When OE is active (low), the output drivers are enabled. When OE is high, all output drivers are turned OFF (blanked).
  digitalWrite(OE, LOW);
  // Serial data is transferred to the respective latch when LE is high. The data is latched when LE goes low.
  digitalWrite(LE, HIGH);
}
  
void loop()
{
  // turn outputs off
  digitalWrite(OE, HIGH);
  
  // average 10 sensor values
  for(i = 0; i < 10; i++)
  {
    ir = (ir * i + analogRead(IR)) / (i + 1);
  }
  
  // limit value
  if(ir < LIGHT_LOW)
    ir = LIGHT_LOW;
  if(ir > LIGHT_HIGH)
    ir = LIGHT_HIGH;
  
  // normalize over 100
  norm = 100 * (ir - LIGHT_LOW) / (LIGHT_HIGH - LIGHT_LOW);
  
  // use normalized value to light up leds
  leds = 32 * norm / 100;
  
  // limit output intensity
  delay(32 - leds);
  
  // set up leds
  for(i = 0; i < 32; i++)
  {
    // set led value
    if(i < leds)
    {
      digitalWrite(SDI, HIGH);
    }
    else
    {
      digitalWrite(SDI, LOW);
    }
    // send clock
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  // turn outputs on
  digitalWrite(OE, LOW);
  delayMicroseconds(50);
}

