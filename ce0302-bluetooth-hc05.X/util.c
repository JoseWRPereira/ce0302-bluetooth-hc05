/*
 * File:   util.c
 * Author: curtaeletronica
 *
 * Created on 8 de Outubro de 2019, 12:58
 */


#include <xc.h>

/****************** Fun��o mask
 * Entrada: n = elemento a ser verificado.
 *          first = Primeiro elemento aceit�vel.
 *          last = �ltimo elemento aceit�vel.
 * Retorno: Se n estiver contido no intervalo entre fist e last, inclusive, 
 *          esse valor � retornado, sen�o � retornado o valor zero.
 * Objetivo: Filtra apenas os caracteres que estiverem entre limites dados.
 */
unsigned char mask( unsigned char n, unsigned char first, unsigned char last )
{
    if( n >= first && n <= last )
        return( n );
    else
        return( 0 );
}
