#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>/*import from it the O_RDWR ...*/
#include <sys/mman.h> /*provide us with the mmap*/


struct not_fun_desc {
    char *action;
    void (*fun)();
};
int debug = 0;
void toggleDebugMode();   
void examineELF_File();     
void quit();

/*global variables*/
int Currentfd = -1; /*the open file directory*/
struct stat *examinee;
void *entry_map;
Elf32_Ehdr *elf;

int main(int argc, char** argv) { 
    char input[256];
    int OP = -1;

    struct not_fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Examine ELF File", examineELF_File},
        {"Quit", quit},
        {NULL, NULL}
    };

    while(1){
        fprintf(stdout,"Choose action:\n");
        int p=0;
        while(menu[p].action!=NULL)
            fprintf(stdout,"%d- %s\n",p,menu[p++].action);
        fgets(input,256,stdin);
        sscanf(input,"%d",&OP);
        if(OP < 0 || OP >= p) {
            printf("Not within bounds\n");
            quit();
        }
        printf("within bounds\n");
        menu[OP-1].fun();
        printf("\n");
        
    }
    quit();
    return 0;
}

void toggleDebugMode() {
    if(debug) {
        debug = 0;
        fprintf(stderr, "Debug flag now off\n");
    }
    else {
        debug = 1;
        fprintf(stderr, "Debug flag now on\n");
    }
}

void quit() {
    if(munmap(entry_map, (size_t) examinee->st_size) < 0)
        perror("munmap failed");
    if(Currentfd != -1)
        close(Currentfd);
    if(debug)
        printf("quitting..\n");
    
    exit(EXIT_SUCCESS);
}

void examineELF_File(){
    char buffer[50]; /*a large enough random size number*/ 
    char filename[50];
    printf("Enter the file name\n");
    fgets(buffer,50,stdin);
    sscanf(buffer,"%s",filename);

    /*close any open file*/ 
    if(Currentfd != -1)
        close(Currentfd);
    Currentfd = open(filename,O_RDWR,0677); /*O_RDWR in short 2, 0677 permission to read*/
    if(Currentfd == -1){
        perror("sorry could not open the file\n");return;
    }

//  int fstat(int descriptor,
//            struct stat *buffer)
    
    if(fstat(Currentfd, examinee) <-1){
        close(Currentfd);
        Currentfd = -1;
        perror("stat failer :(\n");
        _exit(EXIT_FAILURE);
    }
    /*
        mmap() function or system call will create a mapping in the virtual meory of the current process.The address space 
        consist of multiple pages and each page can be mapped some resource. We can create this mapping for a resources we 
        want to use.
    */
   /*
    void *mmap(void *addr, size_t lengthint " prot ", int " flags ,int fd, off_t offset)
        void *addr is the address we want to start mapping
        size_t lengthint is the size we want to map in as integer
        PROT_READ|PROT_WRITE|PROT_EXEC options about page
        MAP_ANON|MAP_PRIVATE options about page
   */
    if(elf = mmap(NULL, (size_t)examinee->st_size,PROT_READ, MAP_PRIVATE, Currentfd, 0) == MAP_FAILED){
        close(Currentfd);
        Currentfd = -1;
        perror("mmap failed\n");
        _exit(EXIT_FAILURE);
    }
    /*the magic numbers 45 4c 46 ===> E L F*/ 
    if(elf->e_ident[EI_MAG0]!= ELFMAG0 || elf->e_ident[EI_MAG1]!= ELFMAG1 || elf->e_ident[EI_MAG2]!= ELFMAG2 || elf->e_ident[EI_MAG3]!= ELFMAG3 || elf->e_ident[EI_CLASS]!= ELFCLASS32){
        munmap(entry_map, (size_t) examinee->st_size);
        close(Currentfd);
        Currentfd = -1;
        perror("The file isn't ELF");
        _exit(EXIT_FAILURE);
    }

    printf("ELF Header:\n");
    printf("\tBytes 1,2,3 of the magic number (in ASCII): %c, %c, %c\n", elf->e_ident[EI_MAG1], elf->e_ident[EI_MAG2], elf->e_ident[EI_MAG3]);
    printf("\tData:");
    if(elf->e_ident[EI_DATA] == ELFDATA2LSB){
        printf("2's complemet ,little indeian data\n");
    }else if(elf->e_ident[EI_DATA] == ELFDATA2MSB){
        printf("2's complemet ,Big indeian data\n");
    }else if (elf->e_ident[EI_DATA]== ELFDATANONE){
        perror("wrong data encoding\n");
        _exit(EXIT_FAILURE);
    }else{
        perror("parameters data error\n");
        _exit(EXIT_FAILURE);
    }

    printf("\nEntry point                      : %X\n",elf->e_entry);
    printf("Section header table file offset : %d\n",elf->e_shoff);
    printf("Section header table entry count : %d\n",elf->e_shnum);
    printf("Section header table entry size  : %d\n",elf->e_shentsize);
    printf("Program header table file offset : %d",elf->e_phoff);
    printf("Section header table entry count : %d\n",elf->e_phnum);
    printf("Section header table entry size  : %d\n",elf->e_phentsize);
}

