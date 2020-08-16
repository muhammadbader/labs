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
void printSectionNames();  
void PrintSymbol();  
void quit();

int ValidSec(int section);
/*global variables*/
int Currentfd = -1; /*the open file directory*/
struct stat examinee;
void *entry_map;
Elf32_Ehdr *elf;

int main(int argc, char** argv) { 
    char input[256];
    int OP = -1;

    struct not_fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Examine ELF File", examineELF_File},
        {"Print Section Names", printSectionNames},
         {"Print Symbol", PrintSymbol},
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
    // if(munmap(entry_map, (size_t) examinee->st_size) < 0)
    //     perror("munmap failed");
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
    Currentfd = open(filename,0); /*O_RDWR in short 2, 0677 permission to read*/
    if(Currentfd == -1){
        perror("sorry could not open the file\n");return;
    }

//  int fstat(int descriptor,
//            struct stat *buffer)
    
    if(fstat(Currentfd, &examinee) <-1){
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
    if((entry_map = mmap(0, (size_t)examinee.st_size,PROT_READ, MAP_PRIVATE, Currentfd, 0)) == MAP_FAILED){
        close(Currentfd);
        Currentfd = -1;
        perror("mmap failed\n");
        _exit(EXIT_FAILURE);
    }
    elf = (Elf32_Addr*) entry_map;
    /*the magic numbers 45 4c 46 ===> E L F*/ 
    if(elf->e_ident[EI_MAG0]!= ELFMAG0 || elf->e_ident[EI_MAG1]!= ELFMAG1 || elf->e_ident[EI_MAG2]!= ELFMAG2 || elf->e_ident[EI_MAG3]!= ELFMAG3 || elf->e_ident[EI_CLASS]!= ELFCLASS32){
        munmap(entry_map, (size_t) examinee.st_size);
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

void printSectionNames()
{
    if(Currentfd == -1){
        perror("no file descriptor open!!!\n");
        _exit(EXIT_FAILURE);
    }
    Elf32_Shdr *sections = (Elf32_Shdr*)(entry_map + elf->e_shoff);
    char *section_header_string_table = (char*)(entry_map + sections[elf->e_shstrndx].sh_offset);
    printf("Section Headers:\n");
    printf("%s %-19s %-15s %-15s %-15s %s\n", "[index]", "Name", "Addr", "Off", "Size", "Type");
    if(debug)
        printf("\t %s\n", "DEBUG MODE");
    int i;
    char stringType[100];
    for(i = 0; i < elf->e_shnum; i++){
        switch(sections[i].sh_type) {
            case SHT_NULL:                          /* Section header table entry unused */
                strcpy(stringType, "NULL");
                break;
            case SHT_PROGBITS:                      /* Program data */
                strcpy(stringType, "PROGBITS");
                break;
            case SHT_SYMTAB:                        /* Symbol table */
                strcpy(stringType, "SYMTAB");
                break;
            case SHT_STRTAB:                        /* String table */
                strcpy(stringType, "STRTAB");
                break;
            case SHT_RELA:                          /* Relocation entries */
                strcpy(stringType, "RELA");
                break;
            case SHT_REL:
                strcpy(stringType, "REL");
                break;
            case SHT_NOTE:
                strcpy(stringType, "NOTE");
                break;
            case SHT_NOBITS:
                strcpy(stringType, "NOBITS");
                break;
            case SHT_GNU_HASH:
                strcpy(stringType, "GNU_HASH");
                break;
            case SHT_DYNSYM:
                strcpy(stringType, "DYNSYM");
                break;
            default:
                strcpy(stringType, "0000000");
                break;
        }

        printf("[%d]\t %-20s %-15X %-15X %-15X %s", i, section_header_string_table + sections[i].sh_name, sections[i].sh_addr,
               sections[i].sh_offset, sections[i].sh_size, stringType);
        
        if(debug)
            printf("\t %d\n", sections[elf->e_shstrndx].sh_offset+sections[i].sh_offset);
        else
            printf("\n");
    }
}

void PrintSymbol()
{
    if(Currentfd == -1){
        perror("no file descriptor open!!!\n");
        _exit(EXIT_FAILURE);
    }
    Elf32_Shdr *symbols = (Elf32_Shdr*)(entry_map + elf->e_shoff);
    char *section_header_string_table = (char*)(entry_map + symbols[elf->e_shstrndx].sh_offset);  
    int i;
    for(i=0; i<elf->e_shnum; i++)
    {
        if(symbols[i].sh_type == SHT_SYMTAB/*this represents a symbol table*/ || symbols[i].sh_type == SHT_DYNSYM /*this represents a dynamic symbol table*/)
        {
            Elf32_Sym *sym_table_entry = (Elf32_Sym*)(entry_map + symbols[i].sh_offset);    /* Section file offset */
            int size = symbols[i].sh_size/ sizeof( Elf32_Sym);                      /* Section size */
            char *symbol_name = (char*)(entry_map + symbols[symbols[i].sh_link].sh_offset);
            if (debug)
            printf("\nThe size of symbol table: %d\nThe number of sybmols therein: %d\n", symbols[i].sh_size, size);
            printf("\n%s\t %s\t\t %s\t %s\t %s\n", "[index]", "Value", "Section index", "Section name", "Symbol name");
            int j;
            for(j=0; j<size; j++)
            {
                if(ValidSec(sym_table_entry[j].st_shndx)){
                    printf("[ %d ]\t %08x\t %d\t\t %s\t\t %s\n", j, sym_table_entry[j].st_value, sym_table_entry[j].st_shndx,
                            section_header_string_table + symbols[sym_table_entry[j].st_shndx].sh_name,
                            symbol_name + sym_table_entry[j].st_name);
                }else{
                    printf("[ %d ]\t %08x\t %d\t\t %s\t\t %s\n", j, sym_table_entry[j].st_value, sym_table_entry[j].st_shndx, "",
                            symbol_name + sym_table_entry[j].st_name);
                }
            }
        }
    }
}

int ValidSec(int section){
    if(section == SHN_LORESERVE  /* End of processor-specific */ || 
        section == SHN_HIPROC  /* End of processor-specific */ ||
        section == SHN_ABS /* Associated symbol is absolute */ || 
        section == SHN_COMMON /* Associated symbol is common */ ||
        section == SHN_HIRESERVE /* End of reserved indices */) 
            return 0;
    return 1;
}