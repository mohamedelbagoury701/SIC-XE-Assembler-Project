#include <bits/stdc++.h>
#include <cstdio>
using namespace std;

struct SymbolEntry {
  string symbol;
  string address;
};

unordered_set<string> loadSicInstructions(const string &filename) {
  unordered_set<string> instructions;
  ifstream file(filename);
  string line;

  if (!file.is_open()) {
    cerr << "Warning: Could not open instruction file '" << filename << "'"
         << endl;
    return instructions;
  }

  while (getline(file, line)) {
    istringstream iss(line);
    string opcode;
    iss >> opcode;
    if (!opcode.empty()) {
      instructions.insert(opcode);
      if (opcode == "JSUB") {
        instructions.insert("+" + opcode);
      }
    }
  }

  file.close();
  return instructions;
}

vector<SymbolEntry>
extractSymbolTable(const string &filename,
                   const unordered_set<string> &instructions) {
  vector<SymbolEntry> symbolTable;

  ifstream file(filename);
  string line;

  if (!file.is_open()) {
    cerr << "Error: Could not open file '" << filename << "'" << endl;
    return symbolTable;
  }

  while (getline(file, line)) {
    istringstream iss(line);
    string address, symbol, opcode, operand;

    iss >> address >> symbol >> opcode >> operand;

    if (!symbol.empty() && instructions.find(symbol) == instructions.end()) {
      // Add to symbol table
      symbolTable.push_back({symbol, address});
    }
  }

  file.close();
  return symbolTable;
}

int main() {
  string instructionFile = "/home/bagoury/Desktop/SIC_XE/INSTRUCTIONS.txt";
  unordered_set<string> instructions = loadSicInstructions(instructionFile);

  instructions.insert("START");
  instructions.insert("BASE");
  instructions.insert("END");
  instructions.insert("RESW");
  instructions.insert("RESB");
  instructions.insert("BYTE");

  string sourceFile = "/home/bagoury/Desktop/SIC_XE/SICXE_Output_LOCATION.txt";
  vector<SymbolEntry> symbolTable =
      extractSymbolTable(sourceFile, instructions);

  freopen ("/home/bagoury/Desktop/SIC_XE/SICXE_SYMBOL_TABLE.txt","w", stdout);
  cout << "+------------+------------+" << endl;
  cout << "| " << left << setw(10) << "Symbol" << " | " << setw(10) << "Address"
       << " |" << endl;
  cout << "+------------+------------+" << endl;

  for (const auto &entry : symbolTable) {
    if (entry.symbol[0] == '+' || entry.address == "0000") {
      continue;
    }
    cout << "| " << left << setw(10) << entry.symbol << " | " << setw(10)
         << entry.address << " |" << endl;
  }

  cout << "+------------+------------+" << endl;
  
  fclose(stdout);
  return 0;
}