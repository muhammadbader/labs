
section .bss
    buffer: resb 5000
    input: resb 1
section .data
    ;;fileName: db "greeting",0
    fileName: db "testme.txt"
    handle: dd 0 ;;file descriptor
    test: db "Dana %d",0x0a,0
    len: equ $ - test
    argc: dd 0
    debug: dd 0
    errorInput:dd "invalid input"
    lenIn: equ $ - errorInput
    errorOutput:dd "could not Print"
    lenOut: equ $ - errorOutput
section .text
    global _start
    
_start:
    
    pop dword[argc]
    mov esi,esp

read_input:
    ;; read from input
    mov eax,3
    mov ebx,0
    mov ecx,input
    mov edx, 1
    int 0x80
    cmp eax,-1
    jng ERRORinput
    cmp dword[input],0x0a
    je continue
    cmp dword[input], 0;;EOF
    je end_program
    cmp dword[argc],2
    je debugging
    jmp check
continue:
    mov eax,4;; write
    mov ebx,1;; write to STDOUT
    mov ecx,input
    mov edx,1
    int 0x80
    cmp eax,-1
    jng ERRORoutput
    jmp read_input

check:
    cmp dword[input], 'a'-1
    jng finish
    cmp dword[input], 'z'
    jg finish
    sub dword[input],'a'
    add dword[input],'A'
finish:
    cmp dword[argc],2
    je continue2
    jmp continue

debugging: 
    mov dword[input+4],"    "
    mov eax,4;; write
    mov ebx,2 ;; 2 is file descriptor for stderr
    mov ecx,input;; print the ascii value
    mov edx,8
    int 0x80
    cmp eax,-1
    jng ERRORoutput
    jmp check

continue2:
    mov dword[input+4],0x0a
    mov eax,4 ;; write
    mov ebx,2 ;; 2 is file descriptor for stderr
    mov ecx,input
    mov edx,5
    int 0x80
    cmp eax,-1
    jng ERRORoutput
    jmp continue

; close_file:
;     mov eax,6
;     mov ebx,[handle]
;     int 0x80



end_program:
    mov eax,1
    mov ebx,0
    int 0x80
    nop
ERRORinput:
    mov eax,4 ;; write
    mov ebx,2 ;; 2 is file descriptor for stderr
    mov ecx, errorInput
    mov edx,lenIn
    int 0x80
    jmp end_program

ERRORoutput:
    mov eax,4 ;; write
    mov ebx,2 ;; 2 is file descriptor for stderr
    mov ecx, errorOutput
    mov edx,lenOut
    int 0x80
    jmp end_program
    ; open_file:
;     mov eax,5
;     mov ebx,fileName
;     mov ecx, 0x00a
;     mov edx, 0x777
;     int 0x80
;     mov dword[handle], eax
;     ;jmp write_toFile

; seek_file:
;     mov eax,19
;     mov ebx,[handle]
;     mov edx,0   ;;the starting point of moving
;     mov ecx,7   ;;number of bytes to move
;     int 0x80

; write_toFile:
;     mov eax, 4 
;     mov ebx,[handle]
;     mov ecx,test
;     mov edx, len
;     int 0x80