#include <bits/stdc++.h>

#include <boost/algorithm/string.hpp>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class VariableCondition
{
public:
    string condition;
    string variableName;
    char operand;
    int lineNo;
    int rValue;
    int tValue;
    int fValue;

public:
    VariableCondition(int lineNum, string s)
    {
        condition = s;
        lineNo = lineNum;
    }
};

vector<string> lines;
vector<string> parameterList;
vector<string> conditions;
vector<VariableCondition> variableCondition;

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

string extractVariableName(string s)
{
    string tem = "";
    int index = 0;
    while (index < s.size() && (s[index] != '<' && s[index] != '>' && s[index] != '=' && s[index] != '!'))
    {
        tem += s[index];
        index++;
    }
    boost::algorithm::trim(tem);
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

char extractOperand(string s)
{
    int index = 0;
    while (index < s.size() && (s[index] != '<' && s[index] != '>' && s[index] != '=' && s[index] != '!'))
    {
        index++;
    }
    return s[index];
}

void readInputFile(char *fileName)
{
    ifstream inputFile(fileName);
    string temp;
    while (getline(inputFile, temp))
    {
        lines.push_back(trimLine(temp));
    }
}

void getParameterList()
{
    int startBracket = lines[0].find('(');
    string bracketParameters = lines[0].substr(startBracket + 1);
    int closeBracket = bracketParameters.find(')');
    string parameters = bracketParameters.substr(0, closeBracket);

    stringstream check1(parameters);
    string intermediate;

    while (getline(check1, intermediate, ','))
    {
        parameterList.push_back(intermediate);
    }
    for (int i = 0; i < parameterList.size(); i++)
    {
        parameterList[i] = parameterList[i].substr(parameterList[i].find(' ')).substr(1);
    }
}

bool hasCondition(string s)
{
    bool b = boost::algorithm::contains(s, "if") ||
             boost::algorithm::contains(s, "esle if") ||
             boost::algorithm::contains(s, "while");
    return b;
}

vector<string> extractConditions(string s)
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
            toRet.push_back(trimLine(tem));
            index += 2;
            tem = "";
            continue;
        }

        index++;
    }
    toRet.push_back(trimLine(tem));
    return toRet;
}

void getCondition(int lineNo, string s)
{
    int startBracket = s.find('(');
    string bracketParameters = s.substr(startBracket + 1);
    int closeBracket = bracketParameters.find(')');
    string conditionString = bracketParameters.substr(0, closeBracket);
    vector<string> conditions = extractConditions(conditionString);
    for (int i = 0; i < conditions.size(); i++)
    {
        VariableCondition newCondition(lineNo, conditions[i]);
        variableCondition.push_back(newCondition);
    }
}

int getTrueValue(int rVal, char op)
{
    if (op == '!')
        return rVal - 1;
    if (op == '>')
        return rVal + 1;
    if (op == '<')
        return rVal - 1;
    return rVal;
}

int getFalseValue(int rVal, char op)
{
    if (op == '=')
        return rVal - 1;
    if (op == '<')
        return rVal + 1;
    if (op == '>')
        return rVal - 1;
    return rVal;
}

void parseConditions()
{
    for (int i = 0; i < variableCondition.size(); i++)
    {
        variableCondition[i].variableName = extractVariableName(variableCondition[i].condition);
        variableCondition[i].operand = extractOperand(variableCondition[i].condition);
        variableCondition[i].rValue = extractValue(variableCondition[i].condition);
        variableCondition[i].tValue = getTrueValue(variableCondition[i].rValue, variableCondition[i].operand);
        variableCondition[i].fValue = getFalseValue(variableCondition[i].rValue, variableCondition[i].operand);
        cout << "line" << variableCondition[i].lineNo << " condition: " << variableCondition[i].condition << ",varName " << variableCondition[i].variableName << ",trueVal " << variableCondition[i].tValue << ",falseVal " << variableCondition[i].fValue << endl;
    }
}

void createTrueTestCase()
{
    int lineNo = variableCondition[0].lineNo;
    for (int i = 0; i < variableCondition.size(); i++)
    {
        cout << "Test " << i + 1 << ":" << endl;
        for (int j = 1; j < variableCondition.size(); j++)
        {
            for (int k = 0; k < parameterList.size(); k++)
            {
                string tempVerName = parameterList[k];
                if (variableCondition[j].variableName == tempVerName)
                {
                    if (variableCondition[i].lineNo == variableCondition[j].lineNo)
                    {
                        cout << tempVerName << " = " << variableCondition[j].tValue;
                    }
                }
            }
        }
        cout << endl;
    }
}

void createFalseTestCase()
{
    int lineNo = variableCondition[0].lineNo;
    for (int i = 0; i < variableCondition.size(); i++)
    {
        cout << "Test " << i + 1 << ":" << endl;
        for (int j = 1; j < variableCondition.size(); j++)
        {
            for (int k = 0; k < parameterList.size(); k++)
            {
                string tempVerName = parameterList[k];
                if (variableCondition[j].variableName == tempVerName)
                {
                    if (variableCondition[i].lineNo == variableCondition[j].lineNo)
                    {
                        cout << tempVerName << " = " << variableCondition[j].fValue;
                    }
                }
            }
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    readInputFile(argv[1]);
    getParameterList();
    int conditionNumber = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        if (hasCondition(lines[i]))
        {
            getCondition(i, lines[i]);
            conditions.clear();
            conditionNumber++;
        }
    }
    parseConditions();
    // createTrueTestCase();
    // createFalseTestCase();
    return 0;
}