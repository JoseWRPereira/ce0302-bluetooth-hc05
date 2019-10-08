/*
 * File:   eusart.c
 * Author: curtaeletronica
 *
 * Created on 2 de Outubro de 2019, 20:25
 * 
 * Objetivo: 
 *      Camada de Abstração de Hardware:
 *          Interface de comunicação Serial EUSART
 *          EUSART = Enhanced Universal Synchronous Asynchronous Receiver Transmitter
 *                   Transmissor Receptor Assíncrono Síncrono Universal Aprimorado
 * 
 * Pinos    |nº     |Conexão
 *  RC6     |25     | TX
 *  RC7     |26     | RX
 * 
 */


//***************** Bibliotecas
#include <xc.h>
#include "config.h"


//***************** Structs
typedef union 
{
    struct 
    {
        unsigned char LO;
        unsigned char HI;
    };
    struct
    {
        unsigned int u16;
    };
} SPBRbits_t;
extern volatile SPBRbits_t SPBRbits @ 0x099;



/****************** Função rxEUSART
 * Entrada: -
 * Retorno: Dado.
 * Objetivo: Ler o valor recebido pelo canal de comunicação serial.
 */
unsigned char rxEUSART( void )
{
    return( RCREG );
}


/****************** Função rxStatusEUSART
 * Entrada: -
 * Retorno: 0 = dado não recebido.
 *          1 = dado recebido.
 * Objetivo: Verificar se algum dado foi recebido pelo canal de comunicação.
 */
unsigned char rxStatusEUSART( void )
{
    return( PIR1bits.RCIF );
}


/****************** Função rxErrorEUSART
 * Entrada: -
 * Retorno: 0 = sem erro.
 *          1 = Framing Error: Erro no pacote de dados.
 *          2 = Overrun Error: Dado recebido de forma sobreposta, ou seja,
 *              um dado chegou antes do anterior ser lido.
 * Objetivo: Verificar se houve erro na recepção de dados.
 */
unsigned char rxErrorEUSART( void )
{
    if( RCSTAbits.FERR )
        return( 1 );
    else if( RCSTAbits.OERR )
        return( 2 );
    else 
        return( 0 );
}


/****************** Procedimento rxResetErrorEUSART
 * Entrada: -
 * Retorno: -
 * Objetivo: Reinicializar/limpar os indicadores(flags) de erro.
 */
void rxResetErrorEUSART( void )
{
    unsigned char aux;
    
    if( RCSTAbits.FERR )
    {
        RCSTAbits.SPEN = 0;
        __delay_ms(100);
        RCSTAbits.SPEN = 1;
    }
    if( RCSTAbits.OERR )
    {
        RCSTAbits.CREN = 0;
        __delay_ms(100);
        RCSTAbits.CREN = 1;
    }
}



/****************** Procedimento txEUSART
 * Entrada: dado.
 * Retorno: -
 * Objetivo: Transmitir um dado pelo canal de comunicação serial.
 */
void txEUSART( unsigned char d )
{
    TXREG = d;
}


/****************** Função txEUSART
 * Entrada: -
 * Retorno: Indicação de dado em transmissão.
 * Objetivo: Indicar que um dado está sendo transmitido.
 */
unsigned char txStatusEUSART( void )
{
    return( !TXSTAbits.TRMT );
}


/****************** Procedimento initEUSAR
 * Entrada: -
 * Retorno: -
 * Objetivo: Inicializar periférico de comunicação serial assíncrona.
 */
void initEUSART( unsigned long baudRate )
{
                            // Calcula valor para registradores que produzem
                            // a taxa de transmissão (Baud rate) de dados desejada.
    SPBRbits.u16 = (unsigned int)( (_XTAL_FREQ / (baudRate<<2)) - 1);

    TXSTAbits.SYNC = 0;     // Modo assíncrono.
    BAUDCTLbits.BRG16 = 1;  // Gerador de BaudRate de 16 bits.
    TXSTAbits.BRGH = 1;     // Seleção de alto BaudRate.
    
    RCSTAbits.SPEN = 1;     // Habilita pinos para TX e RX.
    TXSTAbits.TXEN = 1;     // Inicia transmissão.
    RCSTAbits.CREN = 1;     // Habilita recepção.
    
}
