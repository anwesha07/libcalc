/*
Project Title : Generic Expression Calculator

Project Description: A calculator class which exposes a calculate method to perform calculations on any kind of infix expression.
It is done by converting the infix expression into postfix and then evaluating it.As an example, complex and vector classes have been implemented 
and operations on complex numbers and 3-D vectors have been made possible.


Author: Anwesha Das 
*/


#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

/************************* Vector class *************************/

class Vector {
    private:
        float x;
        float y;
        float z;
    
    public:
        Vector(float, float, float);
        Vector operator +(const Vector&);
        Vector operator -(const Vector&);
        friend ostream& operator <<(ostream&, const Vector&);
};

Vector::Vector(float x = 0, float y = 0, float z = 0) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector Vector::operator +(const Vector& v) {
    float x = this->x + v.x;
    float y = this->y + v.y;
    float z = this->z + v.z;
    return Vector(x, y, z);
}

Vector Vector::operator -(const Vector& v) {
    float x = this->x - v.x;
    float y = this->y - v.y;
    float z = this->z - v.z;
    return Vector(x, y, z);
}

ostream& operator <<(ostream& cout, const Vector& v) {
    cout << "(" << v.x << "i";
    if (v.y >= 0)
        cout << "+" << v.y << "j";
    else
        cout << "-" << -v.y << "j";
    if (v.z >= 0)
        cout << "+" << v.z << "k)";
    else
        cout << "-" << -v.z << "k)";
    return cout;
}

/************************* Complex class *************************/

class Complex {
    private:
        float re;
        float im;
    
    public:
        Complex(float, float);
        Complex operator +(const Complex&);
        Complex operator -(const Complex&);
        Complex operator *(const Complex&);
        friend ostream& operator <<(ostream&, const Complex&);
};

Complex::Complex(float re = 0, float im = 0) {
    this->re = re;
    this->im = im;
}

Complex Complex::operator +(const Complex& b) {
    float re = this->re + b.re;
    float im = this->im + b.im;
    return Complex(re, im);
}

Complex Complex::operator -(const Complex& b) {
    float re = this->re - b.re;
    float im = this->im - b.im;
    return Complex(re, im);
}

Complex Complex::operator *(const Complex& b) {
    float re = (this->re * b.re) - (this->im * b.im);
    float im = (this->re * b.im) + (this->im * b.re);
    return Complex(re, im);
}

ostream& operator <<(ostream& out, const Complex& c) {
    if (c.im >= 0)
        out << c.re << "+" << c.im << "i";
    else
        out << c.re << "-" << -c.im << "i";
    return out;
}

/******************** Calculator class ********************/

template <typename T>
class Calculator {
    private:
        bool isOperator(string);
        int precision(char);
        T evaluatePostfix(vector<string>);
        virtual T getOperandValue(string) = 0;
        virtual string extractOperand(string, int&) = 0;
        virtual T performOperation(T, T, char) = 0;
    
    protected:
        vector<string> infixToPostfix(string);
  
    public:
        T calculate(string);
};

template <typename T>
bool Calculator<T>::isOperator(string symbol) {
    vector<string> operators = {"+", "-", "*", "/"};
    vector<string>::iterator idx = find(operators.begin(), operators.end(), symbol);
    return idx != operators.end();
}

template <typename T>
vector<string> Calculator<T>::infixToPostfix(string infix) {
    int l = infix.length();
    stack<char> operatorStack;
    operatorStack.push('N');
    vector<string> postfix;
    string operand;

    for (int i = 0; i < l; i++) {
        operand = extractOperand(infix, i);

        if (operand != "") {
            postfix.push_back(operand);
            continue;
        }

        if (infix[i] == '(') {
            operatorStack.push('(');
        }

        else if (infix[i] == ')') {
            while (operatorStack.top() != 'N' && operatorStack.top() != '(') {
                char c = operatorStack.top();
                operatorStack.pop();
                string op;
                op += c;
                postfix.push_back(op);
            }
            if (operatorStack.top() == '(') {
                operatorStack.pop();
            }
        }

        else {
            while (operatorStack.top() != 'N' && precision(infix[i]) <= precision(operatorStack.top())) {
                char c = operatorStack.top();
                operatorStack.pop();
                string op;
                op += c;
                postfix.push_back(op);
            }
            operatorStack.push(infix[i]);
        }
    }

    while (operatorStack.top() != 'N') {
        char c = operatorStack.top();
        operatorStack.pop();
        string op;
        op += c;
        postfix.push_back(op);
    }

    return postfix;
}

template <typename T>
T Calculator<T>::calculate(string input) {
    vector<string> postfixExp;
    postfixExp = infixToPostfix(input);
    return evaluatePostfix(postfixExp);
}

template <typename T>
int Calculator<T>::precision(char c) {
    if (c == '*' || c == '/')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

template <typename T>
T Calculator<T>::evaluatePostfix(vector<string> postfix) {
    int l = postfix.size();
    stack<T> solnStack;
    T val1, val2, res;
    T temp;

    for (int i = 0; i < l; i++) {
        if (isOperator(postfix[i])) {
            val1 = solnStack.top();
            solnStack.pop();
            val2 = solnStack.top();
            solnStack.pop();
            res = performOperation(val1, val2, postfix[i][0]);
            solnStack.push(res);
        } else {
            temp = getOperandValue(postfix[i]);
            solnStack.push(temp);
        }
    }
    res = solnStack.top();
    solnStack.pop();
    return res;
}

/******************** ArithmeticCalculator class ********************/

class ArithmeticCalculator:public Calculator<float> {
    private:
        int precision(char);
        float getOperandValue(string);
        string extractOperand(string, int&);
        float evaluatePostfix(vector<string>);
        float performOperation(float, float, char);
};

float ArithmeticCalculator::getOperandValue(string operand) {
    float temp;
    sscanf(operand.c_str(), "%f", &temp);
    return temp;
}

string ArithmeticCalculator::extractOperand(string infix, int& i) {
    string operand = "";
    while ((infix[i] >= '0' && infix[i] <= '9') || infix[i] == '.') {
        operand += infix[i];
        i++;
    }
    if (operand != "")
        i -= 1;
    return operand;
}

float ArithmeticCalculator::performOperation(float val1, float val2, char operation) {
    switch (operation) {
        case '+':
            return val2 + val1;
        case '-':
            return val2 - val1;
        case '*':
            return val2 * val1;
        case '/':
            return val2 / val1;
        default:
            cout << "INVALID EXPRESSION" << endl;
            exit(1);
    }
}

/********************* VectorCalculator class *********************/

class VectorCalculator:public Calculator<Vector> {
    private:
        int precision(char);
        bool isOperator(string);
        Vector getOperandValue(string);
        string extractOperand(string, int&);
        Vector evaluatePostfix(vector<string>);
        Vector performOperation(Vector, Vector, char);
};

bool VectorCalculator::isOperator(string symbol) {
    vector<string> operators = {"+", "-"};
    vector<string>::iterator idx = find(operators.begin(), operators.end(), symbol);
    return idx != operators.end();
}

int VectorCalculator::precision(char c) {
    if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

Vector VectorCalculator::getOperandValue(string operand) {
    int x, y, z;
    // Assuming input is always of form xi±yj±zk
    int v = sscanf(operand.c_str(), "%di%dj%dk", &x, &y, &z);
    if (v != 3) {
        cout << "Error in parsing input" << endl;
        exit(1);
    }
    return Vector(x, y, z);
}

string VectorCalculator::extractOperand(string infix, int& i) {
    string operand = "";
    if (infix[i] != '<') {
        return operand;
    }
    i += 1;
    while (infix[i] != '>') {
        operand += infix[i++];
    }
    return operand;
}

Vector VectorCalculator::performOperation(Vector val1, Vector val2, char operation) {
    switch (operation) {
        case '+':
            return val2 + val1;
        case '-':
            return val2 - val1;
        default:
            cout << "INVALID EXPRESSION" << endl;
            exit(1);
    }
}

/********************* ComplexCalculator class *********************/

class ComplexCalculator:public Calculator<Complex> {
    private:
        int precision(char);
        Complex getOperandValue(string);
        string extractOperand(string, int&);
        Complex evaluatePostfix(vector<string>);
        Complex performOperation(Complex, Complex, char);
};

int ComplexCalculator::precision(char c) {
    if (c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

Complex ComplexCalculator::getOperandValue(string operand) {
    int r, i;
    // Assuming input is always of form a±ib
    int v = sscanf(operand.c_str(), "%d%di", &r, &i);
    if (v != 2) {
        cout << "Error in parsing input" << endl;
        exit(2);
    }
    return Complex(r, i);
}

string ComplexCalculator::extractOperand(string infix, int& i) {
    string operand = "";
    if (infix[i] != '<') {
        return operand;
    }
    i += 1;
    while (infix[i] != '>') {
        operand += infix[i++];
    }
    return operand;
}

Complex ComplexCalculator::performOperation(Complex val1, Complex val2, char operation) {
    switch (operation) {
        case '+':
            return val2 + val1;
        case '-':
            return val2 - val1;
        case '*':
            return val2 * val1;
        default:
            cout << "INVALID EXPRESSION" << endl;
            exit(1);
    }
}


/*******************************************Menu for the project*********************************************************************/

void clrscr(){
    cout << "\033[2J\033[1;1H";
}

void displayMenu(){
    cout << "\n************************************************************* MENU *************************************************************************"   ;
    cout << "\n\n 0. EXIT.";
    cout << "\n\n 1. Basic Arithmetic on Real numbers.";
    cout << "\n\n 2. Basic Arithmetic on Complex numbers.";
    cout << "\n\n 3. Vector Arithmetic.";
    cout << "\n\n*********************************************************************************************************************************************";
}


void Vcal(){
    string input;
    VectorCalculator vcal;
    cout << endl;
    cout << " Vectors should be in the form <xi+yj-zk> , where x,y,z should be real numbers.  " << endl;
    cout << "Enter the expression: ";
    cin >> input;
    cout << endl;
    cout << "ANSWER: " << vcal.calculate(input) << endl;
    
}

void Rcal(){
    string input;
    ArithmeticCalculator acal;
    cout << endl;
    cout << " Please enter real numbers in the expression only.  " << endl;
    cout << "Enter the expression: ";
    cin >> input;
    cout << endl;
    cout << "ANSWER: " << acal.calculate(input) << endl;
    
}

void Ccal(){
    string input;
    ComplexCalculator ccal;
    cout << endl;
    cout << " Complex numbers should be in the form <a+ib>, where a denotes the real part and b is the imaginary part." << endl;
    cout << "Enter the expression: ";
    cin >> input;
    cout << endl;
    cout << "ANSWER: " << ccal.calculate(input) << endl;
    
}

int main() {
    int ch;
    string output = "";
    cout << "This is a generic calculator whih can be used for basic arithmetic on real and complex numbers and vector calculations.";
    cout << "\nYou have to just enter the expression in the mentioned format and press enter to get the results to shift to any of the calculators follow the menu given below.";
    displayMenu();
    
    while (1) {
        string input = "";
        cout << "\nEnter your choice : ";
        cin >> ch;
        switch (ch){
            case 0:
            cout << "Exiting........";
            exit(0);
            break;
            case 1:
            clrscr();
            cout << "\n\nStarting Real number calculator..................";
            cout << "\n\nSample Input = 256+132*45/(128-67)";
            Rcal();
            
            break;
            case 2:
            clrscr();
            cout << "\n\nStarting Complex number calculator..................";
            cout << "\n\nSample Input: <3+0i>-(<5+1i>+<12-3i>)";
            cout << endl << output;
            Ccal();
            
            break;
            case 3:
            clrscr();
            cout << "\n\nStarting Vector calculator..................";
            cout<< "\n\nSample Input: <4i+12j-0k>+<3i-6j+4k>-(<12i+1j+7k>+<-7i+2j-4k>)";
            cout << endl << output;
            Vcal();
            
            break;
            default:
            clrscr();
            cout << "\nSORRY!!!! Invalid choice entered.";
           
            
        }
        sleep(2);
        cout << "\nPress 0 to exit. To continue further, please enter a valid choice from the given menu.";
        displayMenu();
    }
    return 0;
}
