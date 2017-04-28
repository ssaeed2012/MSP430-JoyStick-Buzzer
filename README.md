# Trimodal Music Instrument
This project uses the MSP 430 launchpad and Booster pack MKII from TI. 
## Purpose of the Project:
1.)Uses x and y channels of the joy stick to modify the frequency of the music. This feature can be toggled on and off using the upper button on the Booster pack.
2.)Uses the Select switch of the Joy Stick to play a preprogrammed mini song.The song is played once. 
3.)Uses the other button to enable x and y channels of the accelerometer to create a motion sensitive musical instrument. 

## Why I did this  project
To demonstrate the awesome features of the booster pack board

## Resources of the Launchpad used:
Four Analog input channels to be sampled by ADC:
  Two for X and Y inputs from Joystick (A0 and A1)
  Two for X and Y inputs from the Accelerometer (A2 and A3)
    Pins P1.0 – P1.3 are then unavailable. 
Three binary Input ports: 
  one for Joystick select button (P1.5) and 
  two for the two buttons (P1.7 and P2.3)
Timer and Port P1.6 for Timer Output Signal to be connected with the Buzzer
