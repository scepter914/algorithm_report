#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define HASHSIZE 97    //ハッシュテーブルサイズ
    //primes(n); でn以下の最大の素数を計算可能

int add_hash_table(char *key);
int hash1(char *key);
int hash2(int);
int primes(int);

struct list{
    char word[20];
    int count;
};

char t[HASHSIZE][15]; //ハッシュテーブル

int main(void){
    char buff[80];
    int crush_count;
    int crush_countmax = 1;
    printf("Hash Size=%d\n", HASHSIZE);
    while(1){ //ハッシュテーブルへの登録
        printf("文字列データ(MAX=20) : ");
        gets(buff);
        if(!strcmp(buff, "0")){
            break;
        }
        crush_count = add_hash_table(buff);
        if(crush_count > crush_countmax){ //最大衝突回数
            crush_countmax = crush_count;
        }
    }
    printf("ハッシュ関数最大再計算回数 = %d\n", crush_countmax);
    printf("Return key\n");
    gets(buff);
    for(int i = 0; i <= HASHSIZE; ++i){ //ハッシュテーブル格納状況表示
        if(t[i][0] == '\0'){
            printf("0 ");
        } else {
            printf("1 ");
        }
    }
    printf("\nReturn key\n");
    gets(buff);
    for(int i = 0; i <= HASHSIZE; ++i){ //ハッシュテーブル内容表示
        if(t[i][0] == '\0'){
            printf(".\n ");
        } else {
            printf("%s\n", t[i]);
        }
    }
}

//ハッシュテーブルへ登録
int add_hash_table(char *key){
    int h, h1, h2 = 0;
    int crush_count = 1; //衝突回数
    h1 = hash1(key); //ハッシュ関数1 値計算
    while(1){
        h = (h1 + h2 * crush_count) % HASHSIZE; // ハッシュテーブル番号計算
        if(t[h][0] == '\0'){ // 登録 
            strcpy(t[h], key);
            break;
        }
        h2 = hash2(h1); // ハッシュ関数値再計算
        ++crush_count;// 衝突回数カウント
    }
    --crush_count;
    printf("データ=%-12s  ハッシュ値=%3d  衝突回数=%4d\n", key, h, crush_count);
    return(crush_count);
}

int hash1(char *s){
    int x = 0;
    do{
        x = (x * 0x60 + *s - 0x20) % HASHSIZE;
    } while(*++s);
    return(x);
}

int hash2(int h1){
	if(h1 == 0){
        return(1);
	} else {
        return(HASHSIZE - h1);
	}
}

// 素数計算
/*
int primes(int max){
    int m, i;
    for(m = max; m >= 2; --m){
        for(i = max / 2; i >= 2; --i){
        	if(m % i == 0){
                break;
        	}
        }
    	if(i == 1){
            break;
    	}
    }
    return(m);
}
*/
