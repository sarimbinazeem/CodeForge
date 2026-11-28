#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

// Helper function to return precedence of operators
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return -1;
}

string infixToPostfix(const string& expression) {
    stack<char> st;
    string output = "";

    for (char c : expression) {
        // Skip whitespace if present
        if (c == ' ') continue;

        // 1. If operand, append to output
        if (isalnum(c)) {
            output += c;
        }
        // 2. If '(', push to stack
        else if (c == '(') {
            st.push(c);
        }
        // 3. If ')', pop until '(' is encountered
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                output += st.top();
                st.pop();
            }
            if (!st.empty()) {
                st.pop(); // Discard '('
            }
        }
        // 4. If operator
        else {
            while (!st.empty() && st.top() != '(' &&
                  (precedence(st.top()) > precedence(c) ||
                  (precedence(st.top()) == precedence(c) && c != '^'))) {
                output += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // 5. Pop all remaining operators from the stack
    while (!st.empty()) {
        output += st.top();
        st.pop();
    }

    return output;
}

int main() {
    string expr;
    if (cin >> expr) {
        cout << infixToPostfix(expr) << endl;
    }
    return 0;
}
