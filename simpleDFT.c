#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define N 32
#define DOUBLEPI 6.28318531

double x[N];
double Xre[N];
double Xim[N];
double P[N];

int n, k;

FILE *f;

void init(void){
    //fill x[n] with random values between 0 and 9

    time_t t;
    srand((unsigned) time(&t));

    for(n = 0; n < N; n++){
        x[n] = ((2.0 * rand()) / (RAND_MAX)) - 1.0;
        //printf("%6.4f\n", x[n]);
    }
}

void calculateDFT(void){

    for(k = 0; k < N; k++){
        Xre[k] = 0;

        for(n = 0; n < N; n++){
            Xre[k] += x[n] * cos((k * n * DOUBLEPI) / N);
        }
        Xim[k] = 0;

        for(n = 0; n < N; n++){
            Xim[k] -= x[n] * sin((k * n * DOUBLEPI) / N);
        }

        P[k] = (Xre[k] * Xre[k]) + (Xim[k] * Xim[k]); //Power spectrum
    }
    for(n = 0; n < N; n++) printf("%f\n", Xim[n]);
}

void createFile(FILE *f){
    int i;

    printf("Enter name of .m file: ");
    char fileName[64];
    gets(fileName);
    if(strstr(fileName, ".m") == NULL){ //if the user don't add .m in the fileName, we can concatenate with strcat
        strcat(fileName, ".m");
    }

    if(!(f = fopen(fileName, "w+"))){
        fprintf(stderr, "Cannot create or write, check permission\n");
    }

    /**Matlab .m file structure
    P = [ ... ];
    Xre = [ ... ];
    Xim = [ ... ];
    x = [ ... ];
    subplot(3, 1, 1);
    plot(x);
    subplot(3, 1, 2);
    hold on;
    plot(Xre, 'r');
    plot(Xim, 'g');
    subplot(3, 1, 3);
    stem(P);
    **/


    fputs("P = [", f);
    for(i = 0; i < N; i++){
        fprintf(f, "%f", P[i]);
        fputs(" ", f);

    }
    fputs("];\n", f);

    fputs("Xre = [", f);
    for(i = 0; i < N; i++){
        fprintf(f, "%f", Xre[i]);
        fputs(" ", f);
    }
     fputs("];\n", f);

    fputs("Xim = [", f);
    for(i = 0; i < N; i++){
        fprintf(f, "%f", Xim[i]);
        fputs(" ", f);
    }
    fputs("];\n", f);

    fputs("x = [", f);
    for(i = 0; i < N; i++){
        fprintf(f, "%f", x[i]);
        fputs(" ", f);
    }
    fputs("];\n", f);

    fprintf(f, "subplot(3, 1, 1);\n");
    fprintf(f, "plot(x);\n");
    fprintf(f, "subplot(3, 1, 2);\n");
    fprintf(f, "hold on;\n");
    fprintf(f, "plot(Xre, 'r');\n");
    fprintf(f, "plot(Xim, 'g');\n");
    fprintf(f, "subplot(3, 1, 3);\n");
    fprintf(f, "stem(P);\n");


    fclose(f);
}

int main(void){
    init();
    calculateDFT();
    createFile(f);
    exit(0);
}
