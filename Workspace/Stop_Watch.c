/*
 * Mini_Project_2.c
 *
 *  Created on: Sep 14, 2022
 *      Author: Mohamed Nasser
 */

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char sec_first_digit   = 0;
unsigned char sec_second_digit  = 0;
unsigned char min_first_digit   = 0;
unsigned char min_second_digit  = 0;
unsigned char hour_first_digit  = 0;
unsigned char hour_second_digit = 0;

void Timer1_compare_init (void)
{
	TCNT1 = 0;
	TCCR1A = (1 << FOC1A);
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); //FCPU_64
	OCR1A = 15625;
	TIMSK |= (1 << OCIE1A);
	SREG |= (1 << 7);
}

void Int0_init (void)
{
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);
	SREG |= (1 << 7);
}

void Int1_init (void)
{
	DDRD &= ~(1 << PD3);
	MCUCR |= (1 << ISC10) | (1 << ISC11);
	GICR |= (1 << INT1);
	SREG |= (1 << 7);
}

void Int2_init (void)
{
	DDRB &= ~(1 << PB2);
	PORTB |= (1 << PB2);
	MCUCSR &= ~(1 << ISC2);
	GICR |= (1 << INT2);
	SREG |= (1 << 7);
}



ISR(INT0_vect)
{
	TCNT1 = 0;
	sec_first_digit   = 0;
	sec_second_digit  = 0;
	min_first_digit   = 0;
	min_second_digit  = 0;
	hour_first_digit  = 0;
	hour_second_digit = 0;
}

ISR(INT1_vect)
{
	TCCR1B &= 0xF8;
}

ISR(INT2_vect)
{
	TCCR1B |= (1 << CS11) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect)
{
	SREG |= (1 << 7);
	sec_first_digit++;
	if (10 == sec_first_digit)
	{
		sec_first_digit = 0;
		sec_second_digit++;
	}
	if (6 == sec_second_digit)
	{
		sec_second_digit = 0;
		min_first_digit++;
	}

	if (10 == min_first_digit)
	{
		min_first_digit = 0;
		min_second_digit++;
	}
	if (6 == min_second_digit)
	{
		min_second_digit = 0;
		hour_first_digit++;
	}

	if (10 == hour_first_digit)
	{
		hour_first_digit = 0;
		hour_second_digit++;
	}
	if (2 == hour_second_digit && 4 == hour_first_digit)
	{
		hour_second_digit = 0;
		hour_first_digit  = 0;
	}
}



int main (void)
{
	DDRC |= 0x0F;

	DDRA |= 0x3F;
	PORTA &= 0xC0;

	Timer1_compare_init();
	Int0_init();
	Int1_init();
	Int2_init();

	for(;;)
	{
		PORTA &= 0xC0;
		PORTC = (PORTC & 0xF0) | (sec_first_digit & 0x0F);
		PORTA |= 0x01;

		PORTA &= 0xC0;
		PORTC = (PORTC & 0xF0) | (sec_second_digit & 0x0F);
		PORTA |= 0x02;

		PORTA &= 0xC0;
		PORTC = (PORTC & 0xF0) | (min_first_digit & 0x0F);
		PORTA |= 0x04;

		PORTA &= 0xC0;
		PORTC = (PORTC & 0xF0) | (min_second_digit & 0x0F);
		PORTA |= 0x08;

		PORTA &= 0xC0;
		PORTC = (PORTC & 0xF0) | (hour_first_digit & 0x0F);
		PORTA |= 0x10;

		PORTA &= 0xC0;
		PORTC = (PORTC & 0xF0) | (hour_second_digit & 0x0F);
		PORTA |= 0x20;
	}
}
