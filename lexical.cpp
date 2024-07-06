#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Token class represents individual tokens identified by the lexer
class Token {
private:
    string CP;      // ClassPart
    string VP;      // ValuePart
    int lineNo;     // Line number where the token appears

public:
    // Constructor to initialize Token with ClassPart, ValuePart, and line number
    Token(string CP, string VP, int lineNo) {
        this->CP = CP;
        this->VP = VP;
        this->lineNo = lineNo;
    }

    // Method to print Token details to console
    void print() {
        cout << "(Classpart: " << CP << " , Valuepart: " << VP << ", lineno: " << lineNo << ")" << endl;
    }

    // Method to print Token details to output file
    void printToFile(ofstream& fout) {
        fout << "(" << CP << " , " << VP << " , " << lineNo << " )" << endl;
    }
};

// Keyword map to store keywords and their corresponding ClassParts
unordered_map<string, string> keywordMap = {
    {"while", "ID"},
    {"string", "ID"},
    {"for", "ID"},
    {"if", "ID"},
    {"public", "ID"},
    {"private", "ID"},
    {"protected", "ID"},
    {"static", "ID"},
    {"class", "class"},
    {"array", "ID"},
    {"continue", "ID"},
    {"main", "ID"},
    {"void", "ID"},
    {"true", "ID"},
    {"false", "ID"},
    {"abstract", "ID"},
    {"sealed", "ID"},
    {"break", "ID"},
    {"return", "ID"}
};

// Operator map to store operators and their corresponding ClassParts
unordered_map<string, string> operatorMap = {
    {"&&", "&"},
    {"||", "|"},
    {"==", "="},
    {"!=", "!="},
    {"+=", "Assignment"},
    {"-=", "Assignment"},
    {"*=", "Assignment"},
    {"/=", "Assignment"},
    {">=", "Comparison"},
    {"<=", "Comparison"},
    {">", "Comparison"},
    {"<", "Comparison"},
    {"+", "+"},
    {"-", "-"},
    {"*", "*"},
    {"/", "/"},
    {"%", "%"},
    {":", ":"},
    {",", "','"},
    {";", ";"},
    {"(", "("},
    {")", ")"},
    {"{", "{"},
    {"}", "}"}
};

// Function to check if a string is an operator
bool isOperator(const string& str) {
    return operatorMap.find(str) != operatorMap.end();
}

// Function to check if a string is a keyword
bool isKeyword(const string& str) {
    return keywordMap.find(str) != keywordMap.end();
}

// Function to check if a string is a valid integer constant
bool isInteger(const string& str) {
    for (char c : str) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

// Function to check if a string is a valid double constant
bool isDouble(const string& str) {
    bool hasDot = false;
    for (char c : str) {
        if (c == '.') {
            if (hasDot)
                return false; // more than one dot
            hasDot = true;
        } else if (!isdigit(c))
            return false;
    }
    return true;
}

// Function to tokenize a line of code and identify tokens
void tokenizeLine(const string& line, int lineno, vector<Token>& tokens) {
    string token;
    for (size_t i = 0; i < line.size(); ++i) {
        if (isspace(line[i])) {
            continue;   // Skip whitespace characters
        } else if (line[i] == '/' && line[i + 1] == '/') {
            break;      // Ignore rest of the line as it is a comment
        } else {
            token += line[i];   // Build token character by character

            // Check if token is complete or line ends
            if (isspace(line[i + 1]) || i == line.size() - 1) {
                if (isKeyword(token)) {
                    tokens.emplace_back(keywordMap[token], token, lineno);   // Add keyword token
                } else if (isOperator(token)) {
                    tokens.emplace_back(operatorMap[token], token, lineno);  // Add operator token
                } else if (isInteger(token)) {
                    tokens.emplace_back("Integer_Constant", token, lineno);  // Add integer constant token
                } else if (isDouble(token)) {
                    tokens.emplace_back("Double_Constant", token, lineno);   // Add double constant token
                } else {
                    tokens.emplace_back("ID", token, lineno);                // Add identifier token
                }
                token.clear();  // Clear token for next iteration
            }
        }
    }
}

// Main function to read input file, tokenize each line, and output tokens
int main() {
    ifstream file("program.txt");   // Input file stream
    ofstream outFile("tokens.txt"); // Output file stream

    // Check if input file opened successfully
    if (!file.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    // Check if output file opened successfully
    if (!outFile.is_open()) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    vector<Token> tokens;   // Vector to store tokens
    string line;            // String to store each line of input file
    int lineno = 0;         // Line number counter

    // Read each line from input file
    while (getline(file, line)) {
        ++lineno;   // Increment line number
        tokenizeLine(line, lineno, tokens); // Tokenize current line
    }

    // Print tokens to console and output file
    for (const auto& token : tokens) {
        token.print();              // Print token details to console
        token.printToFile(outFile); // Print token details to output file
    }

    file.close();       // Close input file
    outFile.close();    // Close output file

    return 0;   // Exit program
}
