# SIC/XE Assembler ⚙️

A robust **Two-Pass Assembler** for the SIC/XE (Simplified Instructional Computer / Extra Equipment) architecture, implemented in C++. This tool parses assembly source code and generates machine-executable object code, handling complex addressing modes and instruction formats.

## 🚀 Features

### Core Functionality
- **Two-Pass Architecture:**
  - [cite_start]**Pass 1:** Constructs the Symbol Table (SYMTAB) and assigns addresses to all statements[cite: 74].
  - [cite_start]**Pass 2:** Assembles instructions and generates the final Object Code[cite: 74].

### Instruction Support
- [cite_start]**Full Format Support:** Handles Format 1, 2, 3, and 4 instructions[cite: 45, 48, 53].
- **Addressing Modes:**
  - [cite_start]Immediate Addressing (`#`)[cite: 51].
  - [cite_start]Indirect Addressing (`@`)[cite: 51].
  - [cite_start]Indexed Addressing (`X`)[cite: 51].
- [cite_start]**Program Relocation:** Supports Program Counter (PC) relative and Base relative addressing[cite: 43].

### System Directives
- [cite_start]Processes standard SIC/XE directives including `START`, `END`, `BYTE`, `WORD`, `RESB`, `RESW`, and `BASE`[cite: 90].

---

## 📂 File Structure

| File Name | Description |
|-----------|-------------|
| `SICXE.cpp` | Main source code containing Pass 1 & Pass 2 logic. |
| `INSTRUCTIONS.txt` | [cite_start]Configuration file defining the Instruction Set (OPTAB)[cite: 87]. |
| `REGISTERS.txt` | [cite_start]Configuration file defining architecture registers[cite: 89]. |
| `SICXE_INPUT.txt` | [cite_start]Sample assembly source code for testing[cite: 90]. |
| `SIC_XE_Out.txt` | [cite_start]The generated Object Code (H, T, M, E records)[cite: 76]. |
| `SICXE_SYMBOL_TABLE.txt` | [cite_start]The generated Symbol Table showing labels and addresses[cite: 92]. |

---

## 🛠️ How to Build & Run

### Prerequisites
- A C++ Compiler (GCC/G++ recommended).
- Ensure `INSTRUCTIONS.txt` and `REGISTERS.txt` are in the same directory as the executable.

### Installation
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/mohamedelbagoury701/SIC-XE-Assembler.git](https://github.com/mohamedelbagoury701/SIC-XE-Assembler.git)
   cd SIC-XE-Assembler
