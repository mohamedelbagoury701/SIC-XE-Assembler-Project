#include <bits/stdc++.h>
using namespace std;

map<string, pair<int, string>> OPTAB; // Maps mnemonic // format // opcode
map<string, int> SYMTAB;              // Maps label // address
vector<pair<int, vector<string>>>
    prog;                         // Maps address // label // opcode // operand
map<string, string> REGS;         // Maps register // code
vector<pair<int, string>> obcode; // Maps address // object code
vector<int> modify;               // address of modification
string programName;               // program name
int LOCCTR = 0, base = 0, pc = 0,
    indx = 0;     // Location counter, base, program counter , index value
bool checkpc = 0; // check if pc relative

void printsymtab(); // Prints the symbol table
void ObjectFile();  // Writes the object program to a file in standard SIC/XE
                    // format
void getInput(string l, string *a, string *b,
              string *c); // Parses a line into label, opcode, and operand
int wrdsize(int i); // Returns the size (in bytes) of a WORD directive at line
int bytsize(int i); // Returns the size (in bytes) of a BYTE directive at line
int hextoint(string hexstring); // Converts a hexadecimal string to an integer
string inttohex(int x, int b);  // Converts an integer to a hexadecimal string
                                // with a specified width
string bintohex(bool a, bool b, bool c,
                bool d); // Converts four boolean flags to a hexadecimal string
string readdr(string res); // Calculate relative address to a hexadecimal string
string format2(int i);     // Generates object code for format 2 instructions
string format3(int i);     // Generates object code for format 3 instructions
string format4(int bb);    // Generates object code for format 4 instructions

int wrdsize(int i) {
  string s = prog[i].second[2];
  return s.length() / 2;
}

int bytsize(int i) {
  string adr = prog[i].second[2];
  if (adr[0] == 'C') {
    string s = adr.substr(2, adr.size() - 3);
    return s.size();
  } else if (adr[0] == 'X') {
    return 1;
  }
  return 0;
}

int hextoint(string hexstring) {
  int number = (int)strtol(hexstring.c_str(), NULL, 16);
  return number;
}

string inttohex(int x, int b) {
  string s;
  stringstream sstream;
  sstream << setfill('0') << setw(b) << hex << (int)x;
  s = sstream.str();
  sstream.clear();
  for (int i = 0; i < s.length(); i++)
    if (s[i] >= 97)
      s[i] -= 32;
  return s;
}

string bintohex(bool a, bool b, bool c, bool d) {
  string s;
  int sum = 0;
  sum += (int)a * 8;
  sum += (int)b * 4;
  sum += (int)c * 2;
  sum += (int)d * 1;
  s = inttohex(sum, 1);
  return s;
}

string readdr(string res) {
  int x = hextoint(res);
  if (x - pc > -2048 && x - pc < 2047) {
    checkpc = 1;
    return inttohex(x - pc, 3);
  } else {
    checkpc = 0;
    return inttohex(x - base, 3);
  }
}

string format2(int i) {
  string s, r1, r2 = "A", result;
  s = prog[i].second[2];
  int j;
  for (j = 0; j < s.size() && s[j] != ','; j++)
    ;
  r1 = s.substr(0, j);
  if (j < s.size())
    r2 = s.substr(j + 1, s.size() - j - 1);
  result = OPTAB[prog[i].second[1]].second;
  result += REGS[r1];
  result += REGS[r2];
  return result;
}

string format3(int i) {
  string adr = prog[i].second[2], res1, res2, res3;
  bool flags[6] = {}, dr = 0;
  int no = 0;
  if (adr[adr.size() - 1] == 'X' && adr[adr.size() - 2] == ',') {
    flags[2] = 1; // x flagg
    adr = adr.substr(0, adr.size() - 2);
  }
  if (adr[0] == '#') {
    flags[1] = 1; // immediate flagg
    adr = adr.substr(1, adr.size() - 1);
    if (SYMTAB.find(adr) != SYMTAB.end()) {
      res2 = inttohex(SYMTAB[adr], 3);
    } else {
      res2 = adr;
      dr = 1;
    }
    no = 1;
  } else if (adr[0] == '@') {
    flags[0] = 1; // indirect flagg
    adr = adr.substr(1, adr.size() - 1);
    no = 2;
    int z = SYMTAB[adr], j;
    for (j = 0; j < prog.size(); j++)
      if (prog[j].first == z)
        break;
    res2 = adr;
    adr = inttohex(prog[j].first, 3);

    if (prog[j].second[1] != "WORD" && prog[j].second[1] != "BYTE" &&
        prog[j].second[1] != "RESW" && prog[j].second[1] != "RESB") {
      adr = prog[j].second[2];
      z = SYMTAB[adr];
      for (j = 0; j < prog.size(); j++)
        if (prog[j].second[0] == res2)
          break;
      adr = prog[j].second[2];
      res2 = inttohex(SYMTAB[adr], 3);
    } else {
      res2 = adr;
    }
  } else if (adr[0] == '=') {
    adr = adr.substr(3, adr.size() - 4);
    dr = 1;
  } else {
    res2 = inttohex(SYMTAB[adr], 3);
    flags[0] = 1; //
    flags[1] = 1; // sicxe flagg
    no = 3;
  }
  if (dr != 1 && adr != "*") {

    res2 = readdr(res2);

    res2 = res2.substr(res2.size() - 3, 3);
    flags[4] = checkpc; // base flagg
    flags[3] = !checkpc;
  }
  if (flags[2] == 1) { // x flagg
    res2 = inttohex(hextoint(res2) - indx, 3);
  }
  while (res2.size() < 3)
    res2 = "0" + res2;
  res3 = OPTAB[prog[i].second[1]].second;
  res3 = inttohex(hextoint(res3) + no, 2) +
         bintohex(flags[2], flags[3], flags[4], flags[5]) + res2;
  return res3;
}

string format4(int bb) {
  string z = prog[bb].second[2], te = prog[bb].second[1], TA = "", obcode;
  int no = 0;
  bool nixbpe[6] = {0, 0, 0, 0, 0, 0};
  nixbpe[0] = (z[0] == '@');
  nixbpe[1] = (z[0] == '#');
  if (nixbpe[0] == nixbpe[1]) {
    nixbpe[0] = !nixbpe[0]; // n
    nixbpe[1] = !nixbpe[1]; // i
  }
  nixbpe[2] = (z[z.length() - 1] == 'X' && z[z.length() - 2] == ',') ? 1 : 0;
  nixbpe[3] = 0;
  nixbpe[4] = 0;
  nixbpe[5] = 1; // e flagg
  if (z[0] == '@' || z[0] == '#')
    z = z.substr(1, z.length() - 1);
  if (z[z.length() - 1] == 'X' && z[z.length() - 2] == ',')
    z = z.substr(0, z.length() - 2);
  if (nixbpe[0] == 1 && nixbpe[1] == 1) {
    string s = inttohex(SYMTAB[z], 5);
    for (int i = 0; i < prog.size(); i++) {
      if (inttohex(prog[i].first, 5) == s) {
        if (nixbpe[2] == 0)
          TA = s;
        else
          TA = inttohex(hextoint(s) + indx, 5);
      }
    }
    no = 3;
  } else if (nixbpe[0] == 1 && nixbpe[1] == 0 && nixbpe[2] == 0) {
    string s = to_string(SYMTAB[z]);
    for (int i = 0; i < prog.size(); i++)
      if (to_string(prog[i].first) == s) {
        s = prog[i].second[2];
        for (int j = 0; i < prog.size(); j++)
          if (to_string(prog[j].first) == s)
            TA = prog[j].second[2];
      }
    no = 2;
  } else if (nixbpe[0] == 0 && nixbpe[1] == 1) {

    if (z[0] < 65)
      TA = inttohex(stoi(z), 5);
    else
      TA = inttohex(SYMTAB[z], 5);
    no = 1;
  }
  string res3 =
      OPTAB[prog[bb].second[1].substr(1, prog[bb].second[1].size() - 1)].second;

  res3 = inttohex(hextoint(res3) + no, 2) +
         bintohex(nixbpe[2], nixbpe[3], nixbpe[4], nixbpe[5]) + TA;
  return res3;
}

void getInput(string l, string *a, string *b,
              string *c) { // label // opcode // operand
  string x, y, z;

  if (l[0] == ' ')
    x = "*";
  else {
    int j = 0;
    for (; j < l.size() && l[j] != ' '; j++)
      ;
    x = l.substr(0, j);
  }
  l = l.substr(x.size() + 1, l.size() - x.size() - 1);
  int j = 0;
  for (; j < l.size() && l[j] != ' '; j++)
    ;
  y = l.substr(0, j);
  l = l.substr(y.size() + 1, l.size() - y.size() - 1);
  if (l[0] == ' ')
    z = "*";
  else {
    z = l;
  }

  (*a) = x;
  (*b) = y;
  (*c) = z;
}
string format5(int i) {
  string s = prog[i].second[2];
  vector<string> regs;
  size_t start = 0, end;
  while ((end = s.find(',', start)) != string::npos) {
    regs.push_back(s.substr(start, end - start));
    start = end + 1;
  }
  regs.push_back(s.substr(start));
  vector<pair<string, string>> it;
  it.push_back({"A", "0"});
  it.push_back({"X", "1"});
  it.push_back({"L", "2"});
  it.push_back({"B", "3"});
  it.push_back({"S", "4"});
  it.push_back({"T", "5"});
  it.push_back({"F", "6"});
  it.push_back({"PC", "8"});
  it.push_back({"SW", "9"});
  while (regs.size() < 4)
    regs.push_back("A");
  string opcode = OPTAB[prog[i].second[1]].second;
  string r1;
  for (const auto &reg : it) {
    if (reg.first == regs[0]) {
      r1 = reg.second;
      break;
    }
  }
  string r2;
  for (const auto &reg : it) {

    if (reg.first == regs[1]) {
      r2 = reg.second;
      break;
    }
  }
  string r3;
  for (const auto &reg : it) {

    if (reg.first == regs[2]) {
      r3 = reg.second;
      break;
    }
  }
  string r4;
  for (const auto &reg : it) {

    if (reg.first == regs[3]) {
      r4 = reg.second;
      break;
    }
  }
  if (r1.empty())
    r1 = "0";
  if (r2.empty())
    r2 = "0";
  if (r3.empty())
    r3 = "0";
  if (r4.empty())
    r4 = "0";

  string result = opcode + r1 + r2 + r3 + r4;
  return result;
}

void ObjectFile() {
  freopen("SIC_XE_Out.txt", "w", stdout);
  int sz = obcode.size();
  cout << "H^" << programName;
  for (int j4 = 0; j4 < 6 - programName.size(); j4++)
    cout << " ";
  cout << "^" << inttohex(obcode[0].first, 6) << "^" << inttohex(LOCCTR, 6)
       << endl;

  for (int i = 0; i < obcode.size(); i += 5) {
    long long sum = 0;
    for (int j = i; j < i + min(sz - i, 5); j++) {
      sum += obcode[j].second.size() / 2;
    }
    cout << "T^" << inttohex(obcode[i].first, 6) << "^" << inttohex(sum, 2);

    for (int j = i; j < i + min(sz - i, 5); j++) {
      cout << "^" << obcode[j].second;
    }
    cout << endl;
  }
  for (int i = 0; i < modify.size(); i++)
    cout << "M^" << inttohex(modify[i] + 1, 6) << "^05" << endl;
  cout << "E^" << inttohex(obcode[0].first, 6);
  fclose(stdout);
}

void printsymtab() {
  freopen("SICXE_SYMBOL_TABLE.txt", "w", stdout);
  vector<pair<string, int>> sortedSymtab(SYMTAB.begin(), SYMTAB.end());
  sort(sortedSymtab.begin(), sortedSymtab.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         return a.second < b.second;
       });
  cout << "+------------+------------+" << endl;
  cout << "| " << left << setw(10) << "Symbol" << " | " << setw(10) << "Address"
       << " |" << endl;
  cout << "+------------+------------+" << endl;
  for (auto it : sortedSymtab)
    if (it.first[0] != '+' && it.second != 0)
      cout << "| " << left << setw(10) << it.first << " | " << setw(10)
           << inttohex(it.second, 4) << " |" << endl;
  cout << "+------------+------------+" << endl;
  fclose(stdout);
}

int main() {

  // Loads OPTAB and REGS from files
  ifstream input1;
  do {
    input1.open("SICXE_INPUT.txt");
    if (input1.fail())
      cout << "Not Found" << endl;
  } while (input1.fail());
  freopen("INSTRUCTIONS.txt", "r", stdin);
  string x, y, z;
  int a;
  for (int i = 0; i < 64; i++) {
    cin >> x >> a >> z;
    OPTAB[x] = {a, z};
  }
  freopen("REGISTERS.txt", "r", stdin);
  for (int i = 0; i < 9; i++) {
    cin >> x >> y;
    REGS[x] = y;
  }

  // Pass 1;
  string l;
  ifstream input("SICXE_INPUT.txt");
  while (getline(input, l)) {
    getInput(l, &x, &y, &z);
    if (x[0] == '.')
      continue;
    prog.push_back({LOCCTR, vector<string>()});
    prog.back().second.push_back(x);
    prog.back().second.push_back(y);
    prog.back().second.push_back(z);
    if (x != "*") { // label putting in symtab
      SYMTAB[x] = LOCCTR;
    }
    if (y == "RESW") {
      LOCCTR += stoi(z) * 3;
    } else if (y == "RESB") {
      LOCCTR += stoi(z);
    } else if (y == "WORD") {
      LOCCTR += wrdsize(prog.size() - 1);
    } else if (y == "BYTE") {
      LOCCTR += bytsize(prog.size() - 1);
    } else if (y[0] == '+') {
      LOCCTR += 4;
    } else if (OPTAB.find(y) != OPTAB.end()) {
      LOCCTR += OPTAB[y].first;
    }
  }

  // Pass 2
  for (int i = 0; i < prog.size(); i++) {
    string s;
    int j;
    for (j = i; j < prog.size(); j++) {
      if (prog[j].first != prog[i].first)
        break;
    }
    pc = prog[j].first;
    string s2 = prog[i].second[1];
    int format = OPTAB[prog[i].second[1]].first;
    if (format == 1) {
      s = OPTAB[prog[i].second[1]].second;
    } else if (format == 2) {
      s = format2(i);
    } else if (format == 5) {
      s = format5(i);
    } else if (format == 3) {
      s = format3(i);
    } else if (s2[0] == '+') {
      s = format4(i);
      string tt = prog[i].second[2];
      if (tt[0] != '#')
        modify.push_back(prog[i].first);
    }
    if (prog[i].second[1] == "BASE") {
      base = SYMTAB[prog[i].second[2]];
    }
    if (prog[i].second[2] == "LDX") {
      x = SYMTAB[prog[i].second[2]];
    }
    if (prog[i].second[1] == "NOBASE") {
      base = 0;
    } else if (prog[i].second[1] == "BYTE") {
      string adr = prog[i].second[2];
      s = adr.substr(2, adr.size() - 3);
      string s3 = "";
      for (int j = 0; j < s.size(); j++) {
        s3 += inttohex(s[j], 2);
      }
      if (adr[0] == 'C')
        s = s3;
    }

    prog[i].second.push_back(s);
  }
  // Outputs the location listing, symbol table, and object file
  ofstream outputFile("SICXE_Output_LOCATION.txt");
  for (int i = 0; i < prog.size(); i++) {
    if (prog[i].second[0] == "*") {
      if (prog[i].second[2].length() == 8)
        outputFile << inttohex(prog[i].first, 4) << "\t\t" << prog[i].second[1]
                   << "\t" << prog[i].second[2] << "  " << prog[i].second[3]
                   << endl;
      else if (prog[i].second[2] == "*")
        outputFile << inttohex(prog[i].first, 4) << "\t\t" << prog[i].second[1]
                   << "\t" << " " << "\t  " << prog[i].second[3] << endl;
      else
        outputFile << inttohex(prog[i].first, 4) << "\t\t" << prog[i].second[1]
                   << "\t" << prog[i].second[2] << "\t  " << prog[i].second[3]
                   << endl;

    } else
      outputFile << inttohex(prog[i].first, 4) << "\t" << prog[i].second[0]
                 << "\t" << prog[i].second[1] << "\t" << prog[i].second[2]
                 << "\t  " << prog[i].second[3] << endl;

    if (i == 0)
      programName = prog[i].second[0];
    else if (prog[i].second[3].length() != 0)
      obcode.push_back({prog[i].first, prog[i].second[3]});
  }
  outputFile.close();
  printsymtab();
  ObjectFile();
  return 0;
}
