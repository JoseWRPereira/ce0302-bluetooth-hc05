#ifndef EUSART_H
#define EUSART_H

unsigned char rxEUSART( void );
unsigned char rxStatusEUSART( void );
unsigned char rxErrorEUSART( void );
void rxResetErrorEUSART( void );
void txEUSART( unsigned char d );
unsigned char txStatusEUSART( void );
void initEUSART( unsigned long baudRate );

#endif
