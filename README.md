# WoodCode

**Version**: v1.3.5  
**Author**: Paul (Haha64142)

WoodCode is a custom, date-based encoding system for obfuscating strings. It supports letters, numbers, and select special characters, with output that varies based on the current date. The system is designed to be compact, and extensible.

See the full protocol specification for details:  
[WoodCode Protocol (Google Docs)](https://docs.google.com/document/d/1UjjNRBvrJHCDIoAVRDnQhwWGcTcVQZk7F2mc4qcpg7Y/edit?usp=sharing)

---

## ⚙️ Build Instructions

### 🔧 Prerequisites

Make sure you have the following installed and added to your system's `PATH`:

- [CMake](https://cmake.org/download/) ≥ 3.10
- [Ninja](https://github.com/ninja-build/ninja/releases) (build system)
- C++17-compatible compiler
    - `g++`
    - `clang++`
    - `MSVC` (Visual Studio)

---

### 🛠 Using build script

Default Build (Release)

```bash
./build.sh
```

Show Usage

```bash
./build.sh --help
```

> 💡 **Windows users without Bash:**\
> You can install Bash using [Git for Windows](https://gitforwindows.com) (recommended), [MSYS2](https://www.msys2.org/), or [Cygwin](https://www.cygwin.com/).\
> Then run the script from the Bash terminal.

---

### 🧱 Manual Build

```bash
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build/Release
```

Executables will be placed in:
`build/Release/bin/`

---

### ▶️ Running

- `WoodCodeApp.exe`: Interactive GUI-style terminal interface
- `woodcode.exe`: Command-Line Tool

---

## 🧪 Examples

### WoodCodeApp (Interactive)

```text
Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? e
Enter text to encode: Hello World!

Encoded text: W10305095180175175187469085187199175177467071525

Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? d
Enter text to decode: W10305095180175175187469085187199175177467071525

Decoded text: Hello World!
```

### woodcode (Command Line)

- `woodcode` for CMD or Powershell
- `./woodcode` for Bash

Print Usage and Commands

```bash
./woodcode --help        # Shows Usage
./woodcode encode 'Hello World!'
./woodcode decode W10305095180175175187469085187199175177467071525
```
> 💡 Use quotes around input with spaces or special characters.

You can also encode/decode from a file using `-f` or `--file`

---
