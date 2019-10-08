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


//***************** Programa Principal
void main(void)
{
    unsigned char vetor[17] = "                ";
    unsigned char i = 0;
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
                case '*': 
                            for( unsigned char j=0; j<16; j++ )
                                vetor[j] = ' ';
                            i = 0;
                            lcd(0,1, vetor );             
                            break;

                default :   
                            txEUSART(  currentKey() );    
                            break;
            }
        }

        if(rxStatusEUSART() )
        {
            switch( rxErrorEUSART() )
            {
                case 2: lcd(0,1, "Overrun Error");
                        rxResetErrorEUSART();
                        break;
                case 1: lcd(0,1, "Framing Error");
                        rxResetErrorEUSART();
                        break;
                default:
                case 0: 
                        vetor[i] = rxEUSART();
                        i = ++i % 16;
                        lcd(0,1, vetor );
                        break;
            }
        }
    }
    return;
}
//***************** Fim do Programa Principal
