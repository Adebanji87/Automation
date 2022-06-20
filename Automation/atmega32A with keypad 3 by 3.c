/*
 * atmega32A_with_keypad_3_by_3.c
 *
 * Created: 4/12/2019 1:16:18 PM
 *  Author: Asafe
 */ 



                                               

#ifndef F_CPU
#define F_CPU        8000000UL
#endif
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "LCD16x2_4bit.h"
#include "Keypad.h"
#include "dht.h"
int EepromTemp = 20;
int EepromHumid = 25;
int RefTemp;
int RefHumid;
#define RELAY_PIN1 6
#define RELAY_PIN2 7
int8_t temperature, humidity;
unsigned char key;
unsigned int i=20;





void Humidpreset(){
	 
	 key=keypad_read();
	 if ( key != 0xff )
	 {
		 
		 LCD_setCursor(1, 1);
		 lcddata(key);
		 i = i + key ;
		 eeprom_write_byte((uint8_t*)EepromHumid, i);
		 eeprom_update_byte((uint8_t*)EepromHumid, i);
		 RefHumid=eeprom_read_byte((const uint8_t*)EepromHumid);
		 _delay_ms(500);
	 }
   
         
		LCD_setCursor (3,1);
		lcd_print ("RefHumid: ");
		LCD_setCursor (12,1);
		lcdWriteInt(RefHumid,2);
		LCD_setCursor (15,1);
		lcd_print ("%");
	if (humidity>=RefHumid)
	{
		PORTC &= ~(1 << RELAY_PIN1);
	}
	else
	{
		PORTC |= (1 << RELAY_PIN1);
	}
	if (temperature>=RefTemp)
	{
		PORTC &= ~(1 << RELAY_PIN2);
	}
	else
	{
		PORTC |= (1 << RELAY_PIN2);
	}
}
void Temppreset()
{
	     
   key=keypad_read();
 if ( key != 0xff )
 {  LCD_setCursor(1, 1);
	 lcddata(key);
	 i =  i + key-38;
	 eeprom_write_byte((uint8_t*)EepromTemp, i);
	 eeprom_update_byte((uint8_t*)EepromTemp, i);
	 RefTemp=eeprom_read_byte((const uint8_t*)EepromTemp);
	 _delay_ms(500);
}

    
	 LCD_setCursor (3,1);
	 lcd_print ("RefTemp: ");
	 LCD_setCursor (11,1);
	 lcdWriteInt(RefTemp,2);
	 LCD_setCursor (14,1);
	 lcddata (0xDF);
	 LCD_setCursor (15,1);
	 lcd_print ("C");
	if (temperature>=RefTemp)
	{
		PORTC &= ~(1 << RELAY_PIN2);
	}
	else
	{
		PORTC |= (1 << RELAY_PIN2);
	}
	if (humidity>=RefHumid)
	{
		PORTC &= ~(1 << RELAY_PIN1);
	}
	else
	{
		PORTC |= (1 << RELAY_PIN1);
	}
}


	    
void EepromRead()
{
	RefHumid=eeprom_read_byte((const uint8_t*)EepromHumid);
	RefTemp=eeprom_read_byte((const uint8_t*)EepromTemp);
}

void GetDHT()
{
	if (dht_gettemperaturehumidity(&temperature, &humidity) != -1)
	{
		char printbuff[8];
		lcd_clear();
		dtostrf(humidity, 3, 2, printbuff);
		LCD_setCursor(0,0);        
		lcd_print("H:");
		LCD_setCursor (2,0);      
		lcd_print(printbuff);
		LCD_setCursor (6,0);
		lcd_print("%");
		dtostrf(temperature, 3, 2, printbuff);
		LCD_setCursor(8,0);       
		lcd_print ("T: ");
		LCD_setCursor (10,0);   
		 lcd_print(printbuff);
		 LCD_setCursor (14,0);
		 lcddata (0xDF);
		 LCD_setCursor(15,0);
		 lcd_print ("C");
		 _delay_ms(100);
	
	}
	else
	{
		lcd_clear();
		LCD_setCursor(0,0);        
		lcd_print("not connected");
	 }

 }
      




int main(void)
{
	lcdinit();
	keypad_init();
	EepromRead();
	LCD_setCursor (0,0);
	lcd_print ("LMU AEI BY BANJI");
	_delay_ms(100);
	
DDRC |= (1 << RELAY_PIN1);
	DDRC |= (1 << RELAY_PIN2);
	PORTC &= ~(1 << RELAY_PIN1);
	PORTC &= ~(1 << RELAY_PIN2);
	lcdinit();
	lcd_clear();

	int8_t temperature = 0;
	int8_t humidity = 0;
	
	
	 while (1)
	{
		
		GetDHT();
       key = keypad_read();
	   switch (key){
	
		case '1':
				LCD_setCursor(0,1);
				lcddata (key);
				_delay_ms(100);
				Temppreset();
			break;
			
			case '#':
				LCD_setCursor(0,1);
				lcddata (key);
				_delay_ms(100);
				Humidpreset();
			break;
			case '2':
			GetDHT();
			LCD_setCursor(0,1);
			lcddata (key);
			if (humidity>=0x0000003C)
			{
				PORTC &= ~(1 << RELAY_PIN1);
			}
			else if(humidity<0x00000032)
			{
				PORTC |= (1 << RELAY_PIN1);
			}
			if (temperature>=0x00000025)
			{
				PORTC &= ~(1 << RELAY_PIN2);
			}
			else
			{
				PORTC |= (1 << RELAY_PIN2);
			}
			break;
		
	       default:
			LCD_setCursor(2,1);
			lcd_print ("Wrong choice");
			break;
	   }	
	   
   }
   
 }		
					
	




