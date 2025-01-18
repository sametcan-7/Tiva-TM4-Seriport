/*
 * lcd.h
 *
 *  Created on: 6 Ara 2024
 *      Author: USER
 */

#ifndef LCD_H_
#define LCD_H_

#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7


void LCD_init(void);

void LCD_Temizle(void);

void LCD_Komut(unsigned char c);

void LCD_Karakter(unsigned char d);

void LCD_Cursor(char x, char y);

void LCD_Fonksiyon(unsigned char,unsigned char,unsigned char);





#endif /* LCD_H_ */
