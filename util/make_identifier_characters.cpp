#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

char tab[65536];

vector<string> split(const string& s, const string& delim) {
    vector<string> result;
    string::size_type ofs = 0;
    while (ofs < s.size()) {
        string::size_type next = s.find(delim, ofs);
        if (next == string::npos)
            next = s.size();
        result.push_back(s.substr(ofs, next-ofs));
        ofs = next + delim.size();
    }
    return result;
}

string trim(string s)
{
    while (!s.empty() && isspace(s[0]))
        s.erase(0,1);
    while (!s.empty() && isspace(s[s.size()-1]))
        s.erase(s.size()-1);
    return s;
}

void add(const string& s)
{
    int start, end;
    char dash;
    istringstream in(s);
    in>>hex>>start;
    if (!(in>>dash>>hex>>end)) {
        end = start;
    }
    for (int i = start; i <= end; ++i)
        tab[i]=1;
}

int main() {
    memset(tab,0,sizeof(tab));
    string line;
    while (getline(cin, line)) {
        if (line[0] == '#')
            continue;

        string::size_type i = line.find(':');
        if (i == string::npos)
            continue;

        ++i;
        vector<string> data = split(trim(line.substr(i)), ",");
        for (vector<string>::const_iterator it = data.begin(); it != data.end(); ++it)
            add(trim(*it));
    }

    cout<<"const char IDENTIFIER_CHARACTERS[] = {";
    for (int i = 0; i < 65536; ++i) {
        if ((i%36) == 0)
            cout<<endl;
        cout<<static_cast<int>(tab[i])<<',';
    }
    cout<<endl;
    cout<<"};"<<endl;

    return 0;
}

