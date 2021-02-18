#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cmath>
#include <list>

using namespace std;
#define MAX_BITS 12


string readFile(const char* argv[]) {
    string buffer;
    // Process only first argument as input file
    ifstream infile(argv[1]);
    
    for(string line; getline(infile, line); ) {
        buffer += line;
    }
    infile.close();

    return buffer;
}


void printCodes(vector<int> outCodes) {
    for (auto value : outCodes) {
        cout << value << " ";
    }
    cout << endl;
}


void printDict(map<int, string> const &dict) {
    for (auto& [key, value] : dict) {
        cout << value << " " << key << endl;
    }
}


vector<int> encode(string buffer, bool verbose) {
    map<string, int> dict;

    // Create codes for ASCII 8 bit long
    for (int i = 0; i < 256; i++) {
        string ch(1, char(i));
        dict[ch] = i;
    }

    vector<int> outCodes;
    int nextCode = dict.size();
    
    string cur(1, buffer[0]);
    string next = "";

    int maxSize = pow(2, MAX_BITS);
    for (int i = 0; i < buffer.length(); i++) {
        if (i != buffer.length() - 1) {
            next += buffer[i + 1];
        }

        if (dict.find(cur + next) != dict.end()) {
            // Symbol already exists
            cur += next;
        }
        else {
            // Symbol doesn't exist
            outCodes.push_back(dict[cur]);

            if (dict.size() < maxSize) {
                dict[cur + next] = nextCode;
                nextCode++;
            }

            cur = next;
        }

        next = "";
    }

    // For pretty printing in order of numeric values
    map<int, string> prettyDict;
    for (auto& [key, value] : dict) {
        prettyDict[value] = key;
    }

    if (verbose) {
        printDict(prettyDict);
    }

    return outCodes;
}


void binaryWrite(ofstream& fout, const vector<vector<bool>>& x)
{
    for (auto& vec : x) {
        vector<bool>::size_type n = vec.size();
        fout.write((const char*)&n, sizeof(vector<bool>::size_type));

        for(vector<bool>::size_type i = 0; i < n;)
        {
            unsigned char aggr = 0;
            for(unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
                if(vec.at(i))
                    aggr |= mask;
            fout.write((const char*)&aggr, sizeof(unsigned char));
        }
    }
}


vector<bool> convertToBin(int value) {
    list<bool> binaryVal;
    for (int i = 0; i < MAX_BITS; i++) {
        binaryVal.push_front((value >> i) & 1);
    }

    vector<bool> output {
        make_move_iterator(begin(binaryVal)),
        make_move_iterator(end(binaryVal))
    };

    return output;
}


vector<vector<bool>> createBinCodes(vector<int> codes) {
    vector<vector<bool>> binCodes;
    for (auto val : codes) {
        binCodes.push_back(convertToBin(val));
    }

    return binCodes;
}


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cerr << "Input file must be supplied!" << endl;
        return -1;
    }

    string buffer = readFile(argv);
    vector<int> codes = encode(buffer, true);

    vector<vector<bool>> binCodes = createBinCodes(codes);

    // Output "encoded" sequence
    // printCodes(codes);

    return 0;
}
