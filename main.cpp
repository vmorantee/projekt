#include <iostream>
#include <functional>
#include <cmath>
#include <math.h>
#include <string>
#include <map>
#include <stack>
using namespace std;

map<string,double(*)(double)> func;
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
map <string,double(*)(double)> initFunc(){
    map<string, double(*)(double)> func; 
    func["sin"] = sin;
    func["cos"] = cos;
    func["tg"] =  tan;
    func["ctg"] = [](double x){return 1/tan(x); };
    func["pi"] = [](double){return M_PI;};
    func["exp"] = exp;
    func["log"] = log;
    func["sqrt"] = sqrt;
    return func;
}
int rpnPrio(char op){
    if(op=='^') return 3;
    else if(op=='/'||op=='*') return 2;
    else if(op=='+'||op=='-') return 1;
    else return 0;
}
bool isFunc(string pFunc){
    for(const auto entry:func){
        if(entry.first==pFunc) return true;
    }
    return false;
}
bool isOperator(char isOp) {
    return (isOp=='+'||isOp=='-'||isOp=='*'||isOp=='/'||isOp=='^');
}

string RPN(string equation){
    string allOperators[5] = {"+", "-", "/", "*", "^"},result;
    stack<string>rpn;
    int nfind=0,lfind=0;
    for(int i=0;i<=equation.length();i++){
        if(isdigit(equation[i])||equation[i]=='.'){
            nfind++;
        }
        else{
        if(nfind!=0) rpn.push(equation.substr(i-nfind,nfind));
        if(isalpha(equation[i])){
            nfind++;
            if(lfind>1&&lfind<5) isFunc(equation.substr(i-lfind,i));
        }
        else if(isOperator(equation[i])){
            //oplogic
        }
    }
       }   return equation;
}
int main() {
    string allOperators[5] = {"+", "-", "/", "*", "^"};
    map<string, double(*)(double)> func = initFunc();
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
    cout <<input;
    return 0;
}
