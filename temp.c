#include <bits/stdc++.h>

#include <boost/algorithm/string.hpp>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class VariableCondition {
   public:
    string condition;
    vector<string> varNames;
    int lineNo;

   public:
    VariableCondition(int lineNum, string s) {
        condition = s;
        lineNo = lineNum;
    }
};

vector<string> lines;
vector<string> parameterList;
vector<string> conditions;
vector<VariableCondition> variableCondition;

string trimLine(string s) {
    int i = 0, j = 0;
    while (s[i] == ' ') {
        i++;
    }
    s = s.substr(i, s.size());
    i = s.size();
    while (s[i] == ' ' || s[i] == '\n') {
        i--;
    }
    s = s.substr(0, i);
    return s;
}

void readInputFile(char *fileName) {
    ifstream inputFile(fileName);
    string temp;
    while (getline(inputFile, temp)) {
        lines.push_back(trimLine(temp));
    }
}

void getParameterList() {
    int startBracket = lines[0].find('(');
    string bracketParameters = lines[0].substr(startBracket + 1);
    int closeBracket = bracketParameters.find(')');
    string parameters = bracketParameters.substr(0, closeBracket);

    stringstream check1(parameters);
    string intermediate;

    while (getline(check1, intermediate, ',')) {
        parameterList.push_back(intermediate);
    }
    for (int i = 0; i < parameterList.size(); i++) {
        // string temp = parameterList[i];
        parameterList[i] = parameterList[i].substr(parameterList[i].find(' ')).substr(1);
        // cout << parameterList[i] << endl;
    }
}

bool hasCondition(string s) {
    bool b = boost::algorithm::contains(s, "if") ||
             boost::algorithm::contains(s, "esle if") ||
             boost::algorithm::contains(s, "while");
    return b;
}

vector<string> extractConditions(int lineNo, string s) {
    int index = 0;
    vector<string> toRet;
    string tem = "";
    while (index < s.size()) {
        tem += s[index];
        if (index + 1 < s.size() && ((s[index] == '&' && s[index + 1] == '&') || (s[index] == '|' && s[index + 1] == '|'))) {
            tem[tem.size() - 1] = '\0';
            VariableCondition newVariableCondition(lineNo, trimLine(tem));
            variableCondition.push_back(newVariableCondition);
            toRet.push_back(trimLine(tem));
            index += 2;
            tem = "";
            continue;
        }
        index++;
    }
    VariableCondition newVariableCondition(lineNo, trimLine(tem));
    variableCondition.push_back(newVariableCondition);
    toRet.push_back(trimLine(tem));
    return toRet;
}

void getCondition(int lineNo, string s) {
    int startBracket = s.find('(');
    string bracketParameters = s.substr(startBracket + 1);
    int closeBracket = bracketParameters.find(')');
    string conditionString = bracketParameters.substr(0, closeBracket);
    vector<string> conditions = extractConditions(lineNo, conditionString);
    for (int i = 0; i < variableCondition.size(); i++) {
        // VariableCondition newCondition(lineNo, conditions[i]);
        cout << variableCondition[i].lineNo << ' ' << variableCondition[i].condition << endl;
    }
}

int main(int argc, char *argv[]) {
    readInputFile(argv[1]);
    getParameterList();
    for (int i = 0; i < lines.size(); i++) {
        if (hasCondition(lines[i])) {
            getCondition(i, lines[i]);
        }
    }
    // hasCondition(lines[3]);
    // getCondition(lines[3]);
    // for (int i = 0; i < lines.size(); i++) {
    //     cout << lines[i] << endl;
    // }
    return 0;
}