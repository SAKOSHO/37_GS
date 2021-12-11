/* 
 * File:   main.c
 * Author: ���X����
 *
 * Created on 2021/11/03, 14:18
 * Discription�@�n��ǂ�͋[�����v�Z�@
 * 
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"

// CONFIG1
#pragma config FOSC  = HS       // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE  = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP    = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD   = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO  = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP   = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT   = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 20000000

/*--Prtotype--*/
void CONFIG();                                                                  //�����ݒ�
void timer_init();                                                             //�^�C�}�[�̏����ݒ�
void interrupt timer();                                                         //�^�C�}�[�̊��荞�ݏ���
char BtoD(char data);                                                           //2�i����10�i���ɕϊ�

/*--Grobal Variables--*/
volatile char rx_data;                                                         //��M�f�[�^�̎󂯓����
volatile char *fram_data;                                                       //fram�p�̔�
volatile char rx_status;                                                        //��M�X�e�[�^�X�p�̔�
volatile char rx_int;                                                           //��M�t���O�m�F�p�̔�
volatile char data[8] = {2, 2, 2, 2, 2, 2, 2, 2};                               //HK�f�[�^
volatile char size;                                                             //�f�[�^��
volatile int cnt = 0;                                                           //�R���y�A�}�b�`�p�̕ϐ�

void main(void)
{   
    int i = 1, j = 0;
    
    CONFIG();                                                                   //�����ݒ�
    __delay_ms(100);
    
    T1CONbits.TMR1ON = 1;                                                       //�^�C�}�[�J�n
    
    while(1)
    {            
        while(!RCIF);
        RCIF = 0;
        RCREG = 0;
        
            __delay_ms(500);
            
            switch(i)
            {
                case 1:
                        putch('3');
                    break;
                case 2:
                        putch('4');
                    break;
                default:
                        TXREG = '1';
                    break;
            }

            
            i++;

            if(i > 2) i = 1;
    }
}

void CONFIG()
{
    OSCCON = 0b01101000;
    ANSEL  = 0b00000000;
    ANSELH = 0b00000000;
    TRISB  = 0b00000000;
    TRISC  = 0b00000000;
    PORTB  = 0b00000000;
    PORTC  = 0b00000000;
    
    usart_init();
    timer_init();
    __delay_ms(100);
}

void timer_init()
{
    T1CON = 0b00110000;
    TMR1H = 0b00000000;
    TMR1L = 0b00000000;
    PIE1bits.TMR1IE = 0;
    PIR1bits.TMR1IF = 0;
}

void interrupt timer()
{
    PIR1bits.TMR1IF = 0;
    TMR1H = 0b00000000;
    TMR1L = 0b00000000;
    
    cnt++;
}

char BtoD(char data)
{
    char  binary;
    char decimal = 0;
    char bas = 1;
            
    binary = data & 0b00001111;
    
    while(binary>0)
    {
        decimal = decimal + (binary % 10) * bas;
        binary = binary / 10;
        bas = bas * 2;
    }
    
    return decimal;
}