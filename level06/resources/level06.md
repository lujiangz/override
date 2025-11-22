The programme's `auth()` function contains a hardcoded algorithm. The password (Serial) is generated deterministically based entirely on the entered Username (Login).

Algorithm Steps:
1.  **Length Check:** The login must be at least 6 characters long.
2.  **Seed (Initial) Value:** The 4th character (index 3) of the login string is XORed with `0x1337` and added to the constant `0x5eeded`.
3.  **Hash Loop:** For each character in the login string:
    - The character's ASCII value is XORed with the current hash value.
    - The result is modded by `0x539` (1337).
    - This mod value is added to the current hash value.
---

What we need to do is ensure that the serial value we write with the login matches. So let's write the Python script that performs this operation.

---

```python
import sys

def generate_serial(login):
    if len(login) < 6:
        return None
    hash = (ord(login[3]) ^ 0x1337) + 0x5eeded
    for c in login:
        if ord(c) < 32:
            return None
        hash += (ord(c) ^ hash) % 0x539
    return hash

if __name__ == "__main__":
    username = raw_input("Login girin: ")
    serial = generate_serial(username)
    if serial:
        print serial
```

