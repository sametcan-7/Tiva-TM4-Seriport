#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/uart.c"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/timer.c"
#include "driverlib/interrupt.h"
#include "lcd.h"
#include "driverlib/adc.h"

//void timerIntSet(void);
void saatiGonder(void);
void timer0Int(void);
void uart_ayari();
void diger_ayar();
void serikesme();
void ADC_init();
void ADCdegergönder(deger);
void Lcdmetinyaz();

int saniye_birler;
int saniye_onlar;
int dakika_birler;
int dakika_onlar;
int saat_birler;
int saat_onlar;

int receivedData = 0;
char receivedClock[8];
char saatVerileri[9];

char receivedText[17];
int receivedChar = 0;
int metingeldimi=0;

uint32_t temp[1];

void main(){
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    LCD_init();
//    timerIntSet();
    IntMasterEnable();
    uart_ayari();
    diger_ayar();
    ADC_init();

    while(1){
        if(ADCIntStatus(ADC0_BASE,3,false))
               {
                   ADCIntClear(ADC0_BASE, 3);
                   ADCSequenceDataGet(ADC0_BASE, 3, temp);
                   uint32_t t=temp[0];
                   ADCdegergönder(t);



               }

    }

}

void diger_ayar(){
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT);
    UARTIntClear(UART0_BASE, UART_INT_RX|UART_INT_RT);
    UARTIntRegister(UART0_BASE, serikesme);

}

void uart_ayari(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // pin ayarlarý
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // UART0 yapýlandýr
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, // Baud rate: 115200
                        (UART_CONFIG_WLEN_8 |               // 8-bit veri uzunluðu
                         UART_CONFIG_STOP_ONE |             // 1 stop biti
                         UART_CONFIG_PAR_NONE));

}

void serikesme(){
    char c= UARTCharGet(UART0_BASE);
    if(receivedData==0)
    {
        if(c=='%')
        {
            receivedClock[receivedData++]=c;
        }
    }
    else if (receivedData < 9)
    {
        receivedClock[receivedData++]=c;
    }
    if(receivedData==9)
    {
        saniye_birler=receivedClock[8]-'0';
        saniye_onlar=receivedClock[7]-'0';
        dakika_birler=receivedClock[5]-'0';
        dakika_onlar=receivedClock[4]-'0';
        saat_birler=receivedClock[2]-'0';
        saat_onlar=receivedClock[1]-'0';
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

        TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);
        TimerEnable(TIMER0_BASE, TIMER_A);

        IntMasterEnable();
        IntEnable(INT_TIMER0A);

        TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntRegister(TIMER0_BASE, TIMER_A, timer0Int);

        receivedData=0;
    }

    if(c=='{')
    {
        metingeldimi=1;
        receivedChar=0;
    }
    if(c=='}')
    {
        metingeldimi=0;

        while(receivedChar<17)
        {
            receivedText[receivedChar++]=' ';
        }
        receivedText[receivedChar]='\0';
        Lcdmetinyaz();
        return;
    }
    if(metingeldimi)
    {
            receivedText[receivedChar++]=c;
        return;
    }
}
//void timerIntSet(void)
//{
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
//
//    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
//    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);
//    TimerEnable(TIMER0_BASE, TIMER_A);
//
//    IntMasterEnable();
//    IntEnable(INT_TIMER0A);
//
//    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
//    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//    TimerIntRegister(TIMER0_BASE, TIMER_A, timer0Int);
//
//}
void timer0Int(void)
{

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if(saniye_birler>=0)
    {
        LCD_Cursor(1, 16);
        LCD_Karakter('0'+saniye_birler);

        LCD_Cursor(1, 15);
        LCD_Karakter('0'+saniye_onlar);

        LCD_Cursor(1, 14);
        LCD_Karakter(':');

        LCD_Cursor(1, 13);
        LCD_Karakter('0'+dakika_birler);

        LCD_Cursor(1, 12);
        LCD_Karakter('0'+dakika_onlar);

        LCD_Cursor(1, 11);
        LCD_Karakter(':');

        LCD_Cursor(1, 10);
        LCD_Karakter('0'+saat_birler);

        LCD_Cursor(1, 9);
        LCD_Karakter('0'+saat_onlar);

        saniye_birler++;
    }
        if(saniye_birler > 9)
    {
        saniye_birler=0;
        saniye_onlar++;
    }

    if (saniye_onlar > 5)
    {
        saniye_onlar=0;
        dakika_birler++;
    }
    if (dakika_birler > 9)
        {
            dakika_birler=0;
            dakika_onlar++;
        }
    if (dakika_onlar > 5)
        {
            dakika_onlar=0;
            saat_birler++;
        }
    if(saat_birler > 4)
        {
            saat_birler=0;
            saat_onlar++;
        }
    if(saat_onlar==2 & saat_birler==4)
        {
            saat_birler=0;
            saat_onlar=0;
        }

    char saatVerileri[9] = {'*','0'+saat_onlar, '0'+saat_birler, ':', '0'+dakika_onlar, '0'+dakika_birler, ':', '0'+saniye_onlar,
                            '0'+saniye_birler};
    int i;
    for( i=0; i<9; i++)
    {
        UARTCharPut(UART0_BASE, saatVerileri[i]);
    }
    UARTCharPut(UART0_BASE, '\n');
    ADCProcessorTrigger(ADC0_BASE, 3);
}
void ADC_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); //adc0 ve seq 3 seçildi
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);// seq3te sadece 1 tane baðlantý var                                                                           //ona da ch0 baðlandý
    ADCSequenceEnable(ADC0_BASE, 3);
}
void ADCdegergönder(deger)
{
    char hane3;
    char hane2;
    char hane1;
    char hane0;
    if (deger < 10) {
          hane3 = 0; hane2 = 0; hane1 = 0; hane0 = deger;
      } else if (deger < 100) {
          hane3 = 0; hane2 = 0; hane0 = (deger % 10); hane1 = (deger - hane0) / 10;
      } else if (deger < 1000) {
          hane3 = 0; hane2 = (deger % 100) / 10; hane1 = ((deger - hane2 * 10 - hane0 * 1) / 100);
      } else {
          hane0 = (deger % 10);
          hane1 = ((deger - hane0) / 10) % 10;
          hane2 = ((deger - 10 * hane1 - 1 * hane0) % 100);
          hane3 = (deger - 100 * hane2 - 10 * hane1 - 1 * hane0) / 1000;
      }
    UARTCharPut(UART0_BASE, '[');
    UARTCharPut(UART0_BASE, hane3+'0');
    UARTCharPut(UART0_BASE, hane2+'0');
    UARTCharPut(UART0_BASE, hane1+'0');
    UARTCharPut(UART0_BASE, hane0+'0');
    UARTCharPut(UART0_BASE, '\n');
    LCD_Cursor(1, 4);
    LCD_Karakter(hane0+'0');
    LCD_Cursor(1, 3);
    LCD_Karakter(hane1+'0');
    LCD_Cursor(1, 2);
    LCD_Karakter(hane2+'0');
    LCD_Cursor(1, 1);
    LCD_Karakter(hane3+'0');
}
void Lcdmetinyaz()
{
    int j;
    for(j=1;j<17;j++)
    {
        LCD_Cursor(2, j);
        LCD_Karakter(receivedText[j]);
    }


}

