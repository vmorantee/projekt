#include <iostream>
#include <functional>
#include <cmath>
#include <math.h>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <fstream>

using namespace std;
//todo test file
double basicOperation(string op,double num1, double num2){
    if(op=="^") return pow(num1,num2);
    if(op=="+") return num1+num2;
    if(op=="-") return num1-num2;
    if(op=="*") return num1*num2;
    if(op=="/") return num1/num2;
    return 0;
}


double eval(function<double(double)> func, double x) {
    return func(x);
}
bool checkParentheses(string input){
    int p1=0,p2=0;
    for(char op:input){
        if(op=='(') p1++;
        else if(op==')')p2++;
    }
    if(p2==p1) return true;
    return false;
}
map <string,double(*)(double)> initFunc(){
    map<string, double(*)(double)> func; 
    func["sin"] = sin;
    func["cos"] = cos;
    func["tg"] =  tan;
    func["ctg"] = [](double x){return 1/tan(x); };
    func["exp"] = exp;
    func["ln"] = log;
    func["sqrt"] = sqrt;
    func["pi"] = [](double){return M_PI;};
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
string removeSpaces(string input){
    string res="";
    for(int i=0;i<input.length();i++){
        if(input[i]!=' ') res+=input[i];
    }
    return res;
}

double evalFun(vector<string>RPNV,double value){
    stack<string>operrands;
    double result=0,pres,num1,num2;
    int i=0;
    for(const auto x: RPNV){
        if(isdigit(x[0])){
            operrands.push(x);
        }
        else if(x=="x"){
            operrands.push(to_string(value));
        }
        else if(isFunc(x)){
            if(x[0]=='-') pres=eval(func[x],stod(operrands.top()))*-1;
            else pres=eval(func[x],stod(operrands.top()));
            operrands.pop();
            operrands.push(to_string(pres));
        }
        else if(isOperator(x)){
            num2=stod(operrands.top());
            operrands.pop();
            num1=stod(operrands.top());
            operrands.pop();
            pres=basicOperation(x,num1,num2);
            operrands.push(to_string(pres));
        } 
        i++;
    }
    if(operrands.size()==1) result+=stod(operrands.top());
    return result;
}
vector<string>RPN(string equation){
    stack<string>ostack;
    int nfind=0,lfind=0,currPrio;
    string op,top;
    vector<string>result;
    for(int i=0;i<equation.length();i++){
        if(isdigit(equation[i])||equation[i]=='.') nfind++;
        else{
            if(nfind){
                result.insert(result.end(),equation.substr(i-nfind,nfind));
                nfind=0;
            }
            if(isalpha(equation[i])){
                lfind++;
            }
            op=equation.substr(i,1);
            if(isOperator(op)){
                if(lfind==1&&isalpha(equation[i-1])){
                    result.insert(result.end(),equation.substr(i-1,1));
                    lfind=0;
                }
                else if(lfind&&isFunc(equation.substr(i-lfind,lfind))){
                    if(equation.substr(i-lfind,lfind)=="pi") result.insert(result.end(),to_string(M_PI));
                    else ostack.push(equation.substr(i-lfind,lfind));
                    lfind=0;
                }
                if(ostack.empty())
                    ostack.push(op);
                else{
                    if(op=="(")
                        ostack.push(op);
                    else if(op==")"){
                        while(!ostack.empty()&&ostack.top()!="("){
                            result.insert(result.end(),ostack.top());
                            ostack.pop();
                        }
                        if(ostack.top()=="(") ostack.pop();
                        else {cout<<"Niepoprawne wyrażenie(niedomknięty nawias)";break;}
                    }
                    else if(rnpPrio(op)>rnpPrio(ostack.top()))
                        ostack.push(op);
                    else if(rnpPrio(op)<=rnpPrio(ostack.top())){
                        while(!ostack.empty()&&rnpPrio(op)<=rnpPrio(ostack.top())&&ostack.top()!="("){
                            result.insert(result.end(),ostack.top());
                            ostack.pop();
                        }
                        ostack.push(op);
                    }
                }
            }
        }
    }
    if(nfind)result.insert(result.end(),equation.substr(equation.length()-nfind,nfind));
    if(lfind)result.insert(result.end(),equation.substr(equation.length()-lfind,lfind));
    while(!ostack.empty()){
        result.insert(result.end(),ostack.top());
        ostack.pop();
    }
    return result;
}

string checkString(string input){
    bool digit=false,alpha=false,op=false;
    int len=input.length(),i=0;
    if(input[0]=='-') input.insert(0,"0");
    while(i<len){
        char temp=input[i];
        if(input[i]=='('&&input[i+1]=='-') input.insert(i+1,"0");
        if(isdigit(input[i])){
            digit=true;
            alpha=false;
        }
        else if(isalpha(input[i])){
            alpha=true;
            if(digit) input.insert(i,"*");
            digit=false;
        }
        else if(isOperator(input.substr(i,1))){
            op=true;
            alpha=false;
            digit=false;
        }
        i++;
    }
    return input;
}
double calculateIntegral(double start, double stop, int precision,string equation){
    equation=removeSpaces(equation);
    equation=checkString(equation);
    vector<string> RPNV=RPN(equation);
    double result=0,width,a=evalFun(RPNV,start),b;
    width=(stop-start)/precision;
    for(int i=0;i<precision;i++){
        b=evalFun(RPNV,start+width*i);
        //cout<<result<<" "<<width*i<<" "<<width*(i+1)<<" "<<a<<" "<<b<<" "<<((a+b)*width)/2<<endl;
        result+=((a+b)*width)/2;
        a=b;
    }
    RPNV.clear();
    RPNV.shrink_to_fit();
    return result;
}
int initInterface(){
    while(true){
        cout<<"Program liczący całkę oznaczoną złożoną metodą trapezów\n1-Wpisz funkcje do obliczenia całki\n2-Dane testowe pobierane z pliku\n";
        int choice,schoice,precision;
        double start,stop,result;
        string input;
        cin>>choice;
        if(choice==1){
            cout<<"Wpisz funkcje którą chcesz obliczyć"<<endl;
            cin.ignore();
            getline(cin,input);
            if(!checkParentheses(input)) cout<<"Błędne dane(niedomknięty/nieotwarty nawias)"<<endl;
            else{
            cout<<"Wpisz początek przedziału"<<endl;
            cin>>start;
            cout<<"Wpisz koniec przedziału"<<endl;
            cin>>stop;
            if(start>stop){
                cout<<"Start przedziału jest większy od końca czy chcesz zamienić ich wartości\n1-Tak\n2-Nie";
                cin>>schoice;
                if(choice==1) swap(start,stop);
                }
            else{
                cout<<"Wpisz precyzję(ilość trapezów)"<<endl;
                cin>>precision;
                try
                {
                    result=calculateIntegral(start,stop,precision,input);
                    cout<<"Całka dla podanej funkcji wynosi "<<result<<endl;
                }
                catch(const std::exception& e)
                {
                    cout<<"Wystąpił błąd"<<endl;
                }
                
            }
            }
        }else if(choice==2){
            ifstream test("test.txt");
            string teststr,descstr;
            while(getline(test,teststr)&&getline(test,descstr)){
                cout<<"Funkcja: "<<teststr<<endl<<"Jej całka wynosi(na przedziale 0,1 z precyzją 1000) "<<calculateIntegral(0.2,1,1000,teststr)<<endl<<"Jej wartość teoretyczna(liczona z definicji całki) wynosi: "<<descstr<<endl;
            }
            test.close();
        }else if(choice==3){
            break;
        }else{
            cout<<"Niepoprawny wybór"<<endl;
        }
    }
    return 0;
}
int main() {
    system("chcp 1250>>null");
    setlocale(LC_CTYPE, "Polish");
    initInterface();
    return 0;
}
