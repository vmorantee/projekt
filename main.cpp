#include <iostream>
#include <functional>
#include <cmath>
#include <math.h>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;
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
string fixSpaces(string input){
    stringstream str;
    for(int i=0;i<input.length();i++){
        if(isOperator(input.substr(i,1))) str<<" "<<input[i]<<" ";
        else if(input[i]!=' ') str<<input[i];
    }
    return str.str();
}

double evalFun(string RPNV,double value){
    stack<string>operrands;
    double result=0,pres,num1,num2;
    istringstream temp(RPNV);
    string x;
    while(temp>>x)
    { 
        if(isdigit(x[0])){
            operrands.push(x);
        }
        else if(x=="x"){
            operrands.push(to_string(value));
        }
        else if(x=="pi") operrands.push(to_string(M_PI));
        else if(isFunc(x)){
            pres=eval(func[x],stod(operrands.top()));
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
    }
    if(operrands.size()==1) result+=stod(operrands.top());
    return result;
}
string checkString(string input){
    bool digit=false,alpha=false,op=false;
    if(input[0]=='-') input.insert(0,"0");
    int len=input.length(),i=0;
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
string RPN(string equation){
    equation=checkString(equation);
    equation=fixSpaces(equation);
    stack<string>ostack;
    string curr,result="";
    istringstream temp(equation);
    while(temp>>curr){
        if(isdigit(curr[0])||curr=="x"){
            result+=curr+" ";
        }
        else if(isOperator(curr)){
            if(ostack.empty()||curr=="(") ostack.push(curr);
            else{
                while(!ostack.empty()&&rnpPrio(curr)<=rnpPrio(ostack.top())&&ostack.top()!="("){
                    result+=ostack.top()+" ";
                    ostack.pop();
                }
                if(curr==")") ostack.pop();
                else ostack.push(curr);
            }
        }
    }
    while(!ostack.empty()){
        result+=ostack.top()+" ";
        ostack.pop();
    }
    return result;
}
double calculateIntegral(double start, double stop, int precision, string equation) {
    string RPNV=RPN(equation);
    double result=0,width,a=evalFun(RPNV,start),b;
    width =(stop-start)/precision;
    for(int i=0;i<precision;i++){
        b=evalFun(RPNV,start+width*i);
        result+=((a+b)*width) / 2;
        a=b;
    }
    return result;
}

int initInterface(){
    int choice =1;
    while(choice!=3){
        cout<<"Program liczący całkę oznaczoną złożoną metodą trapezów\n1-Wpisz funkcje do obliczenia całki\n2-Dane testowe pobierane z pliku\n3-Zakończ\n";
        int schoice,precision;
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
                cout<<"Wpisz ilość trapezów"<<endl;
                cin>>precision;
                try
                {
                    cout<<setprecision(5);
                    cout<<"Calka oznaczona na podanym przedziale wynosi: ";
                    cout<<calculateIntegral(start,stop,precision,input)<<endl;
                    
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
                cout<<"Funkcja: "<<teststr<<endl<<"Jej całka wynosi(na przedziale 0.2, 1 z iloscia trapezow 1000) "<<calculateIntegral(0.2,1,1000,teststr)<<endl<<"Jej wartość teoretyczna(liczona z definicji całki) wynosi: "<<descstr<<endl;
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