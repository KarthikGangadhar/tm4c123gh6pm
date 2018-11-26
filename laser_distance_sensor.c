//-----------------------------------------------------------------------------

//2. You are asked to interface a laser distance sensor (FYI Baumer part #OADM20I4460_S14C) with
//external circuitry added to output a voltage that ranges from 0 to 10V corresponding to a detected range
//of 30-130mm to pin AIN3 of the controller. Show all circuitry necessary to interface the sensor to the
//controller without exceeding the input voltage ranges of the controller.


//3. For the circuit above, write a function, unsigned uint16_t read_dist(), that initializes the A/D converter,
//reads the value, and returns the distance in units of 0.1mm (i.e. a returned value of 400 means 40.0mm).

//4. Suppose a potentiometer with 270 degrees of free-rotation is connected with that tapers connected to
//Vdd and ground of the controller such that the voltage on the wiper of the pot is equal to angle / 270 *
//Vdd. Assume that the wiper is connected directly to pin AIN2. Write a function, unsigned uint16_t
//read_angle(), that initializes the A/D converter, reads the value, and returns the angle in units of 1 degree.

//5. Write a short routine that initializes wide timer 1 to create interrupts at 60Hz (or as close as possible).

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tm4c123gh6pm.h"

// To initialize Hardware requirement
void initDistanceHW(){
    // Configure AN3 as an analog input
     SYSCTL_RCGCADC_R |= 1;                           // turn on ADC module 0 clocking
     GPIO_PORTE_AFSEL_R |= 0x01;                      // select alternative functions for AN3 (PE0)
     GPIO_PORTE_DEN_R &= ~0x01;                       // turn off digital operation on pin PE0
     GPIO_PORTE_AMSEL_R |= 0x01;                      // turn on analog operation on pin PE0
     ADC0_CC_R = ADC_CC_CS_SYSPLL;                    // select PLL as the time base (not needed, since default value)
     ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN3;                // disable sample sequencer 3 (SS3) for programming
     ADC0_EMUX_R = ADC_EMUX_EM3_PROCESSOR;            // select SS3 bit in ADCPSSI as trigger
     ADC0_SSMUX3_R = 0;                               // set first sample to AN0
     ADC0_SSCTL3_R = ADC_SSCTL3_END0;                 // mark first sample as the end
     ADC0_ACTSS_R |= ADC_ACTSS_ASEN3;                 // enable SS3 for operation
}

void initAngleHW(){
     // wiper is connected directly to pin AIN2.
     SYSCTL_RCGCADC_R |= 1;                           // turn on ADC module 0 clocking
     GPIO_PORTE_AFSEL_R |= 0x01;                      // select alternative functions for AN2 (PE1)
     GPIO_PORTE_DEN_R &= ~0x01;                       // turn off digital operation on pin PE1
     GPIO_PORTE_AMSEL_R |= 0x01;                      // turn on analog operation on pin PE1
     ADC1_CC_R = ADC_CC_CS_SYSPLL;                    // select PLL as the time base (not needed, since default value)
     ADC1_ACTSS_R &= ~ADC_ACTSS_ASEN3;                // disable sample sequencer 3 (SS3) for programming
     ADC1_EMUX_R = ADC_EMUX_EM3_PROCESSOR;            // select SS3 bit in ADCPSSI as trigger
     ADC1_SSMUX3_R = 1;                               // set first sample to AN1
     ADC1_SSCTL3_R = ADC_SSCTL3_END0;                 // mark first sample as the end
     ADC1_ACTSS_R |= ADC_ACTSS_ASEN3;                 // enable SS3 for operation
}

void initWideTimerHW(){
    SYSCTL_RCGCWTIMER_R |= SYSCTL_RCGCWTIMER_R5;     // turn-on timer
    WTIMER5_CTL_R &= ~TIMER_CTL_TAEN;                // turn-off counter before reconfiguring
    WTIMER5_CFG_R = 4;                               // configure as 32-bit counter (A only)
    WTIMER5_TAMR_R = TIMER_TAMR_TACMR | TIMER_TAMR_TAMR_CAP | TIMER_TAMR_TACDIR; // configure for edge time mode, count up
    WTIMER5_CTL_R = TIMER_CTL_TAEVENT_POS;           // measure time from positive edge to positive edge
    WTIMER5_IMR_R = TIMER_IMR_CAEIM;                 // turn-on interrupts
    WTIMER5_TAV_R = 0;                              // zero counter for first period
    WTIMER5_CTL_R |= TIMER_CTL_TAEN;                 // turn-on counter
    NVIC_EN3_R |= 1 << (INT_WTIMER5A-16-96);         // turn-on interrupt 120 (WTIMER5A)
}

// method to read voltage
int16_t readAdc0Ss3()
{
    ADC0_PSSI_R |= ADC_PSSI_SS3;                     // set start bit
    while (ADC0_ACTSS_R & ADC_ACTSS_BUSY);           // wait until SS3 is not busy
    return ADC0_SSFIFO3_R;                           // get single result from the FIFO
}


// method to read voltage
int16_t readADC1Ss3()
{
    ADC1_PSSI_R |= ADC_PSSI_SS3;                     // set start bit
    while (ADC1_ACTSS_R & ADC_ACTSS_BUSY);           // wait until SS3 is not busy
    return ADC1_SSFIFO3_R;                           // get single result from the FIFO
}

//-----------------------------------------------------------------------------
//initializes the A/D converter,
//reads the value, and returns the distance in units of 0.1mm
//(i.e. a returned value of 400 means 40.0mm).
//-----------------------------------------------------------------------------
uint16_t read_dist(){

    // initialize analog to digital converter
    initDistanceHW();

    uint32_t voltage;
    float vtg;
    uint16_t distance;

    //reads the value
    voltage = readAdc0Ss3(); // count from 0 to 4096
    vtg = ((voltage * 10)/4096.0); // converting to voltage

    //distance conversion
    distance = 300 + (vtg * 10);

    //returns the distance in units of 0.1mm
    return distance;
}

//-----------------------------------------------------------------------------
//initializes the A/D converter, reads the value, and returns the angle in units of 1 degree.
//-----------------------------------------------------------------------------
uint16_t read_angle(){

    // initialize analog to digital converter
    initAngleHW();

    uint32_t voltage;
    float vtg;
    uint16_t angle;

    //reads the value
    voltage = readAdc1Ss3(); // count from 0 to 4096
    vtg = ((voltage * 3.3)/4096.0); // converting to voltage

    //angle conversion
    angle = ((vtg * 270) / 3.3);

    //returns the angle in units of 1 degree
    return angle;
}

/**
 * main.c
 */
void main(void)
{
    uint16_t distance;
	distance = read_dist();

    uint16_t angle;
    angle = read_angle();
}
