
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

vector<string> split(string input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;
 
    while (getline(ss, temp, delimiter)) {
        if (!temp.empty() && temp[0] == ':')
            temp.erase(temp.begin());
        while (temp[temp.length() - 1] == '\r' || temp[temp.length() - 1] == '\n')
            temp.erase(temp.length() - 1);
        if (temp.length() == 0)
            continue ;
        answer.push_back(temp);
    }
 
    return answer;
}
