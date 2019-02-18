#include<stdio.h>
#include<stdlib.h>
#include "des.h"

int charsNumber(FILE *fp){
    int count = 0;
    int buff = fgetc(fp);
    while(buff!=EOF){
        count++;
        buff = fgetc(fp);
    }
    return count;
}

void fileFilling(int l, FILE *fp){
    int t = 8 - l%8;
    int i;
    for(i=1;i<=t;i++){
        fprintf(fp,"%i",0);
    }
}

void writeToFile(FILE *fp, int *tab, int count){
    int n = count%9;
    if(n==0)
        fprintf(fp,"\n");
    int i;
    for(i=0;i<8;i++){
        fprintf(fp,"%i ", tab[i]);
    }
    fprintf(fp,"\n");
}

int main(){
    FILE *fpl1 = fopen("tekst.txt", "r");
    if(fpl1 == NULL){
        printf("Cos nie tak z plikiem!");
        return -1;
    }
    int count = charsNumber(fpl1);
    fclose(fpl1);
    int n = count%8;
    if(n!=0){ //uzupelnianie do wielokrotnosci 8
        FILE *fpl2 = fopen("tekst.txt", "a");
        if(fpl2 == NULL){
            printf("Cos nie tak z plikiem!");
            return -1;
        }
        fileFilling(count,fpl2);
        fclose(fpl2);
    }
    int i,j;
    FILE *fpl_text = fopen("tekst.txt", "r");
    FILE *fpl_cipher_text = fopen("szyforgram.txt", "a");
    int text_block[8];
    int m = (count+n)/8;
    for(i=1;i<=m;i++){
        for(j=0;j<8;j++){
            text_block[j] = fgetc(fpl_text);
        }
        DES(text_block);
        writeToFile(fpl_cipher_text, text_block, i);
    }
    fclose(fpl_cipher_text);
    fclose(fpl_text);
    return 0;
}
