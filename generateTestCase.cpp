#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

#define NEG_INF INT32_MIN
#define POS_INF INT32_MAX

vector<string> lines;
int curIndex = 0, cyclomaticComplexity = 2;

class Node
{
public:
    int lineNo;
    int x, y, positionInLevel;
    vector<Node *> children;
    vector<string> conditions;
    vector<char> operators;
    bool hasBackedge;
    bool loopInitiated = false, isLoop = false;
    int loopStart, loopEnd, loopIncrement;

public:
    Node(int plineNo)
    {
        lineNo = plineNo;
        hasBackedge = false;
    }
    void addChild(Node *node);
};

Node *root;

class window
{
public:
    int minn, maxx;

public:
    window(int mi, int ma)
    {
        minn = mi;
        maxx = ma;
    }
};

class varRanges
{
public:
    string var;
    vector<window> windows;

public:
    varRanges(string ivar)
    {
        var = ivar;
    }
};

vector<varRanges> vars;

string extractVar(string s)
{
    string tem = "";
    int index = 0;
    while (index < s.size() && (s[index] != '<' && s[index] != '>' && s[index] != '=' && s[index] != '!'))
    {

        tem += s[index];
        index++;
    }
    return tem;
}

int extractValue(string s)
{
    string tem = "";
    int index = s.size() - 1;
    while (index >= 0 && (s[index] != '<' && s[index] != '>' && s[index] != '='))
    {
        tem += s[index];
        index--;
    }
    reverse(tem.begin(), tem.end());
    int ret = stoi(tem);
    return ret;
}

char extractOP(string s)
{
    int index = 0;
    while (index < s.size() && (s[index] != '<' && s[index] != '>' && s[index] != '=' && s[index] != '!'))
    {
        index++;
    }
    return s[index];
}

bool varExist(string v)
{
    for (int i = 0; i < vars.size(); i++)
    {
        if (v == vars[i].var)
        {
            return true;
        }
    }
    return false;
}

void Node::addChild(Node *node)
{
    children.push_back(node);
}

string trim(string s)
{
    int i = 0, j = 0;
    while (s[i] == ' ')
    {
        i++;
    }
    s = s.substr(i, s.size());
    i = s.size();
    while (s[i] == ' ' || s[i] == '\n')
    {
        i--;
    }
    s = s.substr(0, i);
    return s;
}

bool containsIf(string s)
{
    s = s.substr(0, 2);
    return s == "if";
}

bool containsElseIf(string s)
{
    s = s.substr(0, 7);
    return s == "else if";
}

bool containsElse(string s)
{
    s = s.substr(0, 4);
    return s == "else";
}

bool containsWhile(string s)
{
    s = s.substr(0, 5);
    return s == "while";
}

bool containsFor(string s)
{
    return s.substr(0, 3) == "for";
}

bool containsLoop(string s)
{
    return containsFor(s) || containsWhile(s);
}

bool isEnd(string s)
{
    return s[s.size() - 2] == '}';
}

bool containsMain(string s)
{
    return s.substr(0, 10) == "int main()";
}

string getItemsInsideIf(string s)
{
    int index = 3;
    string ret = "";
    while (s[index] != ')')
    {
        ret += s[index];
        index++;
    }
    return ret;
}

string getItemsInsideElseIf(string s)
{
    int index = 8;
    string ret = "";
    while (s[index] != ')')
    {
        ret += s[index];
        index++;
    }
    return ret;
}

vector<string> detachIfMultipleCondition(string s)
{
    int index = 0;
    vector<string> toRet;
    string tem = "";
    while (index < s.size())
    {
        tem += s[index];
        if (index + 1 < s.size() && ((s[index] == '&' && s[index + 1] == '&') || (s[index] == '|' && s[index + 1] == '|')))
        {
            tem[tem.size() - 1] = '\0';
            toRet.push_back(trim(tem));
            index += 2;
            tem = "";
            continue;
        }

        index++;
    }
    toRet.push_back(trim(tem));
    return toRet;
}

vector<char> getOperators(string s)
{
    int index = 0;
    vector<char> toRet;
    while (index < s.size())
    {
        if (index + 1 < s.size() && ((s[index] == '&' && s[index + 1] == '&') || (s[index] == '|' && s[index + 1] == '|')))
        {
            if (s[index] == '&' && s[index + 1] == '&')
                toRet.push_back('&');
            else
                toRet.push_back('|');
            index += 2;
            continue;
        }
        index++;
    }
    return toRet;
}

string getItemsInsideLoop(string s)
{
    string toRet = "";
    if (containsWhile(s))
    {
        int index = 6;
        while (index < s.size() && s[index] != ')')
        {
            toRet += s[index];
            index++;
        }
    }
    else
    {
    }
    return toRet;
}

Node *buildCFG(Node *root, bool isLoop)
{

    Node *parent = root;
    vector<Node *> branches;
    while (curIndex < lines.size())
    {
        Node *curNode = new Node(curIndex);

        if (containsLoop(lines[curIndex]) || containsIf(lines[curIndex]))
        {
            cyclomaticComplexity++;
            if (branches.size() > 0)
            {
                for (int i = 0; i < branches.size(); i++)
                {
                    branches[i]->children.push_back(curNode);
                }
                branches.clear(); // test
            }
            else
            {
                parent->children.push_back(curNode);
            }
            curIndex++;
            if (containsLoop(lines[curIndex - 1]))
            {
                branches.push_back(curNode);
                if (containsWhile(lines[curIndex - 1]))
                {
                    string tem = getItemsInsideLoop(lines[curIndex - 1]);
                    // cout << "tem: " << tem << endl;
                    vector<string> conditions = detachIfMultipleCondition(tem);
                    vector<char> operators = getOperators(tem);
                    curNode->operators = operators;
                    curNode->conditions = conditions;
                }
                buildCFG(curNode, true);
            }
            else
            {
                string tem = getItemsInsideIf(lines[curIndex - 1]);
                // cout << "tem: " << tem << endl;
                vector<string> conditions = detachIfMultipleCondition(tem);
                vector<char> operators = getOperators(tem);
                curNode->operators = operators;
                curNode->conditions = conditions;
                Node *temp = buildCFG(curNode, false);
                branches.push_back(temp);
            }
        }
        else if (containsElseIf(lines[curIndex]) || containsElse(lines[curIndex]))
        {
            if (containsElseIf(lines[curIndex]))
            {
                string tem = getItemsInsideElseIf(lines[curIndex]);
                vector<string> conditions = detachIfMultipleCondition(tem);
                vector<char> operators = getOperators(tem);
                curNode->operators = operators;
                curNode->conditions = conditions;
                cyclomaticComplexity++;
            }
            parent->children.push_back(curNode);
            curIndex++;
            branches.push_back(buildCFG(curNode, false));
        }
        else
        {
            if (branches.size() > 0)
            {
                for (int i = 0; i < branches.size(); i++)
                {
                    branches[i]->children.push_back(curNode);
                }
                branches.clear();
            }
            else
            {
                parent->addChild(curNode);
            }
            curIndex++;
            if (isEnd(lines[curIndex - 1]))
            {

                if (isLoop)
                {
                    root->hasBackedge = true;

                    curNode->children.push_back(root);
                }
                return curNode;
            }
            parent = curNode;
        }
    }
    return NULL;
}
bool visited[5000] = {false};
int graph[5000][5000] = {0};
int levelNodeCount[5000];

void traverse(Node *current, int level)
{
    levelNodeCount[level]++;
    visited[current->lineNo] = true;
    // for(auto a: current->conditions){
    //     cout<< a << " ";
    // }
    // cout<< endl;
    // for(auto a: current->operators){
    //     cout<< a << " ";
    // }
    // cout<< endl;
    // cout<< current->lineNo+1 << endl;
    // for( auto a: current->children){
    //     cout<< a->lineNo+1 << " ";
    // }
    // cout<< endl;
    for (int i = 0; i < current->children.size(); i++)
    {
        if (!visited[current->children[i]->lineNo])
        {
            traverse(current->children[i], level + 1);
        }
    }
}

void reset()
{
    for (int i = 0; i < 5000; i++)
    {
        visited[i] = false;
    }
}

window *getWindow(char op, int value, string var)
{
    int minn, maxx;
    if (op == '<')
    {
        minn = NEG_INF;
        maxx = value - 1;
    }
    else if (op == '>')
    {
        minn = value + 1;
        maxx = POS_INF;
    }
    else if (op == '=')
    {
        minn = value;
        maxx = value;
    }
    else if (op == '!')
    {
        cout << "called" << endl;
        bool ff = false;
        if (varExist(var))
        {
            for (int i = 0; i < vars.size(); i++)
            {
                if (var == vars[i].var)
                {
                    for (auto a : vars[i].windows)
                    {
                        cout << a.minn << " " << a.maxx << endl;
                        if (a.minn == NEG_INF)
                        {
                            maxx = POS_INF;
                            minn = value + 1;
                            ff = true;
                        }
                    }
                }
                if (!ff)
                {
                    cout << "siam" << endl;
                    minn = NEG_INF;
                    maxx = value - 1;
                }
            }
        }
        else
        {
            minn = NEG_INF;
            maxx = value - 1;
        }
    }
    window *toRet = new window(minn, maxx);
    return toRet;
}

void prepareVariableWindows(Node *current, int level)
{
    // cout<< "c" <<endl;
    visited[current->lineNo] = true;
    vector<string> conditions = current->conditions;
    vector<char> operators = current->operators;

    for (int i = 0; i < conditions.size(); i++)
    {
        string var = extractVar(conditions[i]);
        int value = extractValue(conditions[i]);
        char op = extractOP(conditions[i]);
        cout << var << " " << value << " " << op << endl;
        window *wn = getWindow(op, value, var);
        if (varExist(var))
        {
            for (int i = 0; i < vars.size(); i++)
            {
                if (var == vars[i].var)
                {
                    vars[i].windows.push_back(*wn);
                }
            }
        }
        else
        {
            varRanges *varR = new varRanges(var);
            varR->windows.push_back(*wn);
            cout << "pushed" << endl;
            vars.push_back(*varR);
        }
        // window *wn = getWindow()
    }
    // for(auto a: current->conditions){
    //     cout<< a << " ";
    // }
    // cout<< endl;
    // for(auto a: current->operators){
    //     cout<< a << " ";
    // }
    // cout<< endl;
    // cout<< current->lineNo+1 << endl;
    // for( auto a: current->children){
    //     cout<< a->lineNo+1 << " ";
    // }
    // cout<< endl;

    for (int i = 0; i < current->children.size(); i++)
    {
        // cout<< visited[current->children[i]->lineNo] <<endl;
        if (!visited[current->children[i]->lineNo])
        {
            // cout << "h" <<endl;
            prepareVariableWindows(current->children[i], level + 1);
        }
    }
}

void init()
{
    prepareVariableWindows(root, 0);
    cout << "varsize: " << vars.size() << endl;
    for (auto a : vars)
    {
        cout << "var: " << a.var << endl;
        cout << "windows: ";
        for (auto b : a.windows)
        {
            cout << b.minn << " " << b.maxx << endl;
        }
    }
}

void calculateCyclomaticComplexity()
{
    root = new Node(curIndex++);
    // curIndex++;
    buildCFG(root, false);
    traverse(root, 0);
    reset();
}

void readInputFile(string fileName)
{
    ifstream inputFile(fileName.c_str());
    string temp;
    while (getline(inputFile, temp))
    {
        lines.push_back(trim(temp));
    }
}

int main(int argc, char *argv[])
{
    readInputFile("hello.c");
    calculateCyclomaticComplexity();
    cout << "Cyclomatic complexity: " << cyclomaticComplexity << endl;

    init();
    /*string in =  getItemsInsideElseIf("else if(a>b && b>c || c<a)");
    vector<string> detached = detachIfMultipleCondition(in);
    for(auto a:detached){
        cout<< a<< endl;
    }
    vector<char> operators = getOperators(in);
    for(auto a:operators){
        cout<< a<< endl;
    }*/
    return 0;
}
