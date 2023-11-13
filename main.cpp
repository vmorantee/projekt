#include <iostream>
#include <functional>
#include <cmath>
#include <math.h>
#include <string>
#include <map>
#include <stack>
using namespace std;

double eval(function<double(double)> func, double x) {
    return func(x);
}
bool checkParentheses(string input){
    int p1=0,p2=0;
    for(char op:input){
        if(op=='(') p1++;
        else if(op==')')p2++;
    }
    if(p2==1) return true;
    return false;
}

int main() {
    map<string, double(*)(double)> func;
    string allOperators[5] = {"+", "-", "/", "*", "^"};
    func["sin"] = sin;
    func["cos"] = cos;
    func["tg"] =  tan;
    func["ctg"] = [](double x){return 1/tan(x); };
    func["pi"] = [](double){return M_PI;};
    func["exp"] = exp;
    func["log"] = log;
    func["sqrt"] = sqrt;
    stack<double> numbers;
    stack<double> operators;
    string input;
    cout << "XD";
    getline(cin, input);
    for (const string op : allOperators) {
        string spaceOperator = " " + op + " ";
        size_t pos = input.find(op);
        while (pos != string::npos) {
            input.replace(pos, op.length(), spaceOperator);
            pos = input.find(op, pos + spaceOperator.length());
        }
    }
    for(const auto op: func){
        size_t pos = input.find(op.first);
        double evaled = eval(op.second,1);
        string sEvaled =to_string(evaled);
        while (pos != string::npos) {
            input.replace(pos, op.first.length(),sEvaled);
            pos = input.find(op.first, pos + sEvaled.length());
        }
    }
    cout << input<<endl;

    return 0;
}
