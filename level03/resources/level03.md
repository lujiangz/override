When we look at the code, the situation is as follows: The program does not directly encrypt our input. Actually, there is a weird string embedded in the code:

```
Q}|u`sfg~sf{}|a3
```

The program uses a number generated from our input as the "Key" and XORs this string with it. Our goal is to get the "Congratulations!" output as a result and drop to the shell.

## XOR Logic
Basic XOR logic is as follows:
- If `A ^ KEY = B` then `A ^ B = KEY`.

## Available Data
- **A (Encrypted Data)**: `0x51` (First character of the string 'Q')
- **B (Target Data)**: `0x43` (First character of 'Congratulations' 'C')

Via CyberChef or a simple calculation, when we do `0x51 XOR 0x43`, we see that the Key we are looking for is `0x12` which is **18**.

## Finding the Required Input
Now the issue is: What number should we give it so that the program produces this value of 18? Let's quickly look at the main function with GDB:

```
(gdb) disas main
...
0x080488ca <+112>:   movl   $0x1337d00d,0x4(%esp)   ; This is the 2nd parameter (Fixed Value)
0x080488d2 <+120>:   mov    %eax,(%esp)             ; This is the 1st parameter (Our Input)
0x080488d5 <+123>:   call   0x8048747 <test>
...
```

In the assembly output, we see that the second parameter going to the test function is a fixed hex value: `0x1337d00d`.
This value is **322424845** in decimal.

## Calculation
The formula inside the code was `param_2 - param_1`. So our equation is as follows:

**Fixed Value - Our Input = 18 should be.**

```
322424845 - Input = 18
```

From here, isolating the Input:

```
Input = 322424845 - 18
Input = 322424827
```

## Tools
- [CyberChef](https://lujiangz.github.io/cyberchef) - Can be used for XOR calculations.
