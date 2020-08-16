#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFFER 10000

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[BUFFER];
} state;

struct fun_desc{
    char *name;
    void (*fun)(state*);
};

void toggleDMode(state* s);
void setUnitSize(state* s);
void setFileName(state* s);
void quit(state* s);
void LoadIntoMemory(state *s);

int main(int argc, char **argv){
    int menuSize = 0; 
    char buf[256];
    int op=-1;

    struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDMode},
        {"Set File Name", setFileName},
        {"Set Unit Size", setUnitSize},
        {"Load Into Memory", LoadIntoMemory},
        {"Quit", quit},
        {NULL, NULL}
    };

    state* s = (state*)malloc(sizeof(state));  
    s->unit_size = 1;  

    while(menu[menuSize].name != NULL)
        menuSize++;

    while (1){

        if(s->debug_mode){
            fprintf(stderr, "Unit Size: %d\n", s->unit_size);
            fprintf(stderr, "File Name: %s\n", s->file_name);
        }
        
        int i = 0 ;
        fprintf(stdout, "Choose action: \n");

        for (i = 0; i < menuSize; i++)
            fprintf(stdout, "%d- %s\n", i, menu[i].name);
        fgets(buf, 256, stdin);
        sscanf(buf, "%d", &op);
        if (op < 0 || op >= menuSize){
            printf("Not within bounds\n");
            quit(s);
        }
        else{
            printf("Within bounds\n");
            menu[op].fun(s);
            printf("DONE.\n\n");
        }
        
    }
}



void toggleDMode(state* s){

    if (s->debug_mode == 0){
        fprintf(stderr,"debug flag is now on\n");
        s->debug_mode = 1;
    }
    else{
        fprintf(stderr,"debug flag is now off\n");
        s->debug_mode = 0;
    }
}


void setFileName(state* s){
    char name[128];
    fgets(name, 128, stdin);
    name[strlen(name)-1] = 0;//NULL Terminating
    strcpy(s->file_name, name);
    if(s->debug_mode){
        fprintf(stderr,"setting name to %s\n", s->file_name);
    }
}

void setUnitSize(state* s){
    char buf[100];
    fgets(buf, 100, stdin);
    int size=atoi(buf);
    if (size!=4&&size!=2&&size!=1){
        fprintf(stderr,"Must be 1,2 or 4\n");
        return;
    }
    else{
        s->unit_size=size;
    } 
    if(s->debug_mode){
        fprintf(stderr,"setting size to: %d\n", s->unit_size);
    }
}
void LoadIntoMemory(state *s)
{
    if(s->file_name == NULL){
        fprintf(stderr,"null filename\n");
        return;
    }
    FILE* fd=fopen(s->file_name,"r");
    char buffer[BUFFER];
    int location,length;
    if(fd==NULL)
    {
        fprintf(stderr,"could nnot open the file %s\n",s->file_name);
        return;
    }
    printf("enter a location in hexadecimal and length in decimal\n");
    fgets(buffer,BUFFER,stdin);
    sscanf(buffer,"%X %d",&location,&length);
    if(s->debug_mode){
        fprintf(stderr,"filename: %-10s location: %d  length: %d",s->file_name,location,length);
    }
    fseek(fd,location,SEEK_SET);
    fread(s->mem_buf,s->unit_size,length,fd);
    printf("Loaded %d units into memory\n",length);
    fclose(fd);
}

void quit(state* s){
    if(s->debug_mode){
        fprintf(stderr,"shutting down...\n");
    }
    free(s);
    exit(0);
}