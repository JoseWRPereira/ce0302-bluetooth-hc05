#ifndef KEYBOARD_H
#define KEYBOARD_H

unsigned char previousKey( void );  // retorna Tecla Anterior.
unsigned char currentKey( void );   // retorna Tecla Atual.
unsigned char keyboardScan( void ); // Varredura do teclado.
void initKeyboard( void );          // Inicializar hw de acesso ao teclado.

#endif
