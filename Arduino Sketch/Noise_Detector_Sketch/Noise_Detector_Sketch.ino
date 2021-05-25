/****************************************
PHYS360 - Arduino Project
Sound Detector & Visualization
By Nate Pierce
****************************************/

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_24bargraph bar = Adafruit_24bargraph();

const int sampleWindow = 10; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
const int maxScale = 24; // Number of LEDs on the bar graph

void setup() 
{
   Serial.begin(9600);
   
   bar.begin(0x70); // pass in the address
   bar.setBrightness(15);
   bar.blinkRate(0);
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window, t = 0
   unsigned int peakToPeak = 0;   // Amplitude measured as peak to peak voltage, initalized at zero initially
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // Collect data for the period specified by sampleWindow. Each while loop iteration is one sample. 
   // When the loop terminates, we go on to dispaly the sample. Then, void loop() executes again,
   // and starts the next sample.
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0); // reads analog voltage output to pin A0 on arduino
      if (sample < 1024)
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
   }
   
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   // double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
   // Serial.println(volts);

     // map 1v peak to peak level to the max scale of the display
     // map(value, fromLow, fromHigh, toLow, toHigh)
   int displayPeak = map(peakToPeak, 0, 600, 0, maxScale); // displayPeak is now an integer which represents the number of LED's to light up on the bargraph
   

   // draw the new sample
   for (int i = 0; i <= maxScale; i++)
   {
     if (i >= displayPeak)  // turn off these LEDs
     {
        bar.setBar(i, LED_OFF);
     }
     else if(displayPeak > maxScale)
     {
      bar.setBar(i, LED_RED); // bar will flash red when peaking
     }
     else
     {
       bar.setBar(i, LED_GREEN); 
     }
   }
      
   }
   bar.writeDisplay();  // write the changes we just made to the display
}
