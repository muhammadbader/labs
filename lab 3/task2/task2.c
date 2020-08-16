#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char* sig;
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

virus* readVirus(FILE*);
void printVirus(virus* virus, FILE* output);
char* toHex(unsigned short dec);
char* chrSize(unsigned short);
void options();

void list_print(link *virus_list, FILE*); 
     /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
 
link* list_append(link* virus_list, virus* data); 
     /* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */
 
void list_free(link *virus_list); /* Free the memory allocated by the list. */

//task 1c
void detect_virus(char *buffer, unsigned int size, link *virus_list);

void kill_virus(char *fileName, int signitureOffset, int signitureSize);

int main(int argc, char** argv)
{
    options();
    return 0;
    
    FILE* f;
    FILE* write;
    write=fopen(argv[2],"w"); 
    f=fopen(argv[1],"rb");
    while(!feof(f))
    {
        virus* vi=readVirus(f);
        printVirus(vi,write);
        free(vi->sig);
        free(vi);
    }
    fclose(f);
    fclose(write);
    return 0;
}

int BUFFERSIZE=100;
void options()
{
    FILE* file=NULL;
    char* original;
    link* virus_list=NULL;
    while(1)
    {
        fseek(stdin,0,SEEK_END);
        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("4) Fix file\n");
        printf("5) Quit\n");
        
        int opt;
        int BUFFERSIZE=100;
        char input[BUFFERSIZE];
        char *final=malloc(1);
        *final=0;
        while(fgets(input,BUFFERSIZE-1,stdin))
        {
            final=realloc(final,strlen(final)+strlen(input)+1);
            strcat(final,input);
            if(input[strlen(input)-1]=='\n') break;
        
        }
         sscanf(final,"%d",&opt);
         free(final);
         fseek(stdin,0,SEEK_END);
        
        if(opt==1)
        {
            char input2[BUFFERSIZE];
            printf("Enter a file name\n");
            char* filename=malloc(sizeof(char));
            *filename=0;
            
            while(fgets(input2,BUFFERSIZE,stdin))
            {
                filename=realloc(filename,strlen(filename)+strlen(input2)+1);
                strcat(filename,input2);
                if(input2[strlen(input2)-1]=='\n') break;
            }
            char fil[strlen(filename)];
            sscanf(filename,"%s %*s",fil);
            file=fopen(fil,"rb");
            if (file==NULL)
            {
                printf("fail to open\n");
                continue;
            }
            while(!feof(file))
            {
                virus* vir = readVirus(file);
                virus_list=list_append(virus_list,vir);
            }
            free(filename);
        } 
        else if(opt==2)
        {
            if (file==NULL) continue;
            list_print(virus_list,stdout);
        } 
        else if (opt==5)
        {
            if(file!=NULL) fclose(file);
            list_free(virus_list);
            return; 
        }
        else if (opt==3)
        {
            FILE* check;
            printf("Enter the file name for detection:\n");
            char* filename=malloc(sizeof(char));
            *filename=0;
            char input2[BUFFERSIZE];
            while(fgets(input2,BUFFERSIZE,stdin))
            {
                filename=realloc(filename,strlen(filename)+strlen(input2)+1);
                strcat(filename,input2);
                if(input2[strlen(input2)-1]=='\n') break;
            }
            char fil[strlen(filename)];
            sscanf(filename,"%s %*s",fil);
            original=fil;
            file=fopen(fil,"rb");
            if (file==NULL)
            {
                printf("fail to open\n");
                continue;
            }
            free(filename);
            char* buffer=malloc(10000*sizeof(char));
            fread(buffer,100000,1,file);

            //check the file size
            int size=10000;
            while(buffer[size-1]=='\0') size--;
            detect_virus(buffer,size,virus_list);//check again
        }
        else if(opt==4)
        {
            //"Fix file" will request the user to enter the the starting byte location in the suspected file (again the one given as the command-line argumen) and the size of the virus signature.
            //The fix will be done by the following function
            printf("Enter the starting byte location: \n");
            int signitureOffset;
            int signitureSize;

            char* filename=malloc(sizeof(char));
            *filename=0;
            char input2[BUFFERSIZE];
            while(fgets(input2,BUFFERSIZE,stdin))
            {
                filename=realloc(filename,strlen(filename)+strlen(input2)+1);
                strcat(filename,input2);
                if(input2[strlen(input2)-1]=='\n') break;
            }
            sscanf(filename,"%d",&signitureOffset);
            free(filename);
            printf("Enter the Signaature size: \n");
            filename=malloc(sizeof(char));
            *filename=0;
            char input[BUFFERSIZE];
            while(fgets(input,BUFFERSIZE,stdin))
            {
                filename=realloc(filename,strlen(filename)+strlen(input)+1);
                strcat(filename,input);
                if(input[strlen(input)-1]=='\n') break;
            }
            sscanf(filename,"%d",&signitureSize);
            free(filename);

            kill_virus(original,  signitureOffset,  signitureSize);
        }
    }
}

virus* readVirus(FILE* file)
{
    virus* vi=malloc(sizeof(virus));
    unsigned char c[2];
    fread(c,1,2,file);
    vi->SigSize=c[0]+c[1]*256;
    if(vi->SigSize!=0)
    fread((vi->virusName),16,1,file);
    vi->sig=malloc(sizeof(char)*(vi->SigSize));
    int* try[vi->SigSize];
    fread(vi->sig,vi->SigSize,1,file);
    return vi;
}

void printVirus(virus* virus, FILE* output)
{
    if(virus!=NULL&&virus->SigSize!=0)
    {
        fseek(output,0,SEEK_END);
        char* vname="Virus name: ";
        fwrite(vname,strlen(vname),1,output);
        fwrite(virus->virusName , strlen(virus->virusName ) , 1 , output );
        fseek(output,0,SEEK_END);
        char* vsize="\nVirus size: ";
        char* size=chrSize(virus->SigSize);
        fwrite(vsize,strlen(vsize),1,output);
        fwrite(size , strlen(size) , 1 , output );
        free(size);
        fseek(output,0,SEEK_END);
        char* signature="\nsignature: \n";
        fwrite(signature,strlen(signature),1,output);
        for(int i=0;i<virus->SigSize;i++)
        {
            char *tmp=toHex(virus->sig[i]);
            fwrite( tmp, 3 , 1 , output );
            free(tmp);
        }
        fseek(output,0,SEEK_END);
        char* newline="\n";
        fwrite(newline,strlen(newline),1,output);
        fwrite(newline,strlen(newline),1,output);
    }
}

char* toHex(unsigned short dec)
{
    char *ans=calloc(3,sizeof (char));
    unsigned short fac=1;
    int counter=1;
    while(counter>=0)
    {
        int x=(dec%16);
        if(x>=0&&x<=9)
        {
            ans[counter--]=x+'0';
        }
        else
        {
            ans[counter--]=x-10+'A';
        }
        
        fac=fac*16;
        dec=dec/16;
    }
    ans[2]=' ';
    return ans;
}

char* chrSize(unsigned short size)
{
    char* tmp;
    int counter=0;
    char* ans=malloc(sizeof(char));
    while(size>0)
    {
        int x=size%10;
        size/=10;
        tmp=ans;
        counter++;
        ans=malloc(counter*sizeof(char));
        ans[0]=x+'0';
        for(int i=1;i<counter;i++)
        {
            ans[i]=tmp[i-1];
        }
    }
    if(counter==0) ans[0]='0';
    return ans;
}

///task 1b



void list_print(link *virus_list, FILE* file)
{
    while(virus_list!=NULL)
    {
        printVirus(virus_list->vir,file);
        virus_list=virus_list->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data) //add to the tail of the list
{
    if(data==NULL) return virus_list;
    if(virus_list == NULL)
    {
        virus_list=malloc(sizeof(link));
        virus_list->vir=data;
        virus_list->nextVirus=NULL;
        return virus_list;
    }
    else
    {
        link *newlist=malloc(sizeof(link));
        newlist->vir=data;
        newlist->nextVirus=NULL;
        link* tmp=virus_list;
        while(tmp->nextVirus!=NULL)
        {
            tmp=tmp->nextVirus;
        }
        tmp->nextVirus=newlist;
        return virus_list;
    }
}

void list_free(link *virus_list) /* Free the memory allocated by the list. */
{
    if(virus_list==NULL) return;
    if(virus_list->nextVirus!=NULL)
    {
        list_free(virus_list->nextVirus);
    }
    free(virus_list);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
    while(virus_list!=NULL)
    {
        for(int i=0;i<size-virus_list->vir->SigSize;i++)
        {
            if(memcmp(virus_list->vir->sig,buffer[i],virus_list->vir->SigSize)==0)
            {
                printf("The starting byte location in the suspected file: %d\n",i);
                printff("The virus name: %s\n",virus_list->vir->virusName);
                printf("The size of the virus signature: %d\n\n",virus_list->vir->SigSize);
                break;
            }
        }
    }
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
    FILE* file=fopen(fileName,"r+");
    fseek(file,signitureOffset,1);
    char x='\0';// NOP??
    for (int i=0;i<signitureSize;i++)
    {
        fwrite(&x,1,1,file);
    }
}