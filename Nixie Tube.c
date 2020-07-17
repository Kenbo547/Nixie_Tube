/*
 * Nixie_Tube.c
 *
 * Created: 14/01/2020 16:13:17
 *  Author: KENBO
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "Nixie Tube.h"
#define Middle_Point 0x3F
uint8_t Hours, minute, seconds, Scanning_Counter, Mode, ChangeModeCounter, Temp, Humidty, dummy, Read_Time_Counter, Duration_Counter, Old_Seconds, Old_Minute;
uint8_t ChangeModeFlag,Change_Voltage, Read_Time_Flag, Timer_Temp, dummy ;
int16_t temp;
uint8_t EEMEM EE_Change_Voltage;

struct rtc_time Time[3];
int main(void)
{
	//Outputs
	DDRC |= (1<<Cathode_0)|(1<<Cathode_1)|(1<<Double_Point)|(1<<Cathode_9);
	DDRD |= (1<<Cathode_5)|(1<<Cathode_6)|(1<<SHDWN_Power_170V)|(1<<Anode_4)|(1<<Cathode_7)|(1<<Cathode_8)|(1<<Temp_OneWire);
	DDRB |= (1<<Cathode_4)|(1<<Anode_3)|(1<<Anode_2)|(1<<Cathode_2);
	DDRE |= (1<<Cathode_3)|(1<<Anode_1)|(1<<Anode_5)|(1<<Anode_6);
	//DDRD |= (1<<PWM_DImming);

	
	DDRB &= ~(1 << Mode_Button);     // Set Pin To Input
	PORTB |= (1 << Mode_Button);    // turn On the Pull-up
	DDRB &= ~(1 << Up_Button);     // Set Pin To Input
	PORTB |= (1 << Up_Button);    // turn On the Pull-up
	DDRB &= ~(1 << Down_Button);     // Set Pin To Input
	PORTB |= (1 << Down_Button);    // turn On the Pull-up
	PORTD |= (1<<SHDWN_Power_170V);	 //170V Power OFF
	PORTD &= ~ (1<<SHDWN_Power_170V);	 //170V Power ON
	Clear_Cathodes();
	Read_Time_Flag = 0;
	Timer_Temp = 0;
	Read_Time_Counter = 0;
	Change_Voltage = eeprom_read_byte((uint8_t*)EE_Change_Voltage);
	Change_Voltage = 0x1F;
	mcp4017_set_resistance(0x1F);				//Set to 180V
	
	//init scanning frequency --> Timer 1  1ms Timer
	TCCR1A = 0;
	TCCR1B |= (1<<WGM12)|(1<<CS11)|(1<<CS10);	//CTC, Prescaler 64
	OCR1A = 60;
	TIMSK1 |= (1<<OCIE1A);	
	
	Mode = 0;
	Read_Time_Flag = 1;
	ChangeModeFlag = 0;
	i2c_init();
	rtc3231_init();
	ds18b20wsp( &PORTD, &DDRD, &PIND, (1 << Temp_OneWire) , NULL, 0x00, 0x00,0x1F);			//9bit resolution
	Read_Time();
	sei();
	//mcp4017_set_resistance(0x22);
	
	dummy = 0;
	Loop();
	
	
	

}
void Loop(){
	
while(1)
{
	
	// Get Buttons
	
	if(!(PINB & (1 << Mode_Button))){
		_delay_ms(200);
		Duration_Counter++;
		if((PINB & (1 << Mode_Button))){
			
			switch (Mode)
			{
				case 0:	
				if (ChangeModeFlag == 0	)
				{
					//Read_Time_Flag = 0;
					dummy = 0;
					ChangeModeFlag = 1;
					Read_Temperature();
				}
				else
				{
					//Read_Time_Flag = 1;
					ChangeModeFlag = 0;
				}
							//Show Temp
				
				break;
				case 1:
				Mode ++;
				break;
				case 2:
				Mode = 0;
				Write_Time();
				Read_Time_Flag = 1;
				break;
				case 3:
				
				break;
				case 4:
				Read_Time_Flag = 1;
				Mode = 0;
				eeprom_write_byte ((uint8_t*)EE_Change_Voltage, Change_Voltage);
				break;
				case 5:
				Mode = 1;
				break;
			}
		}
		if((Duration_Counter == 20) && (Mode == 0) && (ChangeModeFlag == 0)) {
			
			Mode = 5;
			Duration_Counter = 0;
			Read_Time_Flag = 0;
			
		}	
		
		
		
	}
	
	if((!(PINB & (1 << Up_Button))) && (Mode > 0)){
		_delay_ms(200);
		if((PINB & (1 << Up_Button))){
			switch (Mode)
			{
				
				case 1:
				Hours ++;
				if (Hours == 24)
				{
					Hours = 0;
				}
				break;
				case 2:
				minute ++;
				if (minute == 60)
				{
					minute = 0;
				}
				break;
				case 4:
				mcp4017_set_resistance(Change_Voltage++);
				break;
			}
		}
	}
	
	if((!(PINB & (1 << Down_Button))) && (Mode >= 0)){
		_delay_ms(200);
		if((PINB & (1 << Down_Button))){
			switch (Mode)
			{
				case 0:
				if (PIND & (1<<SHDWN_Power_170V))
				{
					PORTD &= ~ (1<<SHDWN_Power_170V);	 //170V Power ON
				}
				else
				{
					PORTD |= (1<<SHDWN_Power_170V);	 //170V Power OFF
					
				}
				break;
				case 1:
				if (Hours == 0)
				{
					Hours = 23;
				}
				else
				{
					Hours --;	
				}
				
				break;
				case 2:
				if (minute == 0)
				{
					minute = 59;
				}
				else
				{
					minute --;
				}
				
				break;
				case 4:
				mcp4017_set_resistance(Change_Voltage--);
				break;
			}
			
		}
	}
	
	if((!(PINB & (1 << Down_Button))) && (!(PINB & (1 << Up_Button)))){
		_delay_ms(200);
		Duration_Counter++;
		if(Duration_Counter == 20){
			Mode = 4;
			Read_Time_Flag = 0;
			seconds = 21;
			minute = 43;
			Hours = 65;
			Duration_Counter = 0;
		}
		
	}
	else
	{
		//Duration_Counter = 0;
	}
	
	
	
}
}
ISR(TIMER1_COMPA_vect){
	
	Scanning_Counter++;
	Blanking();
	Clear_Cathodes();
	//Read_Temperature();	
	switch (Scanning_Counter)
	{
		
	
	case 1:
		if (((Mode <= 1)||(Mode == 4)) && ((ChangeModeFlag == 1)||(ChangeModeFlag == 0)))
		{
			PORTE &= ~(1<<Anode_6);
			PORTE |= (1<<Anode_1);
			
			if ((Mode == 0) && (ChangeModeFlag == 1))
			{
				Digit_To_Cathode(Temp/10);
			}
			else
			{
				Digit_To_Cathode(Hours/10);
			}
			
			//Digit_To_Cathode(Hours/10);
		}
		
	break;
	
	case 2:
		if (((Mode <= 1)||(Mode == 4)) && ((ChangeModeFlag == 1)||(ChangeModeFlag == 0)))
		{
			PORTE &= ~(1<<Anode_1);
			PORTB |= (1<<Anode_2);
			
			if ((Mode == 0) && (ChangeModeFlag == 1))
			{
				Digit_To_Cathode(Temp%10);
			}
			else
			{
				Digit_To_Cathode(Hours%10);
			}
			
			//Digit_To_Cathode(Hours%10);
			
		}
	break;
	
	case 3:
	
		if (((Mode == 0)||(Mode == 2)||(Mode == 4)) && ((ChangeModeFlag == 2)||(ChangeModeFlag == 0)))
		{
			PORTB &= ~(1<<Anode_2);
			PORTB |= (1<<Anode_3);
			if ((Mode == 0) && (ChangeModeFlag == 2))
			{
				Digit_To_Cathode(Temp/10);
			}
			else
			{
				Digit_To_Cathode(minute/10);
			}
			
		}		
		
	/*
		PORTB &= ~(1<<Anode_2);
		PORTB |= (1<<Anode_3);
		Digit_To_Cathode(minute/10);
		*/
	break;
	
	case 4:
	
		if (((Mode == 0)||(Mode == 2)||(Mode == 4)) && ((ChangeModeFlag == 2)||(ChangeModeFlag == 0)))
		{				
			PORTB &= ~(1<<Anode_3);
			PORTD |= (1<<Anode_4);
		}
		if ((Mode == 0) && (ChangeModeFlag == 2))
		{
			Digit_To_Cathode(Temp%10);
		}
		else
		{
			Digit_To_Cathode(minute%10);
		}
		
		/*
		PORTB &= ~(1<<Anode_3);
		PORTD |= (1<<Anode_4);
		Digit_To_Cathode(minute%10);
		*/
	break;
	
	case 5:
		if (((Mode == 0)||(Mode == 4)) && ((ChangeModeFlag == 3)||(ChangeModeFlag == 0)))
		{
		PORTD &= ~(1<<Anode_4);
		PORTE |= (1<<Anode_5);
		if ((Mode == 0) && (ChangeModeFlag == 3))
		{
			Digit_To_Cathode(Temp/10);
		}
		else
		{
			Digit_To_Cathode(seconds/10);
		}
		
		}
	break;
	
	case 6:
		if (((Mode == 0)||(Mode == 4)) && ((ChangeModeFlag == 3)||(ChangeModeFlag == 0)))
		{
		PORTE &= ~(1<<Anode_5);
		PORTE |= (1<<Anode_6);
		if ((Mode == 0) && (ChangeModeFlag == 3))
		{
			Digit_To_Cathode(Temp%10);
		}
		else
		{
			Digit_To_Cathode(seconds%10);
		}
		
		}
	break;
	
	//Read_Time();
	
	
	}
	if (Scanning_Counter == 6)
	{
		Scanning_Counter = 0;
		if (Read_Time_Counter++ == 100)
		{
			if (Read_Time_Flag == 1	)
			{
				Read_Time();
			}
			
			Read_Time_Counter = 0;
		}
		
	}
}

	

void Blanking(){
	PORTE &= ~(1<<Anode_6);
	PORTE &= ~(1<<Anode_1);
	PORTB &= ~(1<<Anode_2);
	PORTB &= ~(1<<Anode_3);
	PORTD &= ~(1<<Anode_4);
	PORTE &= ~(1<<Anode_5);
	_delay_us(100);
	
}

void Clear_Cathodes(){
	PORTB &= ~ ((1<<Cathode_4)|(1<<Cathode_2));
	PORTC &= ~ ((1<<Cathode_0)|(1<<Cathode_1)|(1<<Cathode_9));
	PORTD &= ~ ((1<<Cathode_5)|(1<<Cathode_6)|(1<<Cathode_7)|(1<<Cathode_8));
	PORTE &= ~ (1<<Cathode_3);
	
	
	
}
void Digit_To_Cathode(uint8_t Digit){
	switch (Digit)
	{
		
	
	case 0:
		PORTC |= (1<<Cathode_0);
	break;
	
	case 1:
		PORTC |= (1<<Cathode_1);
	break;

	case 2:
		PORTB |= (1<<Cathode_2);
	break;
	
	case 3:
		PORTE |= (1<<Cathode_3);
	break;
	
	case 4:
		PORTB |= (1<<Cathode_4);
	break;
	
	case 5:
		PORTD |= (1<<Cathode_5);
	break;
	
	case 6:
		PORTD |= (1<<Cathode_6);
	break;

	case 7:
		PORTD |= (1<<Cathode_7);
	break;
	
	case 8:
		PORTD |= (1<<Cathode_8);
	break;
	
	case 9:
		PORTC |= (1<<Cathode_9);
	break;
	}
}
void Read_Time()
{
	rtc3231_read_time(Time);
	Hours = Time->hour;
	minute = Time->min;
	seconds = Time->sec;
	
	if(Old_Seconds != seconds){
		Old_Seconds = seconds;
		if (ChangeModeFlag > 0)
		{
			PORTC &= ~(1<<Double_Point);
			Timer_Temp++;
			if (Timer_Temp == 5	)
			{
				Read_Temperature();
				Timer_Temp = 0;
				dummy++;
				if (dummy < 3)
				{
					ChangeModeFlag++;
				}
				
				else
				{
					ChangeModeFlag--;
				}
				
							
			}
		}
		else
		{
			PORTC ^= (1<<Double_Point);
		}
		
	}
	if (Old_Minute != minute)
	{
		Old_Minute = minute;
		Timer_Temp++;
		if (Timer_Temp == 5)
		{
			Timer_Temp = 0;
			ChangeModeFlag = 1;
			dummy = 0;
			Read_Temperature();
						
		}
	}
	
} 
void Write_Time()
{
	Time->sec = 0;
	Time->hour = Hours;
	Time->min = minute;
	rtc3231_write_time(Time);
}
void Read_Temperature(){
	ds18b20convert( &PORTD, &DDRD, &PIND, (1 << Temp_OneWire), NULL);
	sei();
	_delay_ms( 150 );
	
	ds18b20read( &PORTD, &DDRD, &PIND, (1 << Temp_OneWire) , NULL, &temp );
	temp = (temp >> 4);
	Temp = temp;
	sei();
}