#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define TABLESIZE 50000   
#define HASHSIZE 49999 //ハッシュテーブルサイズ
    //primes(n); でn以下の最大の素数を計算可能
#define WORDSIZE 25

typedef struct {
    char word[WORDSIZE];
    int count_1;
    int count_2;
} list;

int search_hash_table(const char *key, const int *half);
int hash1(const char *s);
int hash2(int *h1);
//void quicksort(const int *half, int left, int right);
//int primes(int max);

list table[TABLESIZE]; //ハッシュテーブル
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
    int half = 0; //1は後半
    char buff[WORDSIZE] = {'\0'}; //単語を収納する配列
    while(t < (filesize - 1)){ //前半ハッシュテーブルへの登録
        char *temp = addr + t;
        if (*temp == '@'){
            half = 1;    
            //break;
        } else {
            if( *temp == ' ' || *temp == ',' || *temp == '.' ||
                    *temp == '\''|| *temp == '\"'|| *temp == '\t' ||
                    *temp == '-' || *temp == '\n'|| *temp == ':' ||
                    *temp == ';' || *temp == '?' || *temp == '!' || *temp == '(' ||
                    *temp == ')' || *temp == '&' ){
                if (buff[0] != '\0'){ //単語の区切り
                    if (*temp == '-' || *temp == '\''){
                        buff[letter] = *temp;
                        ++letter;
                    } else {
                        if(buff[letter - 1] == '-' || buff[letter - 1] == '\''){
                            buff[letter - 1] = '\0';
                        }
                        crush_count = search_hash_table(buff, &half);
                        memset(buff, '\0', WORDSIZE);
                        letter = 0;
                    }
                } else {
                    if (*temp == '\'' || *temp == '-'){
                        buff[letter] = *temp;
                        ++letter;
                    }
                }
            } else {
                buff[letter] = tolower(*temp);
                ++letter;
            }
            if(crush_count > crush_countmax){ //最大衝突回数
                crush_countmax = crush_count;
            }
        }
        ++t;
    }
    printf("ハッシュ関数最大再計算回数 = %d\n", crush_countmax);
    /*
    for(int i = 0; i <= HASHSIZE; ++i){ //ハッシュテーブル内容表示
        if(table[i].word[0] == '\0'){
            printf("%3d .\n", i);
        } else {
            printf("%3d %s %d %d\n", i, table[i].word, table[i].count_1,table[i].count_2 );
        }
    }
    */ 
    printf("crush_countmax %d\n\n", crush_countmax);
    
    //ここからランキングを考える
    int rank_hash_1[5] = {-1,-1,-1,-1,-1}; //前半
    int rank_hash_2[5] = {-1,-1,-1,-1,-1}; //後半
    //前半ランキング
    for(int i = 0; i < HASHSIZE; i++){
        if((table[i].word[0] != '\0') && (table[i].count_2 == 0)){ //後半出現しない
            if(rank_hash_1[0] == -1){ //-1が残っている
                for(int j = 4; j >= 0; j--){
                    if(rank_hash_1[j] == -1){
                        rank_hash_1[j] = i;
                        break;
                    } else if (table[i].count_1 <= table[rank_hash_1[j]].count_1) {
                        for(int k = 0; k < j; k++){
                            rank_hash_1[k] = rank_hash_1[k + 1]; 
                        }
                        rank_hash_1[j] = i;
                        break;
                    }
                }
            } else {
                int j = 4;
                while((j > -1) && (table[i].count_1 > table[rank_hash_1[j]].count_1)){
                    --j; 
                } 
                if(j < 4){ //暫定5位以内なら
                    for(int k = 4; k > (j + 1); --k){
                        rank_hash_1[k] = rank_hash_1[k - 1];
                    }
                    rank_hash_1[j + 1] = i;
                }
            }
        }
    }
    //後半ランキング
    for(int i = 0; i < HASHSIZE; i++){
        if((table[i].word[0] != '\0') && (table[i].count_1 == 0)){ //前半出現しない
            if(rank_hash_2[0] == -1){ //-1が残っている
                for(int j = 4; j >= 0; j--){
                    if(rank_hash_2[j] == -1){
                        rank_hash_2[j] = i;
                        break;
                    } else if (table[i].count_2 <= table[rank_hash_2[j]].count_2) {
                        for(int k = 0; k < j; k++){
                            rank_hash_2[k] = rank_hash_2[k + 1]; 
                        }
                        rank_hash_2[j] = i;
                        break;
                    }
                }
            } else {
                int j = 4;
                while((j > -1) && (table[i].count_2 > table[rank_hash_2[j]].count_2)){
                    --j; 
                } 
                if(j < 4){ //暫定5位以内なら
                    for(int k = 4; k > (j + 1); --k){
                        rank_hash_2[k] = rank_hash_2[k - 1];
                    }
                    rank_hash_2[j + 1] = i;
                }
            }
        }
    }
    printf("後半出現しない単語で、前半でのランキング\n");
    for(int i = 0; i < 5; i++){
        printf("%d %s %d\n", i, table[rank_hash_1[i]].word, table[rank_hash_1[i]].count_1);
    }
    printf("\n前半出現しない単語で、後半でのランキング\n");
    for(int i = 0; i < 5; i++){
        printf("%d %s %d\n", i, table[rank_hash_2[i]].word, table[rank_hash_2[i]].count_2);
    }
}


//ハッシュテーブルを検索、なかったらハッシュテーブルへ登録
int search_hash_table(const char *key, const int *half){
    int h, h1, h2 = 0;
    int crush_count = 1; //衝突回数
    h1 = hash1(key); //ハッシュ関数1 値計算
    while(1){
        h = (h1 + h2 * crush_count) % HASHSIZE; // ハッシュテーブル番号計算
        if (h < 0){
            h += HASHSIZE;
        }
        if(table[h].word[0] == '\0'){ //単語がない時 
            strcpy(table[h].word, key); //単語登録
            if (*half == 0){
                table[h].count_1 += 1;
            } else {
                table[h].count_2 += 1;
            }
            break;
        } else if (strcmp(table[h].word, key) == 0){ //単語が一致
            if (*half == 0){
                table[h].count_1 += 1;
            } else {
                table[h].count_2 += 1;
            }
            break; 
        } else {
            h2 = hash2(&h1); // ハッシュ関数値再計算
            ++crush_count;// 衝突回数カウント
        }
    }
    --crush_count;
    return(crush_count);
}

int hash1(const char *s){
    int x = 0;
    do{
        x = (x * 0x80 + *s - 0x20) % HASHSIZE;
    } while(*++s);
    return(x);
}

int hash2(int *h1){
	if(*h1 == 0){
        return(1);
	} else {
        return(HASHSIZE - *h1);
	}
}

/*
void quicksort(const int *half, int left, int right){
    int i,j;
    int pivot;
    list temp;
    i = left;
    j = right;
    if(*half == 0){
        pivot = table[left].count_1;
        while(1){
            while(table[i].count_1 < pivot){
                printf("i");
                ++i;
            }
            while(pivot < table[j].count_1){
                printf("j");
                --j;
            }
            if(i >= j){
                break;
            }
            printf("swap\n");
            temp = table[i];
            table[i] = table[j];
            table[j] = temp;
        }
        //if (left < i - 1){
            quicksort(half, left, i - 1);
        //}
        //if (j + 1 < right){
            quicksort(half, j + 1, right);
        //}
    } else {
    }
}
*/
/*
int primes(int max){ // 素数計算
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
