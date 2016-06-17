//=====================================================================================
//
//	Author				:Cytron Technologies		
//	Project				:Interface Free Controller-Output Card (IFC-OC04)
//	Project description	:Sample2 for IFC-OC04 with Control Panel
//  Date				: 21 April 2008
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_oc.h"
#include "iic_cp.h"

//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================
#define add_oc1 		0b000011	// Please make sure the communication address on 
									// IFC-OC04 is compatible with the address 

//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);
	
//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write/modify the program here
	//--------------------------------------------------------------------------------
	unsigned short temp1,pwm;	// define 16-bits local variable

	//	User is advised not to change or remove the initialization function called 
	init();						// call initialization function of the setting in program

	while(1)
	{
		//--------------------------------------------------------------------------------
		// This program mainly for slave card,OC04 with Control Panel Card, IFC-CP04.
		// It will test the 4 push buttons on CP04, the push buttons are active low, hence,
		// when any of the push button is pressed, value '0' will be sent to microcontroller 
		// on master card, MB00. 
		// The output ports of OC04 will be activate/deactivate base on the program.
		// Do make sure to select CP1 as communication address on IFC-CP04.
		//--------------------------------------------------------------------------------

		// turn ON LCD backlight by calling function cp1_blight and send max value,255
		cp1_blight(255);

		// display the welcome message
		// each time displaying message on LCD,user has to set the position 
		cp1_goto(0,4);			// set the position of message to be displayed
		cp1_str("Welcome!");	// send message to be displayed by calling function cp1_str
		cp1_goto(1,4);			// set the position of message to be displayed
		cp1_str("IFC User");	// send message to be displayed by calling function cp1_str

		if(cp1_sw(1)==0)					// Test whether SW1 on IFC-CP04 is pressed
		{
			cp1_clr();						// clear LCD screen by calling function cp1_clr
			led1 = 1;						// turn ON LED1 on MB00 by sending value '1'
			buzzer = 1;						// turn ON buzzer on MB00 by sending value '1'
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// turn OFF buzzer on MB00 by sending value '0'
			while(1)
			{
				// this is an infinity loop after SW1 on IFC-CP04 is pressed, 
				// user need to press reset button on MB00 or CP04 to exit from the mode		
				// ON OUT1 & OUT2 on IFC-OC04 and display output status when SW1 is pressed
				cp1_goto(0,0);				// set the position of message to be displayed
				cp1_str("Demo1");			// send message to be displayed by calling function cp1_str
				cp1_goto(1,0);				// set the position of message to be displayed
				cp1_str("ON OUT1 & OUT2");	// send message to be displayed by calling function cp1_str
				oc_out1(add_oc1,1);			// ON OUT1
				oc_out2(add_oc1,1);			// ON OUT2
				oc_out3(add_oc1,0);			// OFF OUT3
				oc_out4(add_oc1,0);			// OFF OUT4
			}
		}

		if(cp1_sw(2)==0)					// Test whether SW2 on IFC-CP04 is pressed
		{
			cp1_clr();						// clear LCD screen by calling function cp1_clr
			led2 = 1;						// turn ON LED2 on MB00 by sending value '1'
			buzzer = 1;						// buzzer ON for first time
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			delay(50000);					// delay for buzzer OFF time
			buzzer = 1;						// buzzer ON for second times
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			while(1)
			{
				// this is an infinity loop after SW2 on IFC-CP04 is pressed, 
				// user need to press reset button on MB00 or CP04 to exit from the mode
				// ON All Output ports on IFC-OC04 and display output status when SW2 is pressed
				cp1_goto(0,0);				// set the position of message to be displayed 
				cp1_str("Demo2");			// send message to be displayed by calling function cp1_str
				cp1_goto(1,0);				// set the position of message to be displayed 
				cp1_str("ON All Output");	// send message to be displayed by calling function cp1_str
				oc_out(add_oc1,0b00001111);	// ON all output ports
			}
		}

		if(cp1_sw(3)==0)					// Test whether SW3 on IFC-CP04 is pressed
		{
			temp1=0;						// clear temp1
			pwm=0;							// pwm
			cp1_clr();						// clear LCD screen by calling function cp1_clr
			led3 = 1;						// turn ON LED3 on MB00 by sending value '1'
			buzzer = 1;						// buzzer ON for first time
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			delay(50000);					// delay for buzzer OFF time
			buzzer = 1;						// buzzer ON for second times
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			delay(50000);					// delay for buzzer OFF time
			buzzer = 1;						// buzzer ON for third times
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			while(1)
			{
				// this is an infinity loop after SW3 on IFC-CP04 is pressed, 
				// user need to press reset button on MB00 or CP04 to exit from the mode
				// When SW3 is pressed, output1 on IFC-OC04 is activated with PWM mode, 
				// the output voltage will increase gradually and maintain in its maximum value, 
				// 255 after the increment. PWM status will display on LCD 
				cp1_goto(0,0);				// set the position of message to be displayed 
				cp1_str("Demo3");			// send message to be displayed by calling function cp1_str
				cp1_goto(1,0);				// set the position of message to be displayed 
				cp1_str("PWM = ");			// send message to be displayed by calling function cp1_str
				cp1_goto(1,6);				// set the position of message to be displayed 
				cp1_dec(pwm,6);				// send PWM value to be displayed by calling function cp1_dec
				temp1+=1;					// increase temp1
				if(temp1=10000)				// the value of temp1 here will determine the speed of
				{							// PWM value being increase, the bigger the value, 
											// the slower the increment.
					temp1=0;				// clear temp1	
					if(pwm<255)pwm+=1;		// increase PWM value if less than 255
					oc_pwm(add_oc1,pwm);	// ON OUT1 with current PWM value
				}
			}
		}

	}
}

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program need a delay
	// this function can be call by type : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	// this is a initialization for set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}


//=====================================================================================
//	Interrupt vector
//=====================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

