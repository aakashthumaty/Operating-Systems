#include <stdio.h>
#include <string.h>

int main( int argc, const char* argv[] ){
        int c ;
        int cc = 0;
        char output[81] = "";

        while((c = getchar()) != EOF){

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
                                }
                        }
                        default:
                                output[strlen(output)] = c;
                                output[strlen(output)+1] = '\0';
                                break;
                }
                if(strlen(output) == 80){
                        printf("%s\n", output);
                        memset(output, 0, 81);
                }
        }
        return 0;
}

