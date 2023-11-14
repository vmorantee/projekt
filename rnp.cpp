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
map<string,double(*)(double)> func=initFunc();

bool isFunc(string pFunc){
    return func.find(pFunc)!=func.end();
}
int rnpPrio(string op){
    if(isFunc(op)) return 4;
    else if(op=="^") return 3;
    else if(op=="/"||op=="*") return 2;
    else if(op=="+"||op=="-") return 1;
    else return 0;
}
bool isOperator(string isOp) {
    return (isOp == "+" || isOp == "-" || isOp == "*" || isOp == "/" || isOp == "^"||isOp==")"||isOp=="("||isFunc(isOp));
}

string RPN(string equation){
    stack<string>ostack;
    int nfind=0,lfind=0,currPrio;
    string result,op,top;
    for(int i=0;i<equation.length();i++){
        if(isdigit(equation[i])) nfind++;
        else{
            if(nfind){
                result+=equation.substr(i-nfind,nfind);
                result+=" ";
                nfind=0;
            }
            if(isalpha(equation[i])){
                lfind++;
            }
            op=equation.substr(i,1);
            if(isOperator(op)){
                if(lfind==1&&isalpha(equation[i-1])){
                    result+=equation[i-1];
                    result+=" ";
                    lfind=0;
                }
                else if(lfind&&isFunc(equation.substr(i-lfind,lfind))){
                    ostack.push(equation.substr(i-lfind,lfind));
                    lfind=0;
                }
                if(ostack.empty())
                    ostack.push(op);
                else{
                    if(op=="(")
                        ostack.push(op);
                    else if(op==")"){
                        while(!ostack.empty()&&ostack.top()!="("){
                            result+=ostack.top();
                            result+=" ";
                            ostack.pop();
                        }
                        if(ostack.top()=="(") ostack.pop();
                        else {cout<<"Niepoprawne wyrażenie(niedomknięty nawias)";break;}
                    }
                    else if(rnpPrio(op)>rnpPrio(ostack.top()))
                        ostack.push(op);
                    else if(rnpPrio(op)<=rnpPrio(ostack.top())){
                        while(!ostack.empty()&&rnpPrio(op)<=rnpPrio(ostack.top())&&ostack.top()!="("){
                            result+=ostack.top();
                            result+=" ";
                            ostack.pop();
                        }
                        ostack.push(op);
                    }
                }
            }
        }
    }
    if(nfind){result+=equation.substr(equation.length()-nfind,nfind); result+=" ";};
    if(lfind){result+=equation.substr(equation.length()-lfind,lfind); result+=" ";};
    while(!ostack.empty()){
        result+=ostack.top();
        result+=" ";
        ostack.pop();
    }
    return result;
}
int main() {
    string allOperators[5] = {"+", "-", "/", "*", "^"};
    map<string, double(*)(double)> func = initFunc();
    stack<double> numbers;
    stack<double> operators;
    string input;
    getline(cin, input);
    cout<<RPN(input);
    return 0;
}
