/*
 * File:   keyboard.c
 * Author: curtaeletronica
 *
 * Created on 3 de Outubro de 2019, 08:43
 * 
 * Objetivo: 
 *      Camada de Abstração de Hardware:
 *          Interface com teclado matricial 4x4.
 * 
 * Pinos    |nº     |Conexão
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

//***************** Structs
typedef union 
{
    struct 
    {
        unsigned rows                   :4;
        unsigned cols                   :4;
    };
} KEYBbits_t;
extern volatile KEYBbits_t KEYBOARDbits @ 0x006;


//***************** Matriz de caracteres do teclado
const char keyboardMatrix[4][4] =   {   {'1','2','3','A'},
                                        {'4','5','6','B'},
                                        {'7','8','9','C'},
                                        {'*','0','#','D'}};

//***************** Vars. Globais (Não acessar de outros arquivos)
unsigned char prevKey = 0;
unsigned char currKey = 0;



/****************** Função previousKey
 * Entrada: -
 * Retorno: Tecla anterior
 * Objetivo: Retorna o valor da tecla pressionada na última varredura do teclado.
 */
unsigned char previousKey( void )
{
    return( prevKey );
}

/****************** Função currentKey
 * Entrada: -
 * Retorno: Tecla atual
 * Objetivo: Retorna o valor da tecla pressionada na atual varredura do teclado.
 */
unsigned char currentKey( void )
{
    return( currKey );
}

/****************** Função keyboardScan
 * Entrada: -
 * Retorno: Tecla atual ou 0.
 * Objetivo: Realiza a varredura do teclado.
 */
unsigned char keyboardScan( void )
{
    unsigned char i, j;
    unsigned char aux = 0;
    
    for( j=0; j<4; j++ )
    {
        KEYBOARDbits.cols = (0x01<<j);
        for( i=0; i<4; i++ )
        {
            if( KEYBOARDbits.rows & (0x01<<i) )
                aux = keyboardMatrix[i][j];
        }
    }
    
    prevKey = currKey;
    currKey = aux;

    return( aux );
}



/****************** Procedimento initKeyboard
 * Entrada: -
 * Retorno: -
 * Objetivo: Inicializa o PORT para acessar o teclado matricial 4x4.
 */
void initKeyboard( void )
{
    PORTB = 0;
    ANSELH  = 0;
    TRISB = 0x0F;
    WPUB = 0x00;
    OPTION_REGbits.nRBPU = 1;

    prevKey = 0;
    currKey = 0;
}
