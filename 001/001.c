#include <stdio.h>


int main(void){
    unsigned int a, b;
    int res = scanf("%x %x", &a, &b);
    if(res != 2){
        perror("incorrect input");
        return 0;
    }

    printf(" ");
    for(int i=0; i<16; i++){
        printf("  ");
        printf("%X", i);
    }
    printf("\n");


    for(int i=2; i<8; i++){
        printf("%dx ", i);
        
        for(int j=0; j<16; j++){
            char ch = (i * 16) + j;
            if(ch < a){
                printf("   ");
                continue;
            }
            if(ch > b) break;
            printf("%c  ", ch);

        }
        printf("\n");
    }

}
