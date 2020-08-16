section .bss
    input: resb 2
section .data
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