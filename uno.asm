section .text

    extern rand

    global carta_valida
    global reversa
    global verificar_uno
    global ganador
    global aplicar_robo
    global siguiente_turno
    global efecto_especial
    global ia_validar
    global generar_valor
    global generar_color
    global castigo_uno


; ==========================================
; VALIDAR CARTA
; ==========================================

carta_valida:

    mov r8, rcx
    mov r9, rdx

    mov rax, r8
    mov r10, 100
    xor rdx, rdx
    div r10
    mov r12, rax
    mov r13, rdx

    mov rax, r9
    xor rdx, rdx
    div r10

    cmp r12, rax
    je valido
    cmp r13, rdx
    je valido

    mov rax, 0
    ret

valido:
    mov rax, 1
    ret


; ==========================================
; REVERSA
; ==========================================

reversa:
    mov rdi, rcx ; Windows: RCX -> RDI
    mov rax, rdi
    neg rax
    ret


; ==========================================
; VERIFICAR UNO
; ==========================================

verificar_uno:
    mov rdi, rcx ; Windows: RCX -> RDI
    cmp rdi, 2
    je tiene_uno

    mov rax, 0
    ret

tiene_uno:
    mov rax, 1
    ret


; ==========================================
; GANADOR
; ==========================================

ganador:
    mov rdi, rcx ; Windows: RCX -> RDI
    cmp rdi, 0
    je gano

    mov rax, 0
    ret

gano:
    mov rax, 1
    ret


; ==========================================
; APLICAR +2 y +4
; ==========================================

aplicar_robo:
    mov rdi, rcx ; Windows: RCX -> RDI
    cmp rdi, 10
    je roba2

    cmp rdi, 14
    je roba4

    mov rax, 0
    ret

roba2:
    mov rax, 2
    ret

roba4:
    mov rax, 4
    ret


; ==========================================
; CAMBIAR TURNO
; ==========================================

siguiente_turno:
    ; Windows: RCX->RDI, RDX->RSI, R8->RDX
    mov rdi, rcx
    mov rsi, rdx
    mov rdx, r8

    mov rax, rdi
    add rax, rsi

ciclo_negativo:

    cmp rax, 0
    jge ciclo_modulo

    add rax, rdx
    jmp ciclo_negativo

ciclo_modulo:

    cmp rax, rdx
    jl fin_turno

    sub rax, rdx
    jmp ciclo_modulo

fin_turno:
    ret


; ==========================================
; EFECTOS ESPECIALES
; ==========================================

efecto_especial:
    mov rdi, rcx ; Windows: RCX -> RDI
    cmp rdi, 11
    je efecto_saltar

    cmp rdi, 12
    je efecto_reversa

    mov rax, 0
    ret

efecto_saltar:
    mov rax, 1
    ret

efecto_reversa:
    mov rax, 2
    ret


; ==========================================
; IA VALIDAR
; ==========================================

ia_validar:
    mov rdi, rcx ; Windows: RCX -> RDI
    mov rsi, rdx ; Windows: RDX -> RSI

    mov rax, rdi
    mov rcx, 100
    xor rdx, rdx
    div rcx

    mov r8, rax
    mov r9, rdx

    mov rax, rsi
    xor rdx, rdx
    div rcx

    mov r10, rax
    mov r11, rdx

    cmp r8, r10
    je ia_si

    cmp r9, r11
    je ia_si

    mov rax, 0
    ret

ia_si:
    mov rax, 1
    ret


; ==========================================
; GENERAR VALOR
; ==========================================

generar_valor:
    sub rsp, 40
    call rand

    add rsp, 40
    xor rdx, rdx
    mov rcx, 15
    div rcx

    mov rax, rdx
    ret


; ==========================================
; GENERAR COLOR
; ==========================================

generar_color:
    sub rsp, 40
    call rand
    add rsp, 40

    xor rdx, rdx
    mov rcx, 4
    div rcx

    mov rax, rdx
    add rax, 1
    ret


; ==========================================
; CASTIGO UNO
; ==========================================

castigo_uno:
    mov rdi, rcx ; Windows: RCX -> RDI
    mov rsi, rdx ; Windows: RDX -> RSI
    cmp rdi, 2
    jne no_castigo

    cmp rsi, 0
    je si_castigo

no_castigo:
    mov rax, 0
    ret

si_castigo:
    mov rax, 1
    ret


section .note.GNU-stack noalloc noexec nowrite progbits
