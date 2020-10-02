#include <stdio.h>
#include <string.h>

int main( int argc, const char* argv[] ){
        int c ;
        int cc = 0;
        char output[81] = "";

        while((c = getchar()) != EOF && strlen(output) < 80){

                switch (c){
                        case'\n':
                                output[strlen(output)] = ' ';
                                output[strlen(output)+1] = '\0';
                                break;
                        case'*':{
                                int tempChar = getchar();
                                if(tempChar!= EOF && tempChar == '*'){
                                        output[strlen(output)] = '^';
                                        output[strlen(output)+1] = '\0';
                                        break;
                                }else{
                                        ungetc(tempChar, stdin);

                                        /* output[cc] = '*';
                                        output[cc+1] = tempChar;

                                        printf("got here")*/
                                }
                        }
                        default:
                                output[strlen(output)] = c;
                                output[strlen(output)+1] = '\0';
                                break;
                }
                if(strlen(output) == 10){
                        printf("%s\n", output);
                        /*output[0] = 0;*/
                }
        }
        /*      printf( "\nHello World\n\n" );*/
        return 0;

}

