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
  int DisplayMode;
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
void ToggleDisplayMode(state* s);
void MemoryDisplay(state *s);
void printHex(int unit_size, int unit, char* addr);
void printDec(int unit_size, int unit, char* addr);
void SaveIntoFile(state* s);
void MemoryModify(state* s);

int main(int argc, char **argv){
    int menuSize = 0; 
    char buf[256];
    int op=-1;

    struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDMode},
        {"Set File Name", setFileName},
        {"Set Unit Size", setUnitSize},
        {"Load Into Memory", LoadIntoMemory},
        {"Toggle Display Mode", ToggleDisplayMode},
        {"Memory Display", MemoryDisplay},
        {"Save Into File", SaveIntoFile},
        {"Memory Modify", MemoryModify},
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

void ToggleDisplayMode(state* s)
{
    if(s->DisplayMode==0){
        printf("Display flag now on, hexadecimal representation\n");
        s->DisplayMode=1;
    }else{
         printf("Display flag now off, decimal representation\n");
        s->DisplayMode=0;
    }
}

void MemoryDisplay(state *s)
{
    int u,addr;
    char buffer[BUFFER];
    printf("enter a address in hexadecimal and units in decimal\n");
    fgets(buffer,BUFFER,stdin);
    sscanf(buffer,"%X %d",&addr,&u);
    if(addr==0){//special case
        if(s->DisplayMode==1){//hexa
            printf("Hexadecimal\n===========\n");
            printHex(s->unit_size, u, (char *)s->mem_buf);
        }else{//decimal
            printf("Decimal\n=======n");
            printDec(s->unit_size, u, (char *)s->mem_buf);
        }
    }else{
        if(s->DisplayMode==1){//hexa
            printf("Hexadecimal\n===========\n");
            printHex(s->unit_size, u, (char*)addr);
        }else{//decimal
            printf("Decimal\n=======n");
            printDec(s->unit_size, u, (char*)addr);
        }
    }
}

void SaveIntoFile(state* s)
{
    FILE* fd = fopen(s->file_name,"r+");
    if(!fd)
    {
        printf("failed to open the file\n");
        return;
    }
    int source_address,target_location,length;
    char buffer[BUFFER];
    printf("Please enter <source-address> <target-location> <length>\n");
    fgets(buffer,BUFFER,stdin);
    sscanf(buffer,"%X %X %d",&source_address,&target_location,&length);

    //check if tager-location greated the size of file_name
    fseek(fd,0,SEEK_END);
    int size=ftell(fd);
    if((target_location+length)>size){
        printf("input location is greater than the size of filename\n");
        fclose(fd);
        return;
    }

    fseek(fd,target_location,SEEK_SET);
    if(source_address==0){
        fwrite((char*)s->mem_buf,s->unit_size,length,fd);
    }else{
        fwrite((char*)source_address,s->unit_size,length,fd);
    }
    fclose(fd);
}

void MemoryModify(state* s)
{
    char buffer[BUFFER];
    printf("Please enter <location> <val>\n");
    int location;
    unsigned char val[4];// dword size
    fgets(buffer,BUFFER,stdin);
    sscanf(buffer,"%X %X",&location,&val);
    if(s->debug_mode){
        fprintf(stderr, "location: %#X, value: %hhn\n\n" ,location, val);
    }
    for (int i=0; i<s->unit_size; i++){//writing the replacement in the specified place
        s->mem_buf[location+i] = val[i];
    }
}

void quit(state* s){
    if(s->debug_mode){
        fprintf(stderr,"shutting down...\n");
    }
    free(s);
    exit(0);
}

void printHex(int unit_size, int unit, char* addr)
{
    char* fin= addr+unit*unit_size;
    while(addr<fin)
    {
        if(unit_size==4){
            int topri=*((int*)(addr));
            printf("%X\n",topri);
        }else if(unit_size==2){
            short topri=*((short*)(addr));
            printf("%X\n",topri);
        }else if(unit_size==1){
            char topri=*((char*)(addr));
            printf("%X\n",topri);
        }
        addr+=unit_size;  
    }
}


void printDec(int unit_size, int unit, char* addr)
{
    char* fin= addr+unit*unit_size;
    while(addr<fin)
    {
        if(unit_size==4){
            int topri=*((int*)(addr));
            printf("%d\n",topri);
        }else if(unit_size==2){
            short topri=*((short*)(addr));
            printf("%d\n",topri);
        }else if(unit_size==1){
            char topri=*((char*)(addr));
            printf("%d\n",topri);
        }
        addr+=unit_size;  
    }
}