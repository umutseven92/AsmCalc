#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <stdexcept>
#include <string>
#include <stack>
#include <map>

using namespace std;

vector<string> *ReadFromFile(string path);

string RemoveSpaces(string input);

string GetMovAssignment(string op, string reg, string val, bool isReg = false);

string GetAvailableRegister(vector<tuple<bool, string, double> > &registers);

void FillRegister(vector<tuple<bool, string, double> > &registers, string reg, double val);

void EmptyRegister(vector<tuple<bool, string, double> > &registers, string reg);

string CheckForOperations(string val, vector<string> &results, vector<tuple<bool, string, double> > &registers);

int main() {
    vector<string> result;
    vector<tuple<bool, string, double> > registers =
            {
                    make_tuple(false, "ax", NAN),
                    make_tuple(false, "bx", NAN),
                    make_tuple(false, "cx", NAN),
                    make_tuple(false, "dx", NAN)
            };

    vector<string> statements = *(ReadFromFile(""));

    // Generate asm code
    long size = statements.size();
    for (int i = 0; i < size; ++i) {
        string st = RemoveSpaces(statements[i]);

        for (int j = 0; j < st.size(); ++j) {
            if (st[j] == '=') {
                // Assignment
                string val = st.substr(j + 1, st.size());

                /*
                string resReg = CheckForOperations(val, result, registers);
                string reg = GetAvailableRegister(registers);
                FillRegister(registers, reg, NAN);
                string r = GetMovAssignment("mov", reg, resReg, true);
                result.push_back(r);
                 */
            } else {
                // Print statement
                /*
                vector<int> opParanPos;
                vector<int> clParanPos;
                int mode = -1;
                int opParan = 0;
                int clParan = 0;
                vector<string> values;
                for (int k = 0; k < st.size(); ++k) {
                    if(st[k] == '(')
                    {
                        opParanPos.push_back(k);
                        opParan++;
                        mode = 0;
                    }
                    else if(st[k] == ')')
                    {
                        clParanPos.push_back(k);
                        clParan++;

                        if(mode == 0)
                        {
                            string val = st.substr(opParanPos.back() + 1, k-opParanPos.back()-1);

                            for (int l = 0; l < val.size(); ++l) {
                                if(val[l] == '+' || val[l] == '-' || val[l] == '*' || val[l] == '/')
                                {
                                    char op = val[l];
                                    string first = val.substr(0, l-1);
                                    string second = val.substr(l);
                                    string postFix = first + " " + second + " " + op;
                                    result.push_back("Ins. to " + val);
                                }
                            }
                            values.push_back(val);
                        }
                        mode = 1;

                    }

                }
                 */
            }
        }
    }

    // Print result
    for (int k = 0; k < result.size(); ++k) {
        cout << result[k] << endl;
    }

    return 0;
}


vector<char> CovertToPostfix(string input)
{
    map<char, int> opPrecedence = {
            {'*', 2},
            {'/', 2},
            {'+', 1},
            {'-', 1},
            {'(', 0},
            {')', 0},
    };

    vector<char> output;
    stack<char> opstack;

    int size = input.size();

    for (int i = 0; i < size; ++i) {
        char c = input[i];

        switch (c) {

            case '+':
            case '-':
            case '*':
            case '/': {
                while (!opstack.empty() && opPrecedence[opstack.top()] >= opPrecedence[c]) {
                    char val = opstack.top();
                    output.push_back(val);
                    opstack.pop();
                }
                opstack.push(c);
            }
                break;

            case '(':
                opstack.push(c);
                break;

            case ')': {

                char token2 = opstack.top();
                opstack.pop();

                while (token2 != '(') {
                    output.push_back(token2);
                    token2 = opstack.top();
                    opstack.pop();
                }
                break;
            }
            default:
                if (c != ' ') {
                    // Assume that whats remaining is a letter or a digit
                    output.push_back(c);
                }
                break;
        }
    }

    while (!opstack.empty()) {
        char val = opstack.top();
        output.push_back(val);
        opstack.pop();
    }

    return output;
}

string CheckForOperations(string val, vector<string> &results, vector<tuple<bool, string, double> > &registers) {
    for (int i = 0; i < val.size(); ++i) {
        switch (val[i]) {
            case '+': {
                string reg = GetAvailableRegister(registers);
                string v1 = string(1, val[i - 1]);
                string v2 = string(1, val[i + 1]);

                FillRegister(registers, reg, stod(v1));

                string r = GetMovAssignment("mov", reg, v1);
                results.push_back(r);
                string r2 = GetMovAssignment("add", reg, v2);
                results.push_back(r2);
                FillRegister(registers, reg, stod(v1));

                return reg;
            }
            case '-': {
                string reg = GetAvailableRegister(registers);
                string v1 = string(1, val[i - 1]);
                string v2 = string(1, val[i + 1]);

                FillRegister(registers, reg, stod(v1));

                string r = GetMovAssignment("mov", reg, v1);
                results.push_back(r);
                string r2 = GetMovAssignment("sub", reg, v2);
                results.push_back(r2);
                FillRegister(registers, reg, stod(v1));

                return reg;
            }
            case '*': {

            }
            case '/': {

            }
            default: {
                continue;
            }
        }
    }
}

string GetAvailableRegister(vector<tuple<bool, string, double> > &registers) {
    for (int i = 0; i < registers.size(); ++i) {
        tuple<bool, string, double> t = registers[i];
        if (!get<0>(t)) {
            return get<1>(t);
        }
    }

    throw invalid_argument("Out of registers!");
}

void FillRegister(vector<tuple<bool, string, double> > &registers, string reg, double val) {
    for (int i = 0; i < registers.size(); ++i) {
        tuple<bool, string, double> &t = registers[i];
        if (get<1>(t) == reg) {
            t = make_tuple(true, reg, val);
        }
    }
}

void EmptyRegister(vector<tuple<bool, string, double> > &registers, string reg) {
    for (int i = 0; i < registers.size(); ++i) {
        tuple<bool, string, double> &t = registers[i];
        if (get<1>(t) == reg) {
            t = make_tuple(false, reg, NAN);
        }
    }
}


string GetMovAssignment(string op, string reg, string val, bool isReg) {
    if (!isReg) {
        return op + " " + reg + "," + val + "d";
    }
    return op + " " + reg + "," + val;

}

string RemoveSpaces(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

vector<string> *ReadFromFile(string path) {
    vector<string> *statements = new vector<string>();
    string file = "((2 * 3) + 1 + (3 * 4)) + 3";

    istringstream f(file);
    string line;
    while (getline(f, line)) {
        // Push lines into vector
        statements->push_back(line);
    }

    return statements;
}

