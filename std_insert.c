#include <stdio.h>
#include <string.h>

int a = 100000;

int prime(int max){
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

int main(void){
    printf("%d",prime(a));
}

