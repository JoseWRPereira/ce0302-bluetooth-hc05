#ifndef EUSART_H
#define EUSART_H

unsigned char rxEUSART( void );
unsigned char statusRxEUSART( void );
unsigned char errorRxEUSART( void );
void resetErrorRxEUSART( void );
void txEUSART( unsigned char d );
unsigned char statusTxEUSART( void );
void wrEUSART( const char * str );
void initEUSART( unsigned long baudRate );
void interrupt_EUSART_RX( void );

#endif
