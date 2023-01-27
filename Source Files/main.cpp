#include"crow_all.h"
#include <bits/stdc++.h>

using namespace std;

int k = 1;
int wire = 1;

void invertInputs(vector<vector<string>>&inputs)
{
    for(int i =0 ; i<(int)inputs.size(); i++)
    {
        for(int j = 0; j<(int)inputs[i].size(); j++)
        {
            if(inputs[i][j].size() < 2)
            {
                inputs[i][j].insert(inputs[i][j].begin()+1,'\'');
            }
            else
            {
                inputs[i][j].erase(inputs[i][j].begin()+1);
            }
        }
    }
        
}

struct CMOS {
    string name, drain, gate, source, body, type;
    CMOS();
    CMOS (string,string,string,string,string,string);
    void print_netlist();
    string get_netlist();
};

CMOS::CMOS() {
    this->name = ""; this->drain = ""; this->gate = ""; this->source = ""; this->body = ""; this->type = "";
}

CMOS::CMOS(string name, string drain, string gate, string source, string body, string type ) {
    this->name = name; this->drain = drain; this->gate = gate; this->source = source; this->body = body; this->type = type;
}
void CMOS::print_netlist() {
    cout << this->name << " " << this->drain << " " << this->gate << " " << this->source << " " << this->body << " " << this->type << "\n";
}
string CMOS::get_netlist() {
    return this->name + " " + this->drain + " " + this->gate + " " + this->source + " " + this->body + " " + this->type + "\n";
}

string pullUp(string logicFunction, char in)
{
    string netlist = "";
    netlist += "The Pull Up Network \n";
    vector<string> boolFunctions;
    istringstream ss(logicFunction);
    string str;
    while(getline(ss, str, '|'))
    {
        boolFunctions.push_back(str);
    }
    vector<vector<string>> inputs((int)boolFunctions.size());
    for(int i = 0; i<(int)boolFunctions.size(); i++)
    {

        istringstream splitByAnd(boolFunctions[i]);
        string s;
        
        while(getline(splitByAnd, s, '&'))
        {
            inputs[i].push_back(s);
        }
    }
    invertInputs(inputs);
    for(auto u : inputs)
    {
        for(auto k:u)
        {
            cout << k << endl;
        }
    }
    vector<CMOS> vec;
    for(int i = 0; i<(int)inputs.size(); i++)
    {
        for(int j = 0; j<(int)inputs[i].size(); j++)
        {
            string name = string(1,'M') + to_string(k);
            CMOS c;
            if(j==0 && (int)inputs[i].size() == 1)
                c = CMOS(name, string(1, in), inputs[i][j], "VDD", "VDD", "PMOS");
            else if(j==0)
                c = CMOS(name, to_string(wire), inputs[i][j], "VDD", "VDD", "PMOS");
            else if(j == (int)inputs[i].size()-1)
                c = CMOS(name, string(1, in), inputs[i][j], to_string(wire-1), "VDD", "PMOS");
            else
                c = CMOS(name, to_string(wire), inputs[i][j], to_string(wire-1), "VDD", "PMOS");
            wire++;
            k++;
            vec.push_back(c);
        }
    }
    for(int i = 0; i<(int)vec.size(); i++)
    {
        cout << vec[i].get_netlist() << endl;
        netlist += vec[i].get_netlist();
    }
    return netlist;
}
string deMorgan(string boolFunction)
{
    for(int i = 0; i<(int)boolFunction.size(); i++)
    {
        if(boolFunction[i] == '&')
            boolFunction[i] = '|';
        else if(boolFunction[i] == '|')
            boolFunction[i] = '&';
        else if( (boolFunction[i]>='a' && boolFunction[i]<='z' && boolFunction[i+1] !='\'') || (boolFunction[i]>='A' && boolFunction[i]<='Z' && boolFunction[i+1] !='\'') )
            boolFunction.insert(boolFunction.begin()+i+1,'\'');
        else if( (boolFunction[i]>='a' && boolFunction[i]<='z' && boolFunction[i+1] =='\'') || (boolFunction[i]>='A' && boolFunction[i]<='Z' && boolFunction[i+1] == '\'') )
            boolFunction.erase(boolFunction.begin()+i+1);
    }
    return boolFunction;
}

string pullDown(string logicFunction, char in)
{
    string netlist = "";
    netlist += "The Pull Down Network \n";
    string outputInverted = deMorgan(logicFunction);
    netlist += "Output Inverted: " + outputInverted + "\n";
    vector<string> boolFunctions;
    istringstream ss(outputInverted);
    string str;
    while(getline(ss, str, '&'))
    {
        boolFunctions.push_back(str);
    }
    vector<vector<string>> inputs((int)boolFunctions.size());
    for(int i = 0; i<(int)boolFunctions.size(); i++)
    {

        istringstream splitByOR(boolFunctions[i]);
        string s;
        
        while(getline(splitByOR, s, '|'))
        {
            inputs[i].push_back(s);
        }
    }
    vector<CMOS> vec;
    for(int i = 0; i<(int)inputs.size(); i++)
    {
        for(int j = 0; j<(int)inputs[i].size(); j++)
        {
            string name = string(1,'M') + to_string(k);
            CMOS c;
            if(i==0 && (int)inputs.size() == 1)
                c = CMOS(name, string(1,in), inputs[i][j], string(1,'0'), string(1,'0'), "NMOS");
            else if(i==0)
                c = CMOS(name, string(1,in), inputs[i][j], to_string(wire), string(1,'0'), "NMOS");
            else if(i == (int)inputs.size()-1)
                c = CMOS(name, to_string(wire-1), inputs[i][j], string(1,'0'), string(1,'0'), "NMOS");
            else
                c = CMOS(name, to_string(wire-1), inputs[i][j], to_string(wire), string(1,'0'), "NMOS");
            k++;
            vec.push_back(c);
        }
        wire++;
    }
    for(int i = 0; i<(int)vec.size(); i++)
        netlist += vec[i].get_netlist();

    return netlist;
}

bool valid_input(string eq)
{
    int index = eq.substr(1,eq.size()-1).find(eq[0]);
    if(index < 0 || index >= eq.size()){
        return true;
    }
    return false;
}

void semi(string &s)
{
    if(s[s.size()-2] != ';')
    {
        s[s.size()-1] = ';';
        cout << s << endl;
    }
}

string get_result(string input)
{
    if(!valid_input(input))
    {
        return "The Input is not Valid.\nOutput Must be Different from Inputs.\n";
    }
    string output;
    string result = "The Equation --> ";
    if(input[1] != '\'')
        output = input.substr(2,input.size()-2);
    else
        output = input.substr(3,input.size()-2);
    result += input + "\n";
    result += pullUp(output, input[0]);
    result += pullDown(output, input[0]);
    return result;
}

string solve(string eqs)
{
    semi(eqs);
    cout << eqs << endl;
    istringstream s(eqs);
    string eq;
    vector<string> equations;
    string results = "";
    while(getline(s, eq, ';'))
    {
        if(eq.size() > 1)
        {
            cout << eq << endl;
            equations.push_back(eq);
        }
    }
    for(auto u: equations)
    {
        results += get_result(u) + "\n";
    }
    return results;
}

int main()
{
    crow::SimpleApp app;
    auto page = crow::mustache::load("ui.html");

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([page](const crow::request& req){        
        if (req.method == crow::HTTPMethod::POST)
        {
            stringstream ss(req.body);
            string eq; vector<string> lines;
            while(getline(ss, eq))
            {
                lines.push_back(eq);
            }
            eq = lines[3];
            string result = solve(eq);
            crow::json::wvalue wv = {{"result", result}};
            crow::mustache::context &ctx = wv;
            cout << "POST" << endl;
            k = 1;
            wire = 1;
            return page.render(ctx);
        }
        else 
        {
            cout << "GET" << endl;
            return page.render();
        }
    });

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
}