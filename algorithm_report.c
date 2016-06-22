#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define TABLESIZE 50000 //50
#define HASHSIZE 49999 //47   //ハッシュテーブルサイズ
    //primes(n); でn以下の最大の素数を計算可能
#define WORDSIZE 30

struct list{
    char word[WORDSIZE];
    int count_1;
    int count_2;
};

int search_hash_table(char *key);
int hash1(char *s);
int hash2(int h1);
//int primes(int max);

struct list table[TABLESIZE]; //ハッシュテーブル
    /* table[i] 
     *    i : hash関数での値
     * table[i].word[j] wordのj文字目
     * table[i].count 出てきた回数      */ 

int main(void){
    int crush_count = 0;
    int crush_countmax = 1;
    
    //初期化
    for(int i = 0; i < HASHSIZE; i++){
        memset(table[i].word, '\0', WORDSIZE);
        table[i].count_1 = 0;
        table[i].count_2 = 0;
    }

    int filesize, pagesize, mmapsize;
    char *addr;
    filesize = lseek(0,0,SEEK_END);
    pagesize = getpagesize();
    mmapsize = (filesize + (pagesize - 1)) / pagesize * pagesize;
    addr = mmap(0, (size_t)mmapsize, PROT_READ, MAP_PRIVATE, 0, 0);
        /* 標準入力した文字列はaddrにある
         * アドレス範囲 addr ~ addr + filesize - 1
         * %c, *(addr + i)
         * %s, (char *)(addr + i) */
    int t = 0; //全体入力t文字目
    int letter = 0; //単語letter文字目
    char buff[WORDSIZE] = {'\0'}; //単語を収納する配列
    while( *(addr + t) != '@'){ //前半ハッシュテーブルへの登録
        char *temp = addr + t;
        if( *temp == ' ' || *temp == ',' || *temp == '.' || *temp == '\'' ||
                *temp == '-' || *temp == '\n' || *temp == ':' ||
                *temp == ';' || *temp == '?' || *temp == '!' || *temp == '(' ||
                *temp == ')' || *temp == '&' ){
            if (buff[0] != '\0'){ //単語の区切り
                crush_count = search_hash_table(buff);
                memset(buff, '\0', WORDSIZE);
                letter = 0;
            }
        } else {
            buff[letter] = tolower(*temp);
            ++letter;
        }
        if(crush_count > crush_countmax){ //最大衝突回数
            crush_countmax = crush_count;
        }
        ++t;
    }
    
    //後半ハッシュテーブルへの登録
    //for(t++ ; t < (filesize - 1); t++){ //@を飛ばす
        //printf("%d %c\n", t, *(addr + t));
    //}
    //printf("ハッシュ関数最大再計算回数 = %d\n", crush_countmax);
    
    for(int i = 0; i <= HASHSIZE; ++i){ //ハッシュテーブル内容表示
        if(table[i].word[0] == '\0'){
            printf("%3d .\n", i);
        } else {
            printf("%3d %s %d\n", i, table[i].word, table[i].count_1 );
        }
    }
    printf("crush_countmax %d\n", crush_countmax);
}


//ハッシュテーブルを検索、なかったらハッシュテーブルへ登録
int search_hash_table(char *key){
    int h, h1, h2 = 0;
    int crush_count = 1; //衝突回数
    h1 = hash1(key); //ハッシュ関数1 値計算
    while(1){
        h = (h1 + h2 * crush_count) % HASHSIZE; // ハッシュテーブル番号計算
        if(table[h].word[0] == '\0'){ //単語がない時 
            strcpy(table[h].word, key); //単語登録
            //printf("add %s\n", table[h].word);
            table[h].count_1 += 1; 
            break;
        } else if (strcmp(table[h].word, key) == 0){ //単語が一致
            //printf("count %s\n",table[h].word);
            table[h].count_1 += 1; //カウンターを増やす
            break; 
        } else {
            h2 = hash2(h1); // ハッシュ関数値再計算
            ++crush_count;// 衝突回数カウント
            //printf("rehash %d\n",crush_count);
        }
    }
    --crush_count;
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
