/*
 Title:        PRismeLedDriver
 Description:  Control 32 LEDs using two TLC5925I LED drivers.
 Author:       Karl Kangur <karl.kangur@gmail.com>
 Version:      1.0
 Website:      http://robopoly.ch
*/

#include <util/delay.h>

uint8_t i = 0;
uint16_t ir, norm;

// pins to control the two TLC5925I
#define CLK 7
#define SDO 6
#define SDI 5
#define OE 4
#define LE 3

// define the low and high light limis
#define LOW 150
#define HIGH 600

// helper functions
#define port_ddr(port) (port-1)
#define port_pin(port) (port-2)
#define pin_mode(port, pin, mode) (*(port_ddr(&port)) = (*(&port-1) & (~(1 << pin))) | (mode << pin))
#define digital_write(port, pin, value) (port = (port & (~(1 << pin))) | (value << pin))

int leds;

void setup()
{
  // clk #1 and #2
  pin_mode(PORTB, CLK, 1);
  // sdi #1
  pin_mode(PORTB, SDI, 1);
  // oe
  pin_mode(PORTB, OE, 1);
  // oe
  pin_mode(PORTB, LE, 1);
  
  // When OE is active (low), the output drivers are enabled. When OE is high, all output drivers are turned OFF (blanked).
  digital_write(PORTB, OE, 0);
  // Serial data is transferred to the respective latch when LE is high. The data is latched when LE goes low.
  digital_write(PORTB, LE, 1);
}
  
void loop()
{
  // turn outputs off
  digital_write(PORTB, OE, 1);
  
  // average 10 sensor values
  for(i = 0; i < 10; i++)
  {
    ir = (ir * i + analogRead(PA(0))) / (i + 1);
  }
  
  // limit value
  if(ir < LOW)
    ir = LOW;
  if(ir > HIGH)
    ir = HIGH;
  
  // normalize over 100
  norm = 100 * (ir - LOW) / (HIGH - LOW);
  
  // use normalized value to light up leds
  leds = 32 * norm / 100;
  
  // limit output intensity
  _delay_ms(32 - leds);
  
  // set up leds
  for(i = 0; i < 32; i++)
  {
    // set led value
    if(i < leds)
    {
      digital_write(PORTB, SDI, 1);
    }
    else
    {
      digital_write(PORTB, SDI, 0);
    }
    // send clock
    digital_write(PORTB, CLK, 1);
    digital_write(PORTB, CLK, 0);
  }
  // turn outputs on
  digital_write(PORTB, OE, 0);
  _delay_us(50);
}

