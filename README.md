# WoodCode

**Version**: v1.3.5  
**Author**: Paul (Haha64142)

WoodCode is a custom, date-based encoding system for obfuscating strings. It supports letters, numbers, and select special characters, with output that varies based on the current date. The system is designed to be compact, and extensible.

📄 See the full protocol specification for details:  
[WoodCode Protocol](https://docs.google.com/document/d/1UjjNRBvrJHCDIoAVRDnQhwWGcTcVQZk7F2mc4qcpg7Y/edit?usp=sharing)

---

## ⚙️ Build Instructions

### Using g++

```bash
g++ main.cpp woodcode.cpp -o WoodCode.exe
```

### Using CMake + Ninja

```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
```

🛠 Make sure you have `cmake` and `ninja` installed.

### Running

- Make sure to compile using either of the methods above
- Run the file `WoodCode.exe` (It will be inside the build folder)

---

## 🧪 Example

```text
Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? e
Enter text to encode: PaulW

Encoded text: W10305086161172171081071125
```

The encoded string includes:
- A version header (`W10305`)
- Encoded 3-digit chunks
- A 6-digit date used for offset

---
