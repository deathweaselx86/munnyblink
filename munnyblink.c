#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
/*
Creepy Custom Munny code for use with
atmega328p and similar AVR microcontrollers.
Jessica Ross, 03/07/2013

Distributed under Created Commons 3.0 -- Attrib & Share Alike

This is based on the RGB Wave firmware for the ATtiny25 by
Mitch Altman.

Find that here:
http://www.tvbgone.com/mfaire/rgbwaveskit/rgb.c
*/

struct rgbElement {
  int fadeTime;            // how long to fade from previous values of RGB to the ones specified in this rgbElement (0 to 65,535)
  int holdTime;            // how long to keep the RGB values once they are faded in (0 or 1000 to 65,535)
  unsigned char red;       // brightness value for Red LED (0 to 255)
  unsigned char green;     // brightness value for Green LED (0 to 255)
  unsigned char blue;      // brightness value for Blue LED (0 to 255)
} const lightTab[] PROGMEM = {
  {     0,   500, 255,   0,   0 },
  {   500,     0,   0,   0,   0 },
  {     0,   500,   0, 255,   0 },
  {   500,     0,   0,   0,   0 },
  {     0,   500,   0,   0, 255 },
  {   500,     0,   0,   0,   0 },
  {  2500,  2500, 255,   0,   0 },
  {  2500,  2500,   0, 255,   0 },
  {  2500,  2500,   0,   0, 255 },
  {  2500,  2500, 155,  64,   0 },
  {  2500,  2500,  64, 255,  64 },
  {  2500,  2500,   0,  64, 255 },
  {  2500,  2500,  64,   0,  64 },
  {     0,  1500, 155,   0,   0 },
  {     0,  1500,   0, 255,   0 },
  {     0,  1500,   0,   0, 255 },
  {     0,  1500, 140,   0, 240 },
  {     0,  1500, 155, 155,   0 },
  {     0,  1500, 155, 255, 255 },
  {     0,  1500, 128, 128, 128 },
  {     0,  1500,  48,  48,  58 },
  {     0,  1500,   0,   0,   0 },
  {  2500,  2500, 155,   0,   0 },
  {  2500,  2500, 155, 255,   0 },
  {  2500,  2500,   0, 255,   0 },
  {  2500,  2500,   0, 255, 255 },
  {  2500,  2500,   0,   0, 255 },
  {  2500,  2500, 155,   0, 255 },
  {  2500,     0,   0,   0,   0 },
  {  2500,  2500, 155,   0,   0 },
  {  2500,  2500, 155, 255,   0 },
  {  2500,  2500,   0, 255,   0 },
  {  2500,  2500,   0, 255, 255 },
  {  2500,  2500,   0,   0, 255 },
  {  2500,  2500, 155,   0, 255 },
  {  2500,     0,   0,   0,   0 },
  {  2500,  2500, 154,  32,   0 },
  {  2500,  2500, 154, 128,   0 },
  {  2500,  2500, 154, 240,   0 },
  {  2500,  2500, 128, 240,   0 },
  {     0,  2500,   0,   0,   0 },
  {  2500,  2500,   0,  16, 255 },
  {  2500,  2500,   0, 128, 255 },
  {  2500,  2500,   0, 240, 128 },
  {  2500,  2500,  16,  16, 240 },
  {  2500,  2500, 140,  16, 240 },
  {  2500,  2500,  64,   0, 250 },
  {     0,  2500,  10,  10,  10 },
  {     0,  2500,   0,   0,   0 },
  {  2500,  2500, 140,   0, 240 },
  {  2500,  2500,  32,   0, 240 },
  {  2500,  2500, 128,   0, 128 },
  {  2500,  2500, 140,   0,  32 },
  {  2500,     0,   0,   0,  10 },
  {  2500,     0,   0,   0,   0 },
  {  1000,  1000,   0,   0,   0 },
  {  1000,  1000,  32,   0,   0 },
  {  1000,  1000,  64,   0,   0 },
  {     0,  1000,  96,   0,   0 },
  {  1000,     0, 128,   0,   0 },
  {  1000,     0, 160,  32,   0 },
  {  1000,     0, 192,  64,   0 },
  {  1000,     0, 124,  96,   0 },
  {     0,  1000, 155, 128,   0 },
  {  1000,  1000,   0, 160,   0 },
  {     0,  1000,   0, 192,   0 },
  {  1000,  1000,   0, 224,  32 },
  {  1000,     0,   0, 255,  64 },
  {  1000,     0,   0,   0,  96 },
  {  1000,     0,   0,   0, 128 },
  {  1000,     0,   0,   0, 160 },
  {  1000,     0,   0,   0, 192 },
  {  1000,     0,   0,   0, 224 },
  {  1000,  1000,   0,   0, 255 },
  {  1000,     0,   0,   0,   0 },
  {     0,  1000,   0,   0, 255 },
  {  1000,  1000,  32,   0,   0 },
  {  1000,  1000,  96,   0,   0 },
  {  1000,  1000, 160,   0,   0 },
  {  1000,     0, 255,   0,   0 },
  {  1000,  1000,   0,  96,   0 },
  {  1000,  1000,   0, 160,  32 },
  {  1000,  1000,   0, 224,  64 },
  {  1000,  1000,   0, 255,  96 },
  {  1000,  1000,   0,   0, 128 },
  {  1000,  1000,   0,   0, 160 },
  {     0,  1000,   0,  32, 192 },
  {     0,  1000,   0,  64, 224 },
  {     0,  1000,   0,  96, 225 },
  {     0,  1000,   0, 128,   0 },
  {     0,  1000,   0, 160,   0 },
  {     0,  1000,   0, 192,  32 },
  {     0,  1000,   0, 224,  64 },
  {     0,  1000,   0, 255,  96 },
  {     0,  1000,   0,   0, 255 },
  {     0,  1000,   0,   0,   0 },
  {     0,     0,   0,   0,   0 }
};

void setRGBElements(unsigned char index);

void setRGBElements(unsigned char index)
{
  // get current values of rgbElement from lightTab
  int FadeTime = pgm_read_word(&lightTab[index].fadeTime);
  int HoldTime = pgm_read_word(&lightTab[index].holdTime);
  unsigned char Red = pgm_read_byte(&lightTab[index].red);
  unsigned char Green = pgm_read_byte(&lightTab[index].green);
  unsigned char Blue = pgm_read_byte(&lightTab[index].blue);
  
  // get previous RGB brightness values from lightTab
  //   (these values are set to 0 if index to lightTab = 0)
  unsigned char redPrev = 0;    // keep track of previous Red brightness value
  unsigned char greenPrev = 0;  // keep track of previous Green brightness value
  unsigned char bluePrev = 0;   // keep track of previous Blue brightness value
  
  // set color timing values
  //   everytime the fadeCounter reaches this timing value in the fade loop
  //   we will update the color value for the color (default value of 0 for no updating)
  int redTime = 0;
  int greenTime = 0;
  int blueTime = 0;
  int fadeCounter = 0; 
  if (index != 0) {
    redPrev = pgm_read_byte(&lightTab[index-1].red);
    greenPrev = pgm_read_byte(&lightTab[index-1].green);
    bluePrev = pgm_read_byte(&lightTab[index-1].blue);
  }

  // set values of temp colors
  //   starting from the previous color values,
  //   these will change to the color values just gotten from rgbElement over fadeTime
  unsigned char redTemp = redPrev;
  unsigned char greenTemp = greenPrev;
  unsigned char blueTemp = bluePrev;

  // fade LEDs up or down, from previous values to current values
  int redDelta = Red - redPrev;                // total amount to fade red value (up or down) during fadeTime
  int greenDelta = Green - greenPrev;          // total amount to fade green value (up or down) during fadeTime
  int blueDelta = Blue - bluePrev;             // total amount to fade blue value (up or down) during fadeTime

  if (redDelta != 0) {
    redTime = (FadeTime / redDelta);           // increment Red value every time we reach this fade value in the fade loop
    if (redTime < 0) redTime = -redTime;       //    absolute value
    redTime = redTime + 1;                     // adjust for truncation of integer division
  }                                            //
  int redTimeInc = redTime;                    // increment Red value every time the fadeCounter increments this amount

  if (greenDelta != 0) {
    greenTime = (FadeTime / greenDelta);       // increment Green value every time we reach this fade value in the fade loop
    if (greenTime < 0) greenTime = -greenTime; //    absolute value
    greenTime = greenTime + 1;                 // adjust for truncation of integer division
  }                                            //
  int greenTimeInc = greenTime;                // increment Green value every time the fadeCounter increments this amount

  if (blueDelta != 0) {
    blueTime = (FadeTime / blueDelta);         // increment Blue value every time we reach this fade value in the fade loop
    if (blueTime < 0) blueTime = -blueTime;    //    absolute value
    blueTime = blueTime + 1;                   // adjust for truncation of integer division
  }                                            //
  int blueTimeInc = blueTime;                  // increment Blue value every time the fade value increments this amount

  // set color increment values
  //   the amount to increment color value each time we update it in the fade loop
  //   (default value of 1, to slowly increase brightness each time through the fade loop)
  unsigned char redInc = 1;
  unsigned char greenInc = 1;
  unsigned char blueInc = 1;
  // if we need to fade down the brightness, then make the increment values negative
  if (redDelta < 0) redInc = -1;
  if (greenDelta < 0) greenInc = -1;
  if (blueDelta < 0) blueInc = -1;


  // fade loop
  //   this loop will independently fade each LED up or down according to all of the above variables
  //   (it will take a length of time, FadeTime, to accomplish the task)
  //   this loop is not executed if FadeTime = 0 (since 1 is not <= 0, in the "for" loop)
  for (fadeCounter=1; fadeCounter<=FadeTime; fadeCounter++) {
    if ( fadeCounter == redTime ) {
      redTemp = redTemp + redInc;                 // increment to next red value
      redTime = redTime + redTimeInc;             // we'll increment Red value again when FadeTime reaches new redTime
    }
    if ( fadeCounter == greenTime ) {
      greenTemp = greenTemp + greenInc;           // increment to next green value
      greenTime = greenTime + greenTimeInc;       // we'll increment Green value again when FadeTime reaches new greenTime
    }
    if ( fadeCounter == blueTime ) {
      blueTemp = blueTemp + blueInc;              // increment to next blue value
      blueTime = blueTime + blueTimeInc;          // we'll increment Blue value again when FadeTime reaches new blueTime
    }
    OCR0A = redTemp;    //PD6   // update PWM for Red LED on OC1A (pin 6)
    OCR0B = blueTemp;   //PD5   // update PWM for Blue LED on OC1B (pin 3)
    OCR2B = greenTemp; //PD3

    OCR1A = redTemp;
    OCR1B = blueTemp;
    OCR2A = greenTemp;
  }
  OCR1A = Red;    // leave Timer1 PWM at final brightness value for Red (in case there were rounding errors in above math)
  OCR1B = Blue;   // leave Timer1 PWM at final brightness value for Blue (in case there were rounding errors in above math)
  
}
int main(void)
{
	
    // set all PWM pins as outputs
    DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3); 
    DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);
    
    
    //  For Timer/Counter0 and pins PD6, PD5 
    OCR0A = 128;
    OCR0B = 128;
    // For Timer/Counter1 and pins PB1, PB2  
    OCR1A = 128;
    OCR1B = 128; 
    // For Timer/Counter2 and pins PD3, PB3
    OCR2A = 128;    
    OCR2B = 128;
    
    
    // Enable variable TOP and set PWM to non-inverted mode.  
    TCCR0A |= _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
    // Set waveform generator mode bits. This sets it to Fast PWM Mode. 
    // TOP is 0xFF, sadly.Initially set Timer/Counter 0 to have a prescaler of 1024.
    TCCR0B |= _BV(CS00) | _BV(CS01);
   
     
    // For Timer/Counter 1 and pins PB1, PB2 16 bit mode
    // These pins are sychronized! We shouldn't set two pins from the same LED on
    // PB1, PB2.
    // Set PWM to non-inverted mode.
    TCCR1A |= _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10) | _BV(WGM11);
    
    // Set waveform generator mode bits. Fast PWM mode with TOP in OCR1A.
    // Timer/Counter 1 is set to have a prescaler of 1024.
    TCCR1B |= _BV(WGM12) | _BV(WGM13) | _BV(CS11);
    
    

    // For Timer/Counter 2 and pins PB2 and PD3 
    
    // Enable variable TOP and set PWM to non-inverted mode.  
    TCCR2A |= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    // Set waveform generator mode bits. This sets it to Fast PWM Mode with TOP in
    // OCR2A and OCR2B. Timer/Counter 2 is set to have a prescaler of 1024 for now.
    // Maybe too fast.
    //TCCR2B |= _BV(WGM22) | _BV(CS22) | _BV(CS21);
    TCCR2B |= _BV(CS22);
     
    unsigned char index = 1;
    while (1)
	{
		index = ( index + 1) % 96;
		setRGBElements(index);
		_delay_ms(1000);
    	}
	
	return 0;

}
