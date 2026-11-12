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
        
    for ( int i = 0 ; i < DIM_n ; i++ ) {

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
        op = op >> ( size - DIM_n - DIM_op - 2 * i );
        mask1 = mask1 >> DIM_op;
        mask2 = mask2 >> DIM_op;

        /* adaug fiecare opeartie pe cate o pozitie intr-un sir de caractere */

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

    for ( int i = 0 ; i < DIM_dim ; i++ ) {

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
     * fiecare operand de dimensiune dim, dim care se divide cu 16, intr-un vector 
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

    position = 0;

    /* iterez prin fiecare numar citit si o folosesc o masca pentru a pune 
     * fiecare operand de dimensiune dim, dim care nu se divide cu 16, intr-un vector 
     */

    if ( DIM_SHORT % dim ) {

        int i = 0;

        /* cazul in care dim este mai mic decat 8, 
         * adica jumatate din dimensiunea maxima 
         */

        if ( dim < DIM_BYTE ) {

            while ( i * 2 != n ) {

                int r = 0;

                operanzi[position] = 0;

                /* folosesc o masca pentru a copia fiecare numar de dimensiune dim in vectorul operanzi */

                if ( i == 0 ) {

                    mask = 1 << ( DIM_SHORT - UNIT );

                    for ( int k = 0 ; k < DIM_SHORT / dim ; k++ ) {

                        operanzi[position] = 0;
                        for ( int j = 0 ; j < dim ; j++ ) {

                            operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                            mask = mask >> 1;
                        }

                        operanzi[position] = operanzi[position] >> ( DIM_SHORT % dim + dim * ( DIM_SHORT / dim - k - UNIT ) );
                        position++;
                    }

                    mask = 1 << ( DIM_SHORT % dim - 1 );
                    operanzi[position] = 0;

                    for ( int l = 0 ; l < DIM_SHORT % dim ; l++ ) {

                        operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                        mask = mask >> 1;
                    }

                    operanzi[position] = operanzi[position] << ( dim - ( DIM_SHORT % dim ) );
                }
                else {

                    mask = 1 << ( ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i ) - UNIT );

                    if ( ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i ) >= dim ) {

                        for ( int k = 0 ; k < ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i ) / dim ; k++ ) {

                            for ( int j = 0 ; j < dim ; j++ ) {

                                operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                                mask = mask >> 1;
                            }

                            operanzi[position] = operanzi[position] >> ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i - k * dim );
                            position++;
                        }
                    }
                    else {

                        /* atunci cand din 16 biti raman ( 16 % dim ) biti, 
                         * pun acesti biti pe o pozitie in vector si shiftez numarul, 
                         * astfel incat sa mai adaug in acest numar ( dim - ( 16 % dim ) ) 
                         * din urmatorul numar citit
                         */

                        for ( int j = 0 ; j < ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i ) ; j++ ) {

                            operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                            mask = mask >> 1;
                        }

                        operanzi[position] = operanzi[position] << ( dim - ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i ) );
                    }                    
                }

                i++;

                mask = 1 << ( DIM_SHORT - UNIT );
                unsigned short aux = 0;

                /* adaug cei ( dim - ( 16 % dim ) ) biti cu ajutorul lui aux */

                for ( int w = 0 ; w < dim - ( DIM_SHORT % dim ) * i ; w++ ) {

                    aux = aux | ( readNumbers[i] & mask );
                    mask = mask >> 1;
                }            

                aux = aux >> ( DIM_SHORT - dim + ( DIM_SHORT % dim ) * i - r * dim );
                r++;                
                operanzi[position] = operanzi[position] | aux;                
                position++; 
            }
        }
        /* cazul in care dim este mai mare decat 8, 
         * adica jumatate din dimensiunea maxima 
         */

        else {        

            while ( i != n ) {        

                operanzi[position] = 0;

                /* folosesc o masca pentru a copia fiecare numar de dimensiune dim in vectorul operanzi */

                if ( i == 0 ) {

                    mask = 1 << ( DIM_SHORT - UNIT );

                    for ( int k = 0 ; k < DIM_SHORT / dim ; k++ ) {

                        operanzi[position] = 0;
                        for ( int j = 0 ; j < dim ; j++ ) {

                            operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                            mask = mask >> 1;
                        }

                        operanzi[position] = operanzi[position] >> ( DIM_SHORT % dim );
                        position++;
                    }

                    mask = 1 << ( DIM_SHORT % dim - UNIT );
                    operanzi[position] = 0;

                    for ( int l = 0 ; l < DIM_SHORT % dim ; l++ ) {

                        operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                        mask = mask >> 1;
                    }

                    operanzi[position] = operanzi[position] << ( dim - ( DIM_SHORT % dim ) );
                }
                else {

                    /* atunci cand din 16 biti raman ( 16 % dim ) biti, 
                     * pun acesti biti pe o pozitie in vector si shiftez numarul, 
                     * astfel incat sa mai adaug in acest numar 
                     * ( dim - ( 16 % dim ) ) biti din urmatorul numar citit
                     */
                
                    mask = 1 << ( ( DIM_SHORT % dim ) * ( i + 1 ) -  1 );

                    for ( int k = 0 ; k < DIM_SHORT / dim ; k++ ) {
                    
                        for ( int j = 0 ; j < ( DIM_SHORT % dim ) * ( i + 1 ) ; j++ ) {

                            operanzi[position] = operanzi[position] | ( readNumbers[i] & mask );
                            mask = mask >> 1;
                        }

                        operanzi[position] = operanzi[position] << ( dim - ( DIM_SHORT % dim ) * ( i + 1 ) );                    
                    }
                }

                i++;

                mask = 1 << ( DIM_SHORT - UNIT );
                unsigned short aux = 0;

                /* adaug cei ( dim - ( 16 % dim ) ) biti cu ajutorul lui aux */

                for ( int w = 0 ; w < dim - ( DIM_SHORT % dim ) * i ; w++ ) {

                    aux = aux | ( readNumbers[i] & mask );
                    mask = mask >> 1;
                }

                aux = aux >> ( ( DIM_SHORT % dim ) * ( i + 1 ) );
                operanzi[position] = operanzi[position] | aux;
                position++; 
            
            }
        }
    }    

    int result = operanzi[0];

    /* calculez rezultatul operatiilor */

    for ( int i = 0 ; i < n ; i++ ) {

        if ( operations[i] == '+' ) {

            result = result + operanzi[i + 1];

        } else if ( operations[i] == '-' ) {

            result = result - operanzi[i + 1];

        } else if ( operations[i] == '*' ) {

            result = result * operanzi[i + 1];

        } else if ( operations[i] == '/' ) {

            result = result / operanzi[i + 1];
        }
    }

    /* afisez rezultatul calculat anterior */

    printf ( "%d\n" , result );

    return 0;
}