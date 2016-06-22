#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define HASHSIZE 41    //ハッシュテーブルサイズ
    //primes(n); でn以下の最大の素数を計算可能

struct list{
    char word[20];
    int count_1;
    int count_2;
};

int add_hash_table(char *key);
int hash1(char *s);
int hash2(int h1);
int primes(int max);

struct list table[HASHSIZE]; //ハッシュテーブル
    /* table[i] 
     *    i : hash関数での値
     * table[i].word[j] wordのj文字目
     * table[i].count 出てきた回数      */ 

int main(void){
    //char buff[20];
    int crush_count = 0;
    int crush_countmax = 1;
    
    //初期化
    for(int i = 0; i < HASHSIZE; i++){
        table[i].count_1 = 0;
        table[i].count_2 = 0;
    }

    int filesize, pagesize, mmapsize;
    char *addr;
    filesize = lseek(0,0,SEEK_END);
    pagesize = getpagesize();
    mmapsize = (filesize + (pagesize - 1)) / pagesize * pagesize;
    addr = mmap(0, (size_t)mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
        /* addr ~ addr + filesize - 1
         * %c, *(addr + i)
         * %s, (char *)(addr + i) */
    
    int t = 0;  
    //前半ハッシュテーブルへの登録
    while( *(addr + t) != '@'){
        printf("%d %c\n", t, *(addr + t));
        ++t;
        /*
        printf("文字列データ(MAX=20) : ");
        gets(buff);
        if(!strcmp(buff, EOF)){
            break;
        }
        
        crush_count = add_hash_table(buff);
        if(crush_count > crush_countmax){ //最大衝突回数
            crush_countmax = crush_count;
        }
        */
    }
    printf("hoge\n");
    //後半ハッシュテーブルへの登録
    for(t++ ; t < (filesize - 1); t++){ //@を飛ばす
        printf("%d %c\n", t, *(addr + t));
    }
    /*
    printf("ハッシュ関数最大再計算回数 = %d\n", crush_countmax);
    printf("Return key\n");
    gets(buff);
    for(int i = 0; i <= HASHSIZE; ++i){ //ハッシュテーブル内容表示
        if(table[i][0] == '\0'){
            printf(".\n ");
        } else {
            printf("%s\n", t[i]);
        }
    }
    */
}

//ハッシュテーブルへ登録
int add_hash_table(char *key){
    int h, h1, h2 = 0;
    int crush_count = 1; //衝突回数
    h1 = hash1(key); //ハッシュ関数1 値計算
    while(1){
        h = (h1 + h2 * crush_count) % HASHSIZE; // ハッシュテーブル番号計算
        if(table[h].word == '\0'){ // 登録 
            strcpy(table[h].word, key);
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
