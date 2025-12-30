# SIC/XE Assembler ⚙️

A powerful **Two-Pass Assembler** for the SIC/XE (Simplified Instructional Computer / Extra Equipment) architecture, written in C++. This tool converts assembly language source code into object code ready for loading and execution.

## 🚀 Features
Based on the system architecture concepts:
- **Complete Instruction Support:** Handles Format 1, 2, 3, and 4 instructions.
- **Addressing Modes:** Supports Immediate (#), Indirect (@), and Indexed (,X) addressing.
- **Directives:** Processes `START`, `END`, `BYTE`, `WORD`, `RESB`, `RESW`, and `BASE`.
- **Output Generation:**
  - **Pass 1:** Generates the Symbol Table (SYMTAB) and calculates addresses (LOCCTR).
  - **Pass 2:** Generates the final Object Code (Header, Text, Modification, and End records).

## 📂 Project Structure
- `SICXE.cpp`: The main assembler source code.
- `INSTRUCTIONS.txt`: Configuration file containing the instruction set and opcodes.
- `REGISTERS.txt`: Configuration file for architecture registers.
- `SICXE_INPUT.txt`: Sample assembly program to test the assembler.

## 🛠️ How to Run
1. **Compile the code:**
   ```bash
   g++ SICXE.cpp -o assembler
