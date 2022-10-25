
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

vector<string> split(string input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;
 
    while (getline(ss, temp, delimiter)) {
        if (temp[temp.length() - 1] == '\r')
            temp.erase(temp.length() - 1);
        answer.push_back(temp);
    }
 
    return answer;
}
