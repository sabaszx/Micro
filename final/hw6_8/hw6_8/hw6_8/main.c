#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


void display_wave(){
	PORTD = 0b00000001;
	for(unsigned char i=0;i<8;i++){
		_delay_ms(50);
		PORTD = PORTD<<1;
		_delay_ms(50);
	}
}

void display_result_positive(char result){
	PORTD = result;
}

void display_result_nagative(char result){
	//result = result + 1;
	PORTD = result;
	_delay_ms(500);
	PORTD = 0x00;
	_delay_ms(500);
}


int main(){

	DDRD = 0xFF;  // DefinePORTD to be output
	DDRB = 0x00;  // Define PORTB to be input
	DDRC = 0x00;  // Define PORTC to be input

	unsigned char inputFromPinB; // variable to store 8 bit input value from PINB
	unsigned char inputFromPinC; // variable to store 8 bit input value from PINC
	unsigned char isInputFromPinBNagative;
	unsigned char isInputFromPinCNagative;
	unsigned char result;

	while(1){
		inputFromPinB = PINB; //0xFF
		inputFromPinC = PINC;

		isInputFromPinBNagative = (inputFromPinB & 0b10000000);
		isInputFromPinCNagative = (inputFromPinC & 0b00001000);

		if(isInputFromPinBNagative){
			inputFromPinB = ~inputFromPinB +1;
		}

		if(isInputFromPinCNagative){
			inputFromPinC |= 0xF0;
			inputFromPinC = ~inputFromPinC +1;
			}else{
			inputFromPinC &= 0x0F;
		}

		if(inputFromPinC == 0){
			display_wave();
			}else{
			result = inputFromPinB/inputFromPinC;
			if((isInputFromPinBNagative && !isInputFromPinCNagative) || (!isInputFromPinBNagative && isInputFromPinCNagative)){
				display_result_nagative(result);
				}else{
				display_result_positive(result);
			}
		}
	}

}