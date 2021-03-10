.global main
.extern ext_jedan
.extern ext_dva
.extern ext_tri


.section .data:
data_niz_byte:
    .byte 1,2,3,4,5,0b1,0b10,0b11,0b100,0b101,0x1,0x2,0x3,0x4,0x5
data_niz_word: .word -1,-2,-3,-4,-5,-0b1,-0b10,-0b11,-0b100,-0b101,-0X1,-0X2,-0X3,-0X4,-0X5
eksterni_simboli_data: .byte a_txt, ext_jedan
   ispis_Rezultata: .byte ext_tri, 0x0F, 7

.word lista_byte_bss
   broj_Elem: .skip 0
.section .text:
main:
push %psw
a_txt: 

    mov %r0, ext_tri
     pop %r4
     jmp eksterni_simboli_data
    mov nestoiz_txt, data_niz_byte(%pc)
     add %r5, main
  nestoiz_txt:   jgt b_txt

     and %r1, data_niz_byte
      xor %r4, kraj
     mov %r0, ispis_Rezultata
     jne *nestoiz_txt

     or 0b11(%r2), kraj
     not %r0, 13(%r4)
     jmp *0xf1

    b_txt:
    pop %psw
    shr %r0,5
 c_text:  
  push lista_byte_bss(%pc)
     mov kraj(%r1), %r0
     jne broj_Elem
     pop 0x5F(%r5)
      and %r0, data_niz_byte
data_txt:
     xor %r0, kraj
       test %r0, (%r4)
       iret
     ret
     shr %r0,5

.equ ekulabela, prvalista_bss-lista_byte_bss+data_txt
 kraj:   halt

.section .bss:
lista_byte_bss:
.byte 0b11111111,0b11111110,0xFF,0xfe
prvalista_bss:
.equ equ_bss,kraj-data_txt+c_text-b_txt+0x1+2+0b11-0x4-21
.end