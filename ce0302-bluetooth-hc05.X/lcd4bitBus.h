/* 
 * File:    lcd4bitBus.h
 * Author:  Curta Eletrônica
 *
 * Created on 27 de Setembro de 2019, 20:33
 */

#ifndef LCD4BITBUS_H
#define	LCD4BITBUS_H

/*
 * **************** Procedimentos de acesso ao LCD
 */
void initLCD( void );
void intTOstr( int ui16, char * str );
void lcd( unsigned char x, unsigned char y, const char * ptr );
void lcdxy( unsigned char x, unsigned char y );
void clearLCD( void );

/*
 * **************** Funções de acesso aos botões da placa do display LCD
 */
char lcdb0( void );
char lcdb1( void );


#endif	/* LCD_H */
