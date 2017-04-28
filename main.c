//*******************************************************
//************* CDA 3331 Microprocessors ****************
//************* Student Name: Saad Saeed ****************
//************* Instructor: Dr. Ravi Shankar*************
//************* Date 04/26/2017          ****************
//************* Individual Project       ****************
/********************************************************
 *
 */
#include <msp430.h>

int value=0, i=0 ;
int light = 0;
int xJoy = 0;
int yJoy =0;
int xAcc = 0;
int yAcc =0;
int flag =0;
int ADCReading [4];


// Function Prototypes
void buzzerControl(int valuePWM);
void ConfigureAdc(void);
void getanalogvalues();
void tone(int valuePWM);
void toneOff();
void miniSong();


int main(void)
{
    int upperButtonFlag=0;
    int lowerButtonFlag=0;
    int joySelectFlag=0;
    int value;
    WDTCTL = WDTPW + WDTHOLD;                 	// Stop WDT
    P1OUT = 0;
    P2OUT = 0;
    P1DIR = 0;
    P1REN = 0;
    P2REN = 0;
    P2DIR = 0;
    P1DIR |= BIT6;	// set bit  6 as outputs.
    P1SEL |= BIT6;// Disable interrupt for this port and use it as the timer output.
                  // Used to drive Buzzer with PWM

    //P2DIR |=   BIT0;				// set bit	0		as output
    ConfigureAdc();

    for (;;)
    {

        // reading light, touch, and temp repeatedly at the beginning of the main loop
        getanalogvalues();
        //Is the Joy Stick Select pressed?
        if (!(P1IN & BIT5)) {
            joySelectFlag =0x01;
            __delay_cycles(150000); //Debounce it.

        }

        //Is the Upper Button pressed?
        if (!(P1IN & BIT7)) {
            upperButtonFlag ^=0x01;
            __delay_cycles(150000); //Debounce it.

        }

        //Is the Lower Button pressed?
        if (!(P2IN & BIT3)) {
            lowerButtonFlag ^=0x01;
            __delay_cycles(150000); //Debounce it.

        }

        if (joySelectFlag){
            miniSong();
            joySelectFlag =0; //Play song only once
            upperButtonFlag=0; //Reset the flags
            lowerButtonFlag=0;
        }

        //Turn to enable joy stick enabled instrument with x-y control.
        if (upperButtonFlag){
            //if(xJoy<yJoy) //If you want to do min or max of {x,y}
                buzzerControl((xJoy+yJoy)>>1);  //PWM value calculated as (x+y)/2
            //else
                //buzzerControl(yJoy);
        }
        //Turn to enable x-y Accelerometer enabled instrument with x-y control.
        if (lowerButtonFlag){
            //if(xJoy<yJoy) //If you want to do min or max of {x,y}
                value = xAcc+yAcc-1200;
                buzzerControl(value);  //PWM value calculated as (x+y)/2
            //else
                //buzzerControl(yJoy);
        }
        if(!upperButtonFlag && !lowerButtonFlag ){
            toneOff();
        }
        __delay_cycles(16000);
    }
}

void ConfigureAdc(void)
{
    ADC10CTL1 = INCH_3 | CONSEQ_1; 				// A3+ A2 + A1 + A0, single sequence
    ADC10CTL0 = ADC10SHT_2 | MSC | ADC10ON;
    while (ADC10CTL1 & BUSY);
    ADC10DTC1 = 0x04; 							// DTC transfers. These bits define the number of transfers in each block.4 conversions
    ADC10AE0 |= (BIT0 | BIT1 | BIT2 | BIT3); 			// ADC10 option select
}

void tone(int valuePWM){
    buzzerControl(valuePWM);
    __delay_cycles(400000);
}

void toneOff(){
    TACTL =  MC_0; // Stop Timer
    return;
}
void miniSong(){
    toneOff();
    tone(500);
    tone(200);
    tone(600);
    tone(300);
    tone(400);
    tone(100);
    tone(50);
    tone(100);
    tone(700);
    tone(500);
    tone(200);
    tone(600);
    tone(300);
    tone(400);
}

void buzzerControl(int valuePWM)
{
    volatile float D;
    int period = 0x7FFF;

    D = ((valuePWM* 1.0)/1024); //normalize result 0.0 - 1.0
    //D = 0 => freq = infinity.
    //Dclose to 1 should have freq = 20 Hz.
    //Shaping input-frequency mapping
    TACCR0 = (period - 1)*(D*D*0.05+D*D*D*0.25+D*D*D*D*0.7+0.004);
    TACCR1 = TACCR0 * (0.5); // CCR1 PWM Duty Cycle. Set duty cycle to 50%
    TACCTL1 = OUTMOD_7; // CRR1 reset/set
    TACTL = TASSEL_2 | MC_1; // SMCLK, Up mode
}
// ADC10SA = Start Address Register for Data Transfer
// ADC10SC bit is selected as Sample and Hold Source.
void getanalogvalues()
{
    i = 0; xJoy = 0; yJoy = 0;
    //ADC programmed for four input channels/
    for(i=1; i<=5 ; i++)                              // read all three analog values 5 times each and average
    {
        ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);                         //Wait while ADC is busy
        ADC10SA = (unsigned)&ADCReading[0]; 			  //RAM Address of ADC Data, must be reset every conversion
        ADC10CTL0 |= (ENC | ADC10SC);                     //Start ADC Conversion
        while (ADC10CTL1 & BUSY);                         //Wait while ADC is busy
        // sum  all 5 reading for the three variables
        yAcc += ADCReading[0];
        xAcc += ADCReading[1];
        yJoy += ADCReading[2];
        xJoy += ADCReading[3];
    }
    yJoy = yJoy/5; xJoy = xJoy/5;     // Average the 5 reading for the three variables
    yAcc = yAcc/5; xAcc = xAcc/5;
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}

