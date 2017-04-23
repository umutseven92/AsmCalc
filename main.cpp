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

vector<string> GenerateAsm(vector<char> postfix, bool assn);

vector<char> ConvertToPostfix(string input);

void SaveResult(vector<vector<string> > result);

int main(int argc, char *argv[]) {

    if (argc <= 1) {
        cout << "Please give the path to your input file as a command line parameter." << endl;
        return 0;
    }

    vector<vector<string> > result;

    vector<string> statements = *(ReadFromFile(argv[1]));

    // Generate asm code
    long size = statements.size();
    for (int i = 0; i < size; ++i) {
        string st = RemoveSpaces(statements[i]);

        std::size_t found = st.find("=");
        if (found != std::string::npos) {
            // Assignment
            string val = st.substr((unsigned long) (found + 1), st.size());

            vector<char> pVal = ConvertToPostfix(val);
            vector<string> pRes = GenerateAsm(pVal, true);

            result.push_back(pRes);
        } else {
            // Print statement
            vector<char> pVal = ConvertToPostfix(st);
            vector<string> pRes = GenerateAsm(pVal, false);
            result.push_back(pRes);
        }
    }

    SaveResult(result);

    return 0;
}

// Save the asm results as result.asm
void SaveResult(vector<vector<string> > result)
{
    ofstream myfile ("result.asm");
    if (myfile.is_open())
    {
        for (int i = 0; i < result.size(); ++i) {
            for (int j = 0; j < result[i].size(); ++j) {
                myfile << result[i][j] << "\n";
            }
        }

        cout << "The result has been saved as result.asm." << endl;
        myfile.close();
    }
}

// Convert infix notation to postfix notation
vector<char> ConvertToPostfix(string input) {
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

    unsigned long size = input.size();

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

// Convert postfix notation to ASM instructions
vector<string> GenerateAsm(vector<char> postfix, bool assn) {
    vector<string> output;
    if (assn) {
        output.push_back("PUSH offset VAL");
    }
    for (int i = 0; i < postfix.size(); ++i) {

        switch (postfix[i]) {
            case '+':
                output.push_back("POP CX");
                output.push_back("POP AX");
                output.push_back("ADD AX, CX");
                output.push_back("PUSH AX");
                break;
            case '-':
                output.push_back("POP CX");
                output.push_back("POP AX");
                output.push_back("SUB AX, CX");
                output.push_back("PUSH AX");
                break;
            case '*':
                output.push_back("POP CX");
                output.push_back("POP AX");
                output.push_back("MULT CX");
                output.push_back("PUSH AX");
                break;
            case '/':
                output.push_back("MOV DX, 0");
                output.push_back("POP CX");
                output.push_back("POP AX");
                output.push_back("DIV CX");
                output.push_back("PUSH AX");
                break;
            default:
                string s(1, postfix[i]);
                output.push_back("PUSH " + s);
                break;
        }
    }

    return output;
}

string RemoveSpaces(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

vector<string> *ReadFromFile(string path) {
    vector<string> *statements = new vector<string>();

    ifstream f(path);
    string line;
    while (getline(f, line)) {
        // Push lines into vector
        statements->push_back(line);
    }

    return statements;
}

