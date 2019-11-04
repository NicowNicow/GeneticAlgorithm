#include "ESLib.h"
#include <stdio.h>
#include <stdlib.h>

float random_generator(int min, int max) 
{ 
    return((((float)rand()/(float)RAND_MAX)*(float)(max-min+1))+(float)min);
}

float poidsrandom()
{
    int signe=valeurAleatoire()*2;
    int coeff;
    if (signe==0){
        coeff=-1;
    }
    else{
        coeff=1;
    }
    printf("%f\n", random_generator(0,1)*coeff);
    printf("%d\n", coeff);
    return(random_generator(0,1)*coeff);
}
float[3][3] mutation(float[3][3] tableau, int indexi, int indexj)
{
switch ((int)valeurAleatoire()*3)
    {
    case 0:
        tableau[indexi][indexj]=random_generator(0,1);
    case 1:
    {
        float mutated = poidsrandom + tableau[indexi][indexj];
        return (mutated<0?0:(mutated>=1?1:mutated));
    }
    default:
    return tableau[indexi][indexj];
    }
}

void main()
{
    int indexi;
    int indexj;
    indexi=valeurAleatoire()*3;
    indexj=valeurAleatoire()*3;
    printf("%d %d", indexi, indexj);

    float tableau[3][3]={1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5};
    float tableaumute[][];

    for (int i=0;i<=2;i++){
            for (int j=0;j<=2;j++){
                printf("%f \t", tableau[i][j]);
            }
            printf("\n");
    }
    tableaumute=mutation(tableau,indexi,indexj);
    for (int i=0;i<=2;i++){
            for (int j=0;j<=2;j++){
                printf("%f \t", tableau[i][j]);
            }
            printf("\n");
    }
}