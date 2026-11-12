#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define NR 10
#define DIM_n 3
#define DIM_op 2
#define DIM_dim 4
#define UNIT 1
#define DIM_BYTE 8
#define DIM_SHORT 16

int main () {

    unsigned int inst;
    unsigned int size = DIM_BYTE * sizeof ( inst );

    scanf ( "%u" , &inst );

    unsigned int mask = 1 << ( size - UNIT );
    unsigned int n = 0;

    /* iterez peste peste cei mai semnificativi 3 biti din inst pentru a obtine n */ 
        
    for ( int i = 0 ; i < 3 ; i++ ) {

        n = n | ( inst & mask );
        mask = mask >> 1;
    }
    
    n = n >> ( size - DIM_n );
    n++;

    /* folosesc 2 masti pentru pentru a obtine cele n operatii */

    unsigned int mask1 = 1 << ( size - DIM_n - UNIT );
    unsigned int mask2 = 1 << ( size - DIM_n - 2 * UNIT );
    unsigned int op = 0;

    char operations[MAX];
    
    int k = 0; 

    for ( int i = 0 ; i < n ; i ++ ) {

        op = op | ( inst & mask1 );
        op = op | ( inst & mask2 );
        op = op >> ( size - DIM_n - DIM_op - DIM_op * i );
        mask1 = mask1 >> DIM_op;
        mask2 = mask2 >> DIM_op;

        /* pun fiecare operatie pe cate o pozitie intr-un sir de caractere */

        if ( op == 0 ) {

            operations[k++] = '+';
        } 
        else if ( op == 1 ) {

            operations[k++] = '-';
        }
        else if ( op == 2 ) {

            operations[k++] = '*';
        }
        else if ( op == 3 ) {

            operations[k++] = '/';
        }
    }

    /* folosesc o masca pentru a obtine dim, variabila pe 4 biti */

    mask = 1 << ( size - DIM_dim - DIM_op * n );
    unsigned int dim = 0;

    for ( int i = 0 ; i < 4 ; i++ ) {

        dim = dim | ( inst & mask );
        mask = mask >> 1;
    }
    
    dim = dim >> ( size - DIM_dim - DIM_n - DIM_op * n );
    dim++;

    float x = ( float ) ( 1.0 * ( n + 1 ) * dim ) / DIM_SHORT;
    unsigned int nr = ( ( n + 1 ) * dim ) / DIM_SHORT;

    if ( nr != x ) {

        nr++;
    }

    unsigned short operanzi[NR];
    unsigned short y;
    unsigned short readNumbers[NR]; 

    /* pun numerele citite intr-un vector */

    for ( int i = 0 ; i < nr ; i++ ) {

        scanf ( "%hu" , &y );
        readNumbers[i] = y;    
    }   

    int position = 0;

    /* iterez prin fiecare numar citit si o folosesc o masca pentru a pune 
     * fiecare operand de dimensiune dim intr-un vector 
     */
        
    for ( int i = 0 ; i < nr ; i++ ) {

        if ( dim == DIM_SHORT / DIM_SHORT ) {

            mask = 1 << ( DIM_SHORT - UNIT );

            operanzi[position] = 0;

            for ( k = 0 ; k < n + 1 ; k++ ) {

                operanzi[position] = readNumbers[i] & mask;
                operanzi[position] = operanzi[position] >> ( DIM_SHORT - UNIT - k );
                mask = mask >> 1;
                position++;
            }
        }
        if ( dim != DIM_SHORT && dim != DIM_SHORT / DIM_SHORT ) {

             mask = 1 << ( DIM_SHORT - 1 );
            
            for ( k = 0 ; k < DIM_SHORT / dim ; k++ ) {

                operanzi[position] = 0;

                int j;
                for ( j = 0 ; j < dim ; j++ ) {

                    operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                    mask = mask >> 1;
                }

                operanzi[position] = operanzi[position] >> ( dim * ( DIM_SHORT / dim - k - 1 ) );
                position++;
            }
        }      
        if ( dim == DIM_SHORT ) {

            operanzi[position] = readNumbers[i];
            position++; 
        }
    }

    char tokens[MAX][MAX];
    
    int pos = 0 , pos1 = 0 , pos2 = 0;

    /* pun in sirul de stringuri pe pozitiile pare operanzii */

    for ( pos = 0 ; pos < 2 * n + 1 ; pos += 2 ) {

        sprintf ( tokens[pos] , "%hu" , operanzi[pos1] );
        pos1++;
    }

    /* pun in sirul de stringuri pe pozitiile impare operatiile */

    for ( pos = 1 ; pos < 2 * n ; pos += 2 ) {

        sprintf ( tokens[pos] , "%c" , operations[pos2] );
        pos2++;
    }

    int result = 0;

    /* verificare caz in care citesc doar 2 numere */

    if ( n == 1 ) {

        int n1 = atoi ( tokens[0] );
        int n2 = atoi ( tokens[2] );

        if ( strcmp ( tokens[1] , "+" ) == 0 ) {

            result = n1 + n2;

        } else if ( strcmp ( tokens[1] , "-" ) == 0 ) {

            result = n1 - n2;

        } else if ( strcmp ( tokens[1] , "*" ) == 0 ) {

            result = n1 * n2;

        } else if ( strcmp ( tokens[1] , "/" ) == 0 ) {

            result = n1 / n2;
        }

        printf ( "%d\n" , result );

        return 0;
    }

    /* cazul general 
     * pentru operatiile de gradul 2 calculez rezultatul operatiei respective
     * pe pozitia primului operand din cadrul acestei operatii, pun "0"
     * pe pozitia pe care se afla operatia, pun "+"
     * iar pe pozitia celui de-al doilea operand, pun rezultatul operatiei     
     */

    for ( pos = 1 ; pos < 2 * n ; pos += 2 ) {

        if ( strcmp ( tokens[pos] , "*" ) == 0 ) {

            int n1 = atoi ( tokens[pos - 1] );
            int n2 = atoi ( tokens[pos + 1] );
            int aux = n1 * n2;
            char buffer[MAX];
            sprintf ( buffer , "%d" , aux );
            strcpy ( tokens[pos - 1] , "0" );
            strcpy ( tokens[pos] , "+" );
            strcpy ( tokens[pos + 1] , buffer );

        } else if ( strcmp ( tokens[pos] , "/" ) == 0 ) {

            int n1 = atoi ( tokens[pos - 1] );
            int n2 = atoi ( tokens[pos + 1] );
            int aux = n1 / n2;
            char buffer[MAX];
            sprintf ( buffer , "%d" , aux );
            strcpy ( tokens[pos] , "+" );
            strcpy ( tokens[pos + 1] , buffer );
        }
    }

    result = atoi ( tokens[0] );

    /* acum, in sirul de stringuri am doar numere si operatii de gradul 1
     * si calculez rezultatul operatiilor */

    for ( pos = 1 ; pos < 2 * n ; pos++ ) {

        int nr = atoi ( tokens[pos + 1] );

        if ( strcmp ( tokens[pos] , "+" ) == 0 ) {

            result = result + nr;;

        } else if ( strcmp ( tokens[pos] , "-" ) == 0 ) {

            result = result - nr;
        } 
    }

    /* afisez rezultatul calculat mai sus */
     
    printf ( "%d\n" , result );

    return 0;
}