#include <bits/stdc++.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<string> lines;
vector<string> parameterList;

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
        string temp = parameterList[i];
        parameterList[i] = parameterList[i].substr(parameterList[i].find(' ')).substr(1);
        cout << parameterList[i] << endl;
    }
}

int main(int argc, char *argv[]) {
    readInputFile(argv[1]);
    getParameterList();
    for (int i = 0; i < lines.size(); i++) {
        cout << lines[i] << endl;
    }
    return 0;
}