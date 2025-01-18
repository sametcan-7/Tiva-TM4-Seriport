/*
 * lcd.c
 *
 *  Created on: 6 Ara 2024
 *      Author: USER
 */

#include <stdbool.h>
#include <stdint.h>
#include <lcd.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "string.h"

void LCD_init()
{

        SysCtlPeripheralEnable(LCDPORTENABLE);
        GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, RS,  0x00 );

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x20 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(500000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);


        LCD_Komut(0x0C); //LCD calistir
        LCD_Komut(0x04); //Giris kipi sec
        LCD_Komut(0x10);//Kursor ayarý
        LCD_Temizle(); //Ekraný temizle
        LCD_Fonksiyon(4, 2, 7);
}

void LCD_Komut(unsigned char c)
{

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

}

void LCD_Karakter(unsigned char d)
{

        GPIOPinWrite(LCDPORT, RS, 0x01);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );

        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );

        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);
        SysCtlDelay(50000);

}


void LCD_Cursor(char x, char y)
{
    if(x==1)
        LCD_Komut(0x80+((y-1)%16));
    else
        LCD_Komut(0xC0+((y-1)%16));
}

void LCD_Temizle(void)
{

        LCD_Komut(0x01);
        SysCtlDelay(10);

}

void LCD_Fonksiyon(unsigned char DL,unsigned char N,unsigned char F)
{
    if(DL==4 && N==1 && F==7)
    {
        LCD_Komut(0x20);
    }

    else if(DL==4 && N==1 && F==10)
    {
        LCD_Komut(0x24);
    }

    else if(DL==4 && N==2 && F==7)
    {
        LCD_Komut(0x28);
    }

    else if(DL==4 && N==2 && F==10)
    {
        LCD_Komut(0x2c);
    }

    else if(DL==8 && N==1 && F==7)
    {
        LCD_Komut(0x30);
    }

    else if(DL==8 && N==1 && F==10)
    {
        LCD_Komut(0x34);
    }

    else if(DL==8 && N==2 && F==7)
    {
        LCD_Komut(0x38);
    }

    else if(DL==8 && N==2 && F==10)
    {
        LCD_Komut(0x3c);
    }

}


