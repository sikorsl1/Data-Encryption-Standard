#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "keys.h"

int *toBit8(int l){ //zamienia liczby na ciagi zer i jednek
    int count = 7;
    int *tab = malloc(8*sizeof(int));
    while(l!=0){
        tab[count] = l%2;
        l = l/2;
        count--;
    }
    while(count>=0){
        tab[count] = 0;
        count--;
    }
    return tab;
}

int *keyRand(){ //genrowanie 8 znakowego klucza
    int random, i;
    time_t tt;
    random = time(&tt);
    srand(random);
    int *tab = malloc(8*sizeof(int));
    for(i=0;i<8;i++){
        tab[i] = rand()%256;
    }
    return tab;
}

int *vectorShift(int *tabb, int n){  //przesuniecie bitowe
    int tmp[28];
    int i;
    for(i=0;i<28;i++){
        tmp[i]=tabb[i];
    }
    for(i=0;i<26;i++){
        tabb[i]=tmp[i+n];
    }
    if(n!=1){
        tabb[26] = tmp[0];
        tabb[27] = tmp[1];
    }
    else{
        tabb[26] = tmp[27];
        tabb[27] = tmp[0];
    }
    return tabb;
}

void keyGenerator(){
    int Vi[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};  //okreslenie przesuniecia dla konkretnych kluczy
    int i,j;
    int *tab = keyRand();
    int *tmptab = malloc(8*sizeof(int));
    int K[8][8];
    for(i=0;i<8;i++){
            tmptab = toBit8(tab[i]);
            for(j=0;j<8;j++){
                K[i][j] = tmptab[j];
            }
    }
    free(tab);
    free(tmptab);
    int PC1[56] = {56,48,40,32,24,16,8,0,57,49,41,33,25,19,
                   9,1,58,50,42,34,26,18,10,2,59,51,43,35,
                   62,54,46,38,30,22,14,6,61,53,45,37,29,21,
                   13,5,60,52,44,36,28,20,12,4,27,19,11,3};
    int PC2[48] = {13,16,10,23,0,4,2,27,14,5,20,9,
                   22,18,11,3,25,7,15,6,26,19,12,1,
                   40,51,30,36,46,54,29,39,50,44,32,47,
                   43,48,38,55,33,52,45,41,49,35,28,31};
    int T[64];
    for(i=0; i<8; i++){
        for(j=0;j<8;j++){
            T[i*8+j] = K[i][j];
        }
    }
    int C[28];
    int D[28];
    for(i=0; i<56; i++){
        if(i<28)
            C[i] = T[PC1[i]];
        else
            D[i-28] = T[PC1[i]];
    } //dwa ciagi po 28 bitow po pierwszej permutacji
    FILE *fpl_keys = fopen("klucze.txt", "w");
    int Ki[48];
    int Ktmp[56];
    for(i=1;i<=16;i++){//16 rund generowania kluczy
        vectorShift(C,Vi[i-1]);
        vectorShift(D,Vi[i-1]);
        for(j=0; j<56; j++){
            if(j<28)
                Ktmp[j] = C[j];
            else
                Ktmp[j] = D[j-28];
        }
        for(j=0;j<48;j++){
            Ki[j] = Ktmp[PC2[j]];
            fprintf(fpl_keys,"%i",Ki[j]);
        }
    }
    fclose(fpl_keys);
}
