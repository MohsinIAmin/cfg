#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector<string> lines;
int curIndex = 0, cyclomaticComplexity = 2;
bool visited[1000] = {false};
int graph[1000][1000] = {0};
int levelNodeCount[1000];

class Node
{
public:
    int lineNo;
    vector<Node *> children;
    bool hasBackedge;

public:
    Node(int indexNo)
    {
        lineNo = indexNo;
        hasBackedge = false;
    }
    void addChild(Node *node);
};

Node *root;

void Node::addChild(Node *node)
{
    children.push_back(node);
}

string trimLine(string s)
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

bool hasIf(string s)
{
    s = s.substr(0, 2);
    return s == "if";
}

bool hasElseIf(string s)
{
    s = s.substr(0, 7);
    return s == "else if";
}

bool hasElse(string s)
{
    s = s.substr(0, 4);
    return s == "else";
}

bool hasWhile(string s)
{
    s = s.substr(0, 5);
    return s == "while";
}

bool hasFor(string s)
{
    return s.substr(0, 3) == "for";
}

bool hasLoop(string s)
{
    return hasFor(s) || hasWhile(s);
}

bool isEnd(string s)
{
    return s[s.size() - 2] == '}';
}

Node *buildCFG(Node *root, bool isLoop)
{
    Node *parent = root;
    vector<Node *> branches;
    while (curIndex < lines.size())
    {
        Node *curNode = new Node(curIndex);
        if (hasLoop(lines[curIndex]) || hasIf(lines[curIndex]))
        {
            cyclomaticComplexity++;
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
                parent->children.push_back(curNode);
            }
            curIndex++;
            if (hasLoop(lines[curIndex - 1]))
            {
                branches.push_back(curNode);
                buildCFG(curNode, true);
            }
            else
            {
                branches.push_back(buildCFG(curNode, false));
            }
        }
        else if (hasElseIf(lines[curIndex]) || hasElse(lines[curIndex]))
        {
            if (hasElseIf(lines[curIndex]))
            {
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

void traverse(Node *current, int level)
{
    levelNodeCount[level]++;
    visited[current->lineNo] = true;
    for (int i = 0; i < current->children.size(); i++)
    {
        if (!visited[current->children[i]->lineNo])
        {
            traverse(current->children[i], level + 1);
        }
    }
}

void calculateCyclomaticComplexity()
{
    root = new Node(curIndex++);
    buildCFG(root, false);
    traverse(root, 0);
}

void readInputFile(string fileName)
{
    ifstream inputFile(fileName.c_str());
    string temp;
    while (getline(inputFile, temp))
    {
        lines.push_back(trimLine(temp));
    }
}

int main(int argc, char *argv[])
{
    readInputFile(argv[1]);
    calculateCyclomaticComplexity();
    cout << "Cyclomatic complexity: " << cyclomaticComplexity << endl;
    return 0;
}
