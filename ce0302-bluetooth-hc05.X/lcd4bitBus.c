/*
 * File:   lcd4bitBus.c
 * Author: Curta Eletrônica
 *
 * Created on 27 de Setembro de 2019, 20:33
 * 
 * Objetivo: 
 *      Camada de Abstração de Hardware:
 *          Interface com display LCD 16x2 caracteres.
 * 
 * Pinos    |nº     |Conexão
 *  RD0     |19     | LCD_D4
 *  RD1     |20     | LCD_D5
 *  RD2     |21     | LCD_D6
 *  RD3     |22     | LCD_D7
 *  RD4     |27     | LCD_RS
 *  RD5     |28     | LCD_EN
 *  RD6     |29     | LCD_B0 (Botão 0)
 *  RD7     |30     | LCD_B1 (Botão 1)
 *
 */

//***************** Bibliotecas
#include <xc.h>
#include "config.h"


//***************** Interface com PORTs/Pinos
#define LCD_BUS         LCDbits.BUS
#define LCD_RS          LCDbits.RS
#define LCD_EN          LCDbits.EN
#define LCD_B0          LCDbits.B0
#define LCD_B1          LCDbits.B1
#define LCD_ADDRS       0x008
#define LCD_ROWS        2
#define LCD_COLS        16


//***************** CMD LCD: Definicao de Comandos ao LCD 
#define LCD_CLEAR_DISPLAY           0x01

#define LCD_RETURN_HOME             0x02

#define LCD_ENTRY_MODE_SET          0x04
#define LCD_EMS_CURSOR_RIGHT        0x02
#define LCD_EMS_CURSOR_LEFT         0x00
#define LCD_EMS_SHIFT_DISPLAY       0x01

#define LCD_DISPLAY_CONTROL         0x08
#define LCD_DC_DISPLAY_ON           0x04
#define LCD_DC_DISPLAY_OFF          0x00
#define LCD_DC_CURSOR_ON            0x02
#define LCD_DC_CURSOR_OFF           0x00
#define LCD_DC_BLINK_ON             0x01
#define LCD_DC_BLINK_OFF            0x00

#define LCD_CURSOR_SHIFT            0x10
#define LCD_CS_LEFT                 0x00
#define LCD_CS_RIGHT                0x04

#define LCD_DISPLAY_SHIFT           0x10
#define LCD_DS_LEFT                 0x08
#define LCD_DS_RIGHt                0x0C

#define LCD_FUNCTION_SET            0x20
#define LCD_FS_DATA_LENGTH_8        0x10
#define LCD_FS_DATA_LENGTH_4        0x00
#define LCD_FS_LINE_NUMBER_1        0x00
#define LCD_FS_LINE_NUMBER_2        0x08
#define LCD_FS_DOTS_FORMAT_5x8      0x00
#define LCD_FS_DOTS_FORMAT_5x11     0x04

#define LCD_SET_CGRAM_ADDRS( adrs ) (0x40+(adrs & 0x3F))

#define LCD_SET_DDRAM_ADDRS( adrs ) (0x80+(adrs & 0x7F))
#define LCD_ADDR_LINE_0             0x00
#define LCD_ADDR_LINE_1             0x40



//***************** Structs
typedef union 
{
    struct 
    {
        unsigned char LO :4;
        unsigned char HI :4;
    };
    unsigned char HILO;
} REGnibble_t;


typedef union 
{
    struct 
    {
        unsigned char BUS :4;
        unsigned char RS  :1;
        unsigned char EN  :1;
        unsigned char B0  :1;
        unsigned char B1  :1;
    };
} LCDbits_t;

volatile LCDbits_t LCDbits __at(LCD_ADDRS);


/****************** Procedimento lcdcmd
 * Entrada: Comando a ser enviado ao Display LCD.
 * Retorno: -
 * Objetivo: Enviar um comando (cmd) para o display LCD de acordo com seu protocolo.
 */
void lcdcmd( unsigned char cmd )
{
    volatile REGnibble_t nibble;
    
    nibble.HILO = cmd;
    LCD_RS = 0;
    LCD_BUS = nibble.HI;

    LCD_EN = 0;
    if( cmd == LCD_CLEAR_DISPLAY || cmd == LCD_RETURN_HOME )
        __delay_ms( 2 );
    else
        __delay_us( 40 );
    LCD_EN = 1;
    

    if( cmd != (LCD_FUNCTION_SET | LCD_FS_DATA_LENGTH_4) )
    {
        LCD_RS = 0;
        LCD_BUS = nibble.LO;
        LCD_EN = 0;
        if( cmd == LCD_CLEAR_DISPLAY || cmd == LCD_RETURN_HOME )
            __delay_ms( 2 );
        else
            __delay_us( 40 );
        LCD_EN = 1;
    }
}

/****************** Procedimento lcd4dat
 * Entrada: Caracter alfanumérico a ser impresso na tela do Display LCD.
 * Retorno: -
 * Objetivo: Enviar um dado (dat) para o display LCD de acordo com seu protocolo.
 */
void lcddat( unsigned char dat )
{
    volatile REGnibble_t nibble;
    nibble.HILO = dat;

    LCD_RS = 1;
    LCD_BUS = nibble.HI;
    LCD_EN = 0;
    __delay_us( 40 );
    LCD_EN = 1;

    LCD_RS = 1;
    LCD_BUS = nibble.LO;
    LCD_EN = 0;
    __delay_us( 40 );
    LCD_EN = 1;
}

/****************** Procedimento lcdxy
 * Entrada: x: Posição horizontal para posicionar o cursor.
 *          y: Posição vertical para posicionar o cursor.
 * Retorno: -
 * Objetivo:Posiciona o cursor nas coordenadas x e y do display LCD. 
 *          O caracter do canto superior esquerdo tem coordenadas (0,0).
 */
void lcdxy( unsigned char x, unsigned char y )
{
    lcdcmd( LCD_SET_DDRAM_ADDRS( (LCD_ADDR_LINE_1 * y) + (x + LCD_ADDR_LINE_0) ) );
}

/****************** Procedimento lcd
 * Entrada: x: Posição horizontal para posicionar o cursor.
 *          y: Posição vertical para posicionar o cursor.
 *          ptr: Endereço da mensagem / string(conjunto de caracteres) a ser 
 *               exibida no Display LCD.
 * Retorno: -
 * Objetivo:Exibe a string  na tela do Display LCD, 
 *          nas posições 'x' e 'y'.
 */
void lcd( unsigned char x, unsigned char y, const char * ptr )
{
    lcdxy(x,y);
    while( *ptr )
        lcddat( *ptr++ );
}



/****************** Procedimento intTOstr
 * Entrada: ui16: Número a ser convertido em String.
 *          str:  Endereço que recebe o número convertido em String.
 * Retorno: -
 * Objetivo: Converter um número em string.
 */
void intTOstr( int ui16, char * str )
{
    for(int div=10000; div>=1; div/=10 )
    {
        *str = (ui16 / div) + '0';
        ui16 = ui16 % div;
        ++str;
    }
    *str = 0;
}



/****************** Função lcdb0
 * Entrada: -
 * Retorno: Estado lógico da entrada Botão 0 associado ao LCD.
 * Objetivo: Entrada do Botão 0.
 */
char lcdb0( void )
{
    return( LCD_B0 );
}


/****************** Função lcdb0
 * Entrada: -
 * Retorno: Estado lógico da entrada Botão 1 associado ao LCD.
 * Objetivo: Entrada do Botão 1.
 */
char lcdb1( void )
{
    return( LCD_B1 );
}


/****************** Procedimento initlcd
 * Entrada: -
 * Retorno: -
 * Objetivo: Limpa display LCD.
 */
void clearLCD( void )
{
    lcdcmd( LCD_CLEAR_DISPLAY );
}



/****************** Procedimento initlcd
 * Entrada: -
 * Retorno: -
 * Objetivo: Inicializar interface com display LCD.
 */
void initLCD( void )
{
    unsigned char i,j;
    LCD_BUS = 0;
    LCD_RS = 0;
    LCD_EN = 1;
    TRISD = 0xC0;

    __delay_ms(1000);
    lcdcmd(LCD_FUNCTION_SET | LCD_FS_DATA_LENGTH_4 );
    __delay_ms(1000/4);
    lcdcmd(LCD_FUNCTION_SET | LCD_FS_LINE_NUMBER_2 );
    __delay_ms(1000/4);
    lcdcmd(LCD_DISPLAY_CONTROL | LCD_DC_DISPLAY_ON );
    __delay_ms(1000/4);

    for( j=0; j<LCD_ROWS; j++ )
    {
        for( i=0; i<LCD_COLS; i++ )
        {
            lcdxy(i,j); 
            lcddat( 0xFF );
            __delay_ms(1000/(LCD_ROWS*LCD_COLS));
        }
    }
    
    __delay_ms(1000);
    lcdcmd( LCD_CLEAR_DISPLAY );
    lcdcmd( LCD_RETURN_HOME );


}
