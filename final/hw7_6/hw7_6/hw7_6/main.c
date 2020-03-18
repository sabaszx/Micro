#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void initPort(void)
{
	DDRC = 0xff;
	DDRD = 0x01;
}
int main(void)
{
	unsigned char LOOKUPTB[] = { 0x3F,
		0x06,
		0x5B,
		0x4F,
		0x66,
		0x6D,
		0x7D,
		0x07,
		0x7F,
		0x6F,
		0x77,
		0x7C,
		0x39,
		0x5E,
		0x79,
		0x71 };
	
	unsigned char sw1, sw2, result, display, status;
	result = 0;
	status = 0;
	initPort();
	while (1)
	{
		sw1 = PIND;
		sw1 &= 0x02;
		sw2 = PIND;
		sw2 &= 0x04;
		
		if (sw2 == 0) {
			status = ~status;
			_delay_ms(1000);
		}
		if(status == 0){
			if(result == 15){
				result =0;
			}
			else{
				result++;
			}
		}
		else{
			if(result == 0){
				result=15;
			}
			else{
				result--;
			}
		}
		if (sw1 == 0){
			result = 0;
		}
		display = LOOKUPTB[result];
		PORTC = display;
		display &=0x40;
		display = display >>6;
		PORTD = display;
		_delay_ms(1000);
	}
}