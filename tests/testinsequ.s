.section .data:
.equ data_label, -0x6+1+2+0b110-23+0x12+0x1F-41+0xC
.equ pom_label, a_text-b_text+d_text-c_text+0xF-28+0xB
.section .text:
iret
mov  a_text,c_text(%r7)
a_text: mov  $0x7FFF,pom_label(%pc)
add  $111,b_text
jmp a_text
jgt 0x5
mul (%r3),%r2
pop 0b110
push (%r4)
push %psw
jne *%r5
jmp *(%r7)
mov 0xA,(%r3)
pop 0x5A
cmp  (%r3),(%r6)
b_text:
not $0x55 , data_label
ret
and $0xfab , c_text
push $12
int bss_label
call c_text
xchg  $0b111, pom_label
c_text: push $bss_label
jmp *c_text(%r3)
jmp *0b101(%r2)
jne *data_label(%pc)
jmp 0xA
xor $pom_label,bss_label
not  0b11(%r1),0x41(%r2)
shl b_text,(%r4)
not %r1,(%r6)
jgt d_text
shr (%r7),c_text
d_text: test bss_label(%pc),%r7
halt
.section .bss:
.equ bss_label, pom_label-data_label+21+0b1011
.end