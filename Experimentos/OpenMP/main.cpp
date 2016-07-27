//
// Created by Guilherme Wachs on 23/05/16.
//
#include <cstdio>


int main(int argc, char *argv[]) {

    int i;
    #pragma omp parallel for private (i)
    for(i=0; i<120;i++){
        printf("%d\n", i);
    }

    return 0;
}