#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "keys.h"
#include "des.h"

int xor(int x, int y){
    if(x == 1 && y == 1)
        return 0;
    else if(x == 0 && y == 0)
        return 0;
    else
        return 1;
}

int *toBit7(int l){ //zamienia liczby na ciagi zer i jednek
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

int *DES(int *Znak){
    int k,f;
    int P[64];
    int *tab = malloc(8*sizeof(int));
    int count = 7;
        for(f = 0; f<8; f++){
            tab = toBit7(Znak[f]);
            for(k = 0; k<8; k++){
                P[k+(f*8)] = tab[k];
            }
        }
    keyGenerator();
    FILE *fpl = fopen("klucze.txt", "r");
    int K[16][48];
    int x,y,i,j;
    for(x = 0; x<16; x++){
        for(y = 0; y<48; y++){
            K[x][y] = fgetc(fpl) - '0';
        }
    }
    fclose(fpl);
    int IP[64] =  {P[57], P[49], P[41], P[33], P[25], P[17], P[9], P[1],
                   P[59], P[51], P[43], P[35], P[27], P[19], P[11], P[3],
                   P[61], P[53], P[45], P[37], P[29], P[21], P[13], P[5],
                   P[63], P[55], P[47], P[39], P[31], P[23], P[15], P[7],
                   P[56], P[48], P[40], P[32], P[24], P[16], P[8], P[0],
                   P[58], P[50], P[42], P[34], P[26], P[18], P[10], P[2],
                   P[60], P[52], P[44], P[36], P[28], P[20], P[12], P[4],
                   P[62], P[54], P[46], P[38], P[30], P[22], P[14], P[7]};//pierwsza permutacja
    int L[32];
    int R[32];
    int T[48];
    int Tp[8][6];
    int Tpp[8][4];
    int Tppp[32];
    int F[32];
    int tmp[32];
    int E[48] = {31, 0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 7, 8, 9, 10, 11, 12,
                 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20,
                 21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30,
                 31, 0};
    int PE[32] = {15,6,19,20,28,11,27,16,0,14,22,25,4,17,30,9,1,7,23,13,
                  31,26,2,8,18,12,29,5,21,10,3,24};
    int S[8][64] = {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
                     0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
                     4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
                     15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
                    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
                     3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
                     0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
                     13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
                    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
                     13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
                     13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
                     1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
                    {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
                     13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
                     10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
                     3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
                    {2,12,4,17,10,11,6,8,5,13,15,13,0,14,9,
                     14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
                     4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
                     11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
                    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
                     10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
                     9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
                     4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
                    {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
                     13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
                     1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
                     6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
                    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
                     1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
                     7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
                     2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}};
    int c = 0;
    int var = 0;
    count = 3;
    for(x = 0; x<64; x++){
        if(x<32)
            L[x] = IP[x];
        else
            R[x-32] = IP[x];
    }
    for(x = 1; x<17; x++){ //glowna petla - 16 rund
        for(y = 0; y<32; y++){
            tmp[y] = L[y];
        }
        for(y = 0; y<32; y++){
            L[y] = R[y];
        }
        for(i = 0; i<48; i++){
            T[i] = R[E[i]]; //krok a)
        }
        for(i = 0; i<48; i++){
            T[i] = xor(T[i], K[x-1][i]);
        }
        for(j = 0; j<8; j++){
            for(k = 0; k<6; k++){
                Tp[j][k] = T[(j*6)+k];
            }
        } //krok b)
        for(j = 0; j<8; j++){
            for(k = 4; k>0; k--){
                c = c + (Tp[j][k])*(pow(2,(4-k)));
            }
            var = S[j][16*(2*Tp[j][0]+Tp[j][5])+c];
            while(var!=0){
                Tpp[j][count] = var%2;
                var = var/2;
                count--;
            }
            while(count>=0){
                Tpp[j][count] = 0;
                count--;
            }
            c = 0;
            var = 0;
            count = 3;
        }
        for(j = 0; j<8; j++){
            for(k = 0; k<4; k++){
                Tppp[j*4+k] = Tpp[j][k];
            }
        }
        for(j = 0; j<32; j++)
            F[j] = Tppp[PE[j]];
        for(y = 0; y<32; y++)
            R[y] = xor(tmp[y], F[y]);
    }
    for(x = 0; x<64; x++){
        if(x<32)
            IP[x] = R[x];
        else
            IP[x] = L[x-32];
    }
    int C[64] = {IP[39],IP[7],IP[47],IP[15],IP[55],IP[23],IP[63],IP[31],
                 IP[38],IP[6],IP[46],IP[14],IP[54],IP[22],IP[62],IP[30],
                 IP[37],IP[5],IP[45],IP[13],IP[53],IP[21],IP[61],IP[29],
                 IP[36],IP[4],IP[44],IP[12],IP[52],IP[20],IP[60],IP[28],
                 IP[35],IP[3],IP[43],IP[11],IP[51],IP[19],IP[59],IP[27],
                 IP[34],IP[2],IP[42],IP[10],IP[50],IP[18],IP[58],IP[26],
                 IP[33],IP[1],IP[41],IP[9],IP[49],IP[17],IP[57],IP[25],
                 IP[32],IP[0],IP[40],IP[8],IP[48],IP[16],IP[56],IP[24]};
    int CC[8] = {0,0,0,0,0,0,0,0};
    for(x = 63; x>=0; x--){
        CC[x/8] = CC[x/8] + C[x]*(pow(2,((x+1)%8)));
    }
    for(x=0;x<8;x++){
        Znak[x] = CC[x];
    }
    free(tab);
    return Znak;
}

