#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define DIM_n 3
#define DIM_op 2
#define DIM_dim 4
#define UNIT 1
#define DIM_BYTE 8

int main() {

    unsigned int inst;
    unsigned int size = DIM_BYTE * sizeof (inst);
    
    scanf ("%u", &inst);

    unsigned int mask = 1 << ( size - UNIT );
    unsigned int n = 0;

    /* iterez peste peste cei mai semnificativi 3 biti din inst 
     * pentru a obtine n 
     */ 

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

    /* afisez n */

    printf ( "%u " , n );

    /* afisez operatiile */

    for ( k = 0 ; k < n ; k++ ) {

        printf ( "%c " , operations[k] );        
    }

    /* afisez dim */

    printf ( "%u" , dim );

    printf ( "\n" );

    return 0;
}