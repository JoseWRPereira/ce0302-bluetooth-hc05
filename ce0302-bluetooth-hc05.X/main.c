/*
 * File:   main.c
 * Author: curtaeletronica
 *
 * Created on 08 de Outubro de 2019, 10:45
 * 
 * IDE:         MPLAB X IDE v3.15
 * Compiler:    XC8 v1.45
 * OS:          Deepin 15.11 X86_64
 * Kernel:      4.15.0-30deepin-generic
 * 
 * Objetivo: 
 *      Produzir comunicação serial via módulo Bluetooth HC-05.
 * 
 * Pinos    |nº     |Conexão
 *  VDD     |11,32  | Alimentação (Vcc/+5V)
 *  VSS     |12,31  | Alimentação (GND/0V)
 *  RD0     |19     | LCD_D4
 *  RD1     |20     | LCD_D5
 *  RD2     |21     | LCD_D6
 *  RD3     |22     | LCD_D7
 *  RC6     |25     | TX
 *  RC7     |26     | RX
 *  RD4     |27     | LCD_RS
 *  RD5     |28     | LCD_EN
 *  RD6     |29     | LCD_B0 (Botão 0)
 *  RD7     |30     | LCD_B1 (Botão 1)
 *  RB0     |33     | L1 : 123A
 *  RB1     |34     | L2 : 456B
 *  RB2     |35     | L3 : 789C
 *  RB3     |36     | L4 : *0#D
 *  RB4     |37     | C1 : 147*
 *  RB5     |38     | C2 : 2580
 *  RB6     |39     | C3 : 369#
 *  RB7     |40     | C4 : ABCD
 * 
*/

//***************** Bibliotecas
#include <xc.h>
#include "config.h"
#include "lcd4bitBus.h"
#include "keyboard.h"
#include "eusart.h"
#include "util.h"


void interrupt geral( void )
{
    INTCONbits.GIE = 0;

    if( PIE1bits.RCIE && PIR1bits.RCIF )
    {
        interrupt_EUSART_RX();
    }
    
    INTCONbits.GIE = 1;
}




//***************** Programa Principal
void main(void)
{
    unsigned char vetor[33];
    unsigned char i = 0;
    unsigned char atcmd = 0;
    initLCD();
    initKeyboard();
    initEUSART(9600);
    
    lcd(0,0, "Curta Eletronica");
    
    while( 1 )                      // Laço de repetição infinita.
    {
        keyboardScan();
        
        if( currentKey() && !previousKey() )
        {
            switch( currentKey() )
            {
                case '0': 
                            clearLCD();
                            lcd(0,0,"     AT:");
                            atcmd = currentKey();
                            break;
                case '1': 
                            clearLCD();
                            lcd(0,0,"VERSION:");
                            atcmd = currentKey();
                            break;
                case '2': 
                            clearLCD();
                            lcd(0,0,"   ADDR:");
                            atcmd = currentKey();
                            break;
                case '3': 
                            clearLCD();
                            lcd(0,0,"bt NAME:");
                            atcmd = currentKey();
                            break;
                case 'A': 
                            clearLCD();
                            lcd(0,0,"setNAME:cp-7/9");
                            atcmd = currentKey();
                            break;
                case 'B': 
                            clearLCD();
                            lcd(0,0,"setNAME:cp-5/9");
                            atcmd = currentKey();
                            break;
                case 'C': 
                            clearLCD();
                            lcd(0,0,"setNAME:cp-3/9");
                            atcmd = currentKey();
                            break;
                case 'D': 
                            clearLCD();
                            lcd(0,0,"setNAME:cp-1/9");
                            atcmd = currentKey();
                            break;
                case '4': 
                            clearLCD();
                            lcd(0,0," PASSWD:");
                            atcmd = currentKey();
                            break;
                case '*': 
                            clearLCD();
                            lcd(0,0,"  RESET:");
                            atcmd = currentKey();
                            break;
                case '#':
                            for( i=0; i<33; i++ )
                                vetor[i] = 0;
                            i = 0;

                            switch( atcmd )
                            {
                                case '0': wrEUSART("AT\r\n");               break;
                                case '1': wrEUSART("AT+VERSION?\r\n");      break;
                                case '2': wrEUSART("AT+ADDR?\r\n");         break;
                                case '3': wrEUSART("AT+NAME?\r\n");         break;
                                case 'A': wrEUSART("AT+NAME=A16-7/9\r\n");  break;
                                case 'B': wrEUSART("AT+NAME=A16-5/9\r\n");  break;
                                case 'C': wrEUSART("AT+NAME=A16-3/9\r\n");  break;
                                case 'D': wrEUSART("AT+NAME=A16-1/9\r\n");  break;
                                case '4': wrEUSART("AT+PSWD?\r\n");         break;
                                case '*': wrEUSART("AT+RESET\r\n");         break;
                            }
                            break;
            }
        }

        switch( errorRxEUSART() )
        {
            case 2: lcd(0,1, "Overrun Error");
                    resetErrorRxEUSART();
                    break;
            case 1: lcd(0,1, "Framing Error");
                    resetErrorRxEUSART();
                    break;
            default:
            case 0: 
                    if( statusRxEUSART() )
                    {
                        vetor[i] = mask( rxEUSART(), ' ', '~');
                        i = ++i % 33;
                        vetor[i] = 0;
                        lcd(8,0, vetor );
                        lcd(0,1, &vetor[8] );
                    }
                    break;
        }
    }
    return;
}
//***************** Fim do Programa Principal
