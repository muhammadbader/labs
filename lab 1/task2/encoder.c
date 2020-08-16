#include <stdio.h>
#include <string.h>
#include<stdbool.h>

void Decoder(char c);
char encrypt(char c,int pos,char* eword,bool minus);
// int encryptminus(char c,int pos,char* eword,int len);

int main(int argc, char **argv) {
    bool D=false;
    bool e=false;
    bool o=false;
    bool minus=false;
    FILE* fp;
    char* filename;
    int pos=0;
    size_t len;
    char* eword;
    for(int i=1;i<argc;i++){
        if(strncmp(argv[i],"-D",2)==0) {D=true;}
        if(strncmp(argv[i],"-e",2)==0) {e=true;minus=true;eword=&argv[i][2];len=strlen(argv[i])-2;}
        if(strncmp(argv[i],"+e",2)==0) {e=true;eword=&argv[i][2];len=strlen(argv[i])-2;}
        if(strncmp(argv[i],"-i",2)==0) {o=true; filename=&argv[i][2];}
        if(D&&e&&o) break;
    }

    if (!(D||e||o)){
        while(1){
            char c = getchar();
            if( c==EOF ) {
                break ;
            }
            if(c>='a'&&c<='z'){
                c=c-'a'+'A';
            }
            printf("%c",c);
            }
        return 0;
    }else if(D&&!e&&!o){
        
             while(1){
                char c = getchar();
                
                if( c==EOF ) {
                    break ;
                }
                Decoder(c);
            }
        
    }else if(!D&&e&&!o){
        while(1){
            char c = getchar();
            if(c==EOF ) {
                    break ;
                }
            c=encrypt(c,pos,eword,minus);
            printf("%c",c);
            pos=(pos+1)%len;

        }
    }else if(D&&e&&!o){
        while(1){
            char c = getchar();
            if(c==EOF ) {
                    break ;
                }
            c=encrypt(c,pos,eword,minus);
            printf("%c",c);
            pos=(pos+1)%len;
        }
    }else if(o){
        fp=fopen(filename,"r");
        if(!D&&!e){
            while(1){
                char c = fgetc(fp);
                if(c!='\n'){
                    if( c==EOF ) {
                        break ;
                    }
                if(c>='a'&&c<='z'){
                    c=c-'a'+'A';
                }
                printf("%c",c);
                }else{
                    printf("\n");
                }
            }
        }else if(D&&!e){
            while(1){
                char c = fgetc(fp);
                if(c!='\n'){
                    if( c==EOF ) {
                        break ;
                    }
                Decoder(c);
                // if(c>='a'&&c<='z'){
                //     c=c-'a'+'A';
                // }
                // printf("%c",c);
                // }else{
                //     printf("\n");
                // }
                }
            }
        }else if(!D&&e){
             while(1){
                char c = fgetc(fp);
                if( c==EOF ) {
                    break ;
                }
                c=encrypt(c,pos,eword,minus);
                pos=(pos+1)%len;
                printf("%c",c);
            }
        }else if(D&&e){
            while(1){
                char c = fgetc(fp);
                if( c==EOF ) {
                    break ;
                }
                Decoder(c);
                c=encrypt(c,pos,eword,minus);
                pos=(pos+1)%len;
                printf("%c",c);
            }
        
    }

}
}
void Decoder(char c){
    if(c!='\n'){
        
        fprintf(stderr,"%d      ",c);
        if(c>='a'&&c<='z'){
            c=c-'a'+'A';
        }
        fprintf(stderr,"%d\n",c);
        fprintf(stdout,"%c",c);
    }else{
        fprintf(stderr,"\n");
        fprintf(stdout,"\n");
    }
}
char encrypt(char c,int pos,char* eword,bool minus){
    if(minus){
        if(c!='\n'){               
        c=c-eword[pos+2]+'0';
        // printf("%c",c);   
        }
    }else{
        if(c!='\n'){               
        c=c+eword[pos+2]-'0';
           
        }
    }
    return c;
}


