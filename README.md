# WoodCode

**Most Recent Stable Version**: `v1.4.0`\
**Latest Devolpment Version**: `v1.4.0`\
**Author**: Paul ([@Haha64142](https://github.com/Haha64142))

WoodCode is a custom, date-based encoding system for obfuscating strings. It supports letters, numbers, and select special characters, with output that varies based on the current date. The system is designed to be compact, and extensible.

See the full protocol specification for details:  
[WoodCode Protocol (Google Docs)](https://docs.google.com/document/d/1UjjNRBvrJHCDIoAVRDnQhwWGcTcVQZk7F2mc4qcpg7Y/edit?usp=sharing)

---

## ⚙️ Build Instructions

### You can download pre-built binaries in the [Releases tab](https://github.com/Haha64142/Wood-Code/releases)

---

### 🔧 Build Prerequisites

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
> You can install Bash using [Git for Windows](https://gitforwindows.com) (recommended), [MSYS2](https://www.msys2.org/), [Cygwin](https://www.cygwin.com/), or [WSL](https://learn.microsoft.com/en-us/windows/wsl/).\
> Then run `build.cmd` inside Command Prompt or `./build.sh` inside the installed terminal

---

### 🧱 Manual Build

```bash
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build/Release
```

---

Executables will be placed in:
`build/Release/bin/`

---

### ▶️ Running

- `WoodCodeApp.exe`: Interactive GUI-style terminal interface
- `woodcode.exe`: Command-Line Tool

---

## 🧪 Examples

### 🎛️ WoodCodeApp (Interactive)

```text
Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? e
Enter text to encode: Hello World!

Encoded text: W10400502154239234234246528144246258234540274564112925

Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? d
Enter text to decode: W10400502154239234234246528144246258234540274564112925

Decoded text: Hello World!
```

### 💻 woodcode (Command Line)

- `woodcode` for CMD or Powershell
- `./woodcode` for Bash

Print Usage and Commands

```bash
./woodcode --help        # Shows Usage
./woodcode encode 'Hello World!'
./woodcode decode W10400579231316311311323605221323335311538272562112925
```

> 💡 **Tip**: Use quotes around inputs with spaces or special characters.

You can also encode/decode from a file using `-f` or `--file`

---
