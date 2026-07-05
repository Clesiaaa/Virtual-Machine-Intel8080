# Intel 8080 Emulator (Work in Progress)

A complete Intel 8080 microprocessor emulator with a virtual filesystem, an assembler, and a vim-like text editor.

![Version](https://img.shields.io/badge/version-2.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![CPU](https://img.shields.io/badge/CPU-Intel%208080-red)

---

## Features

### 8080 Emulation
- All 256 opcodes implemented
- Full instruction set: MOV, MVI, ADD, SUB, JMP, CALL, etc.
- Flag management: Zero, Sign, Parity, Carry, Auxiliary Carry
- 64KB addressable memory
- Accurate cycle timing for each instruction
- Stack operations with SP register
- I/O ports: IN and OUT instructions
- Interrupt support: EI, DI, RST

### Virtual Filesystem
- Persistent storage — files survive between sessions
- Directory hierarchy for organizing projects
- Unix-like commands: ls, cd, mkdir, touch, del
- Import files from the host system
- Dynamic prompt showing the current directory

### Built-in Text Editor
- Vim-like interface with familiar commands
- Save with `:w`
- Quit with `:q`
- Save and quit with `:wq`
- Line editing: append and delete
- Integrated with the filesystem

### Assembler
- Full 8080 assembly support
- All mnemonics: 74 instruction types
- Multiple numeric formats: decimal, hexadecimal, octal
- Comment support with `;`
- Direct compilation from the filesystem

### Example Programs
- Hello World — classic first program
- Factorial — calculates the factorial of 4
- Sum — computes the sum from 0 to 10
- Preloaded in `/root/examples`

---

## Installation

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install
```

### Build

```bash
# Clone the repository
git clone https://github.com/Clesiaaa/Virtual-Machine-Intel8080.git
cd Virtual-Machine-Intel8080

# Compile
make

# Run
./emulator
```

---

## Quick Start

### 1. First Launch

```
$ ./emulator

booting....
Filesystem initialized

root>
```

### 2. Explore Examples

```
root> cd examples
root/examples> ls

Directory: root/examples
================================================
  [FILE] hello.asm                       567 B
================================================
  1 item(s)
```

### 3. Compile and Run

```
root/examples> compile
source file (.asm): hello.asm
output file (.hex): hello.hex
Compilation complete: hello.hex

root/examples> exec
file: hello.hex
ROM loaded: 29 bytes
HELLO WORLD
```

---

## Command Reference

### System Commands

| Command | Description | Example |
|---------|-------------|---------|
| `help` | Show all commands | `help` |
| `clear` | Clear the screen | `clear` |
| `exit` | Save and quit | `exit` |

### Filesystem Commands

| Command | Description | Example |
|---------|-------------|---------|
| `ls` | List files in current directory | `ls` |
| `cd <dir>` | Change directory | `cd examples` |
| `cd ..` | Go to parent directory | `cd ..` |
| `mkdir` | Create directory | `mkdir myproject` |
| `touch` | Create empty file | `touch test.asm` |
| `del` | Delete file or directory | `del oldfile.hex` |
| `cat` | Display file contents | `cat hello.asm` |
| `editor` | Edit file (vim-like) | `editor hello.asm` |
| `import` | Import from host system | `import` |

### Emulator Commands

| Command | Description | Example |
|---------|-------------|---------|
| `load` | Load ROM into memory | `load program.hex` |
| `run` | Execute loaded ROM | `run` |
| `exec` | Load and execute ROM | `exec hello.hex` |
| `compile` | Compile assembly to hex | `compile` |

---

## Using the Editor

The emulator includes a vim-like text editor.

### Opening a File

```
root> editor hello.asm
```

### Editor Commands

| Command | Description |
|---------|-------------|
| `:w` | Save file |
| `:q` | Quit editor |
| `:wq` | Save and quit |
| `a` | Append new line |
| `d NUM` | Delete line NUM |

### Example Session

```
=== EDITOR: hello.asm ===
Press ESC then :w to save, :q to quit, :wq to save and quit
--------------------------------------------------

  1 | ; Hello World
  2 | MVI A, 72
  3 | OUT 1
  4 | HLT

--------------------------------------------------
Commands:
  :w    - Save
  :q    - Quit
  :wq   - Save and quit
  a     - Append new line
  d NUM - Delete line NUM

> a
Enter line (empty to finish):
MVI A, 69
Line added

> :wq
File saved
```

---

## Assembly Language

### Syntax

```assembly
; Comments start with a semicolon
LABEL:
    INSTRUCTION OPERAND1, OPERAND2
```

### Example: Hello World

```assembly
; Hello World for Intel 8080
MVI A, 72    ; 'H'
OUT 1
MVI A, 69    ; 'E'
OUT 1
MVI A, 76    ; 'L'
OUT 1
MVI A, 76    ; 'L'
OUT 1
MVI A, 79    ; 'O'
OUT 1
MVI A, 10    ; '\n'
OUT 1
HLT
```

### Supported Instructions

**Data Transfer**
`MOV`, `MVI`, `LXI`, `LDA`, `STA`, `LHLD`, `SHLD`, `LDAX`, `STAX`

**Arithmetic**
`ADD`, `ADC`, `SUB`, `SBB`, `INR`, `DCR`, `INX`, `DCX`, `DAD`

**Logical**
`ANA`, `XRA`, `ORA`, `CMP`, `ANI`, `XRI`, `ORI`, `CPI`

**Branch**
`JMP`, `JZ`, `JNZ`, `JC`, `JNC`, `JP`, `JM`, `JPE`, `JPO`

**Stack**
`PUSH`, `POP`, `XTHL`, `SPHL`

**I/O**
`IN`, `OUT`

**Control**
`HLT`, `NOP`, `EI`, `DI`, `RST`

---

## Filesystem Structure

The emulator maintains a persistent virtual filesystem stored in `.emulator_fs.dat`.

### Default Structure

```
root/
└── examples/
    └── hello.asm       - Hello World program
```

### Creating a Project

```
root> mkdir myproject
root> cd myproject
root/myproject> mkdir src
root/myproject> mkdir bin
root/myproject> cd src
root/myproject/src> touch main.asm
root/myproject/src> editor main.asm
```

---

## Importing Files

You can import assembly files from your host system:

```
root> import
host path: /home/user/programs/test.asm
name in filesystem: test.asm
Imported 'test.asm' from host
```

The file is then available in the virtual filesystem and persists between sessions.

---

## Workflow Example

### Complete Development Cycle

```bash
# 1. Create project structure
root> mkdir calculator
root> cd calculator
root/calculator> mkdir src
root/calculator> mkdir build

# 2. Write code
root/calculator> cd src
root/calculator/src> touch add.asm
root/calculator/src> editor add.asm
# ... write your assembly code ...
> :wq

# 3. Compile
root/calculator/src> compile
source file (.asm): add.asm
output file (.hex): ../build/add.hex
Compilation complete: ../build/add.hex

# 4. Execute
root/calculator/src> cd ../build
root/calculator/build> exec
file: add.hex
ROM loaded: 15 bytes
8
```

---

## Architecture

### CPU Structure

```c
typedef struct {
    uint8_t A, B, C, D, E, H, L;    // 8-bit registers
    uint16_t PC;                     // Program Counter
    uint16_t SP;                     // Stack Pointer
    FLAGS f;                         // Flags register
    uint8_t memory[0x10000];         // 64KB RAM
    uint64_t cycles;                 // Cycle counter
    uint8_t interrupt_enable;        // Interrupt state
} cpu;
```

### Flags

| Bit | Flag | Description |
|-----|------|-------------|
| 0   | CY   | Carry flag |
| 2   | P    | Parity flag |
| 4   | AC   | Auxiliary Carry (BCD) |
| 6   | Z    | Zero flag |
| 7   | S    | Sign flag |

### Memory Map

```
0x0000 - 0xFFFF    RAM (64KB)
```

For Space Invaders compatibility:
```
0x2000 - 0x3FFF    ROM
0x2400 - 0x3FFF    Video RAM (7KB)
```

---

## Development

### Adding New Features

To add a new instruction:

1. Open `src/execution_engine/instructions.c`
2. Add your case in the switch statement
3. Update the cycle table
4. Recompile with `./compile.sh`

---

## Testing

### Test Hello World

```bash
root> cd examples
root/examples> compile
source file (.asm): hello.asm
output file (.hex): hello.hex

root/examples> exec
file: hello.hex
HELLO WORLD
```

### Test Sum

```bash
root/examples> exec
file: sum.hex
55
```

---

## Performance

On a modern Intel i7:
- Emulation speed: ~50-100 MHz (25-50x faster than a real 8080)
- Instruction overhead: ~20 nanoseconds
- Memory usage: ~100KB (64KB RAM + structures)

---

## Troubleshooting

### Filesystem Won't Load

```bash
# Remove corrupted filesystem
rm .emulator_fs.dat

# Restart emulator
./emulator
```

### Compilation Errors

Check your assembly syntax:
- Instructions must be uppercase
- Operands separated by commas
- Values can be decimal, hex (0x), or octal (0)

### ROM Won't Execute

Verify:
1. File exists: `ls`
2. File is in `.hex` format: `cat filename.hex`
3. ROM is loaded: check for the "ROM loaded" message

---

## Contributing

Contributions are welcome. Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

---

## License

MIT License — see LICENSE file for details.

---

## Acknowledgments

- Intel for the 8080 processor
- The retro computing community
- All contributors

---

## Contact

**Author**: Robert Folga

**Issues**: Please report bugs on GitHub

**Mail**: robert.folga@ens.uvsq.fr

---

## Resources

### Documentation
- [Intel 8080 Datasheet](http://www.nj7p.org/Manuals/PDFs/Intel/9800301D_8080_8080A_Instruction_Set.pdf)
- [8080 Instruction Reference](http://www.emulator101.com/reference/8080-by-opcode.html)
- [Assembly Programming Guide](http://www.emulator101.com/)

### Similar Projects
- [8080 Online Assembler](http://www.asm80.com/)
