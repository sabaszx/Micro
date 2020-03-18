#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

signed char pressedKey;
signed char row;
// Switch Control, Switch using Active Low
void redOn(){
	PORTD &= 0b11011111; //PD5 Low
}

void greenOn(){
	PORTD &= 0b11101111;//PD4 Low
}

void redOff(){
	PORTD |= 0b00100000; //Only PD5 Must High
}

void greenOff(){
	PORTD |= 0b00010000; //Only PD4 Must Low
}

// Blink Setting
void blinkRed(){
	redOn();
	_delay_ms(500);
	redOff();
	_delay_ms(500);
}

void blinkGreen(){
	greenOn();
	_delay_ms(500);
	greenOff();
	_delay_ms(500);
}
void initPort(){
	DDRB = 0b00001111; // 0x0F
	DDRD = 0b11110111; //0xFF but INT1 is located on PD3, it must be input
	DDRC = 0b11111000; //PORT C will be input
}
//Interrupt Service Routine of INT1
ISR(INT1_vect){
	
	// Initial pressedKey with -1 represent to key pressed
	pressedKey = -1;
	
	// Initial row to 1 represent start search with row 1
	row = 1;
	
	do
	{
		//using port B output logic to find and Port C input logic to check
		if (row == 1)
		{
			// Check For Row 1,PB3 must be 0 (We have 1,2,3 in row 1)
			PORTB = 0b11110111;
			_delay_ms(10); //bounce check
			
			// Read Value from PINC
			switch(PINC & 0x07){
				case 0b011:
				pressedKey = 3; break;
				case 0b101:
				pressedKey = 2; break;
				case 0b110:
				pressedKey = 1; break;
			}
		}
		
		else if(row ==2){
			// Check For Row 2, PB 2 Must be 0
			PORTB = 0b11111011;
			_delay_ms(10); // bounce check
			
			// Read Value from PINC
			switch(PINC & 0x07){
				case 0b011:
				pressedKey = 6; break;
				case 0b101:
				pressedKey = 5; break;
				case 0b110:
				pressedKey = 4; break;
			}
		}
		
		else if(row ==3){
			// Check For Row 3, PB 1 Must be 0
			PORTB = 0b11111101;
			_delay_ms(10); // bounce check
			
			// Read Value from PINC
			switch(PINC & 0x07){
				case 0b011:
				pressedKey = 9; break;
				case 0b101:
				pressedKey = 8; break;
				case 0b110:
				pressedKey = 7; break;
			}
		}
		
		else if(row ==4){
			// Check For Row 0, PB 0 Must be 0
			PORTB = 0b11111110;
			_delay_ms(10); // bounce check
			
			// Read Value from PINC
			switch(PINC & 0x07){
				case 0b011:
				pressedKey = 11; break;
				case 0b101:
				pressedKey = 0; break;
				case 0b110:
				pressedKey = 10; break;
			}
		}
		
		row ++;
	} while (row <= 4);
	
	if (pressedKey == 0)
	{
		for (int i=0;i<5;i++) // blink for 5 times
		{
			blinkRed();
		}
	}
	else if (pressedKey != -1) // other button but not no click
	{
		for(int i=0;i<pressedKey;i++){
			blinkGreen();
		}
	}
	PORTB = 0b0000; // for receive next interrupt
}
int main(void)
{
	// Port and Pin Direction Setting
	//PORT D and PORT B will be output
	initPort();
	
	//Interrupt Flag Setting EIMSK,EICRA
	// bit 1 must be 1 (active)
	EIMSK = 0x02;
	
	//falling edge and logic low interrupt
	EICRA = 0b0100;
	
	//Interrupt Setting
	sei();
	
	// Initial Setting
	// Input will hold all port high because we want interrupt on low
	PORTC = 0x07; //0b111
	
	// Output reset with all low because we want low to toggle interrupt
	PORTB = 0x00;
	
	// Hold high logic in Interrupt port INT0
	PORTD |= (1<<3);
	greenOff();
	redOff();
	
	while (1)
	{
	}
}