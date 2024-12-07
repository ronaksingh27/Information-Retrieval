#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
using namespace std;

class matrix{
    int substitute;
    int insert;
    int deletion;
    int transposition;

    matrix(int rows, int cols)
    {
        (*this).substitute = rows + cols;
        (*this).insert = rows + cols;
        (*this).deletion = rows + cols;
        (*this).transposition = rows + cols;
    }
};


vector<int> demaruArray;
int damerauLevenshteinDistance(const string &a, const string &b) {
    // Step 1: Initialize da array (store last occurrences of characters in the alphabet Σ)
    unordered_map<char, int> da;

    // Step 2: Initialize d matrix
    int len_a = a.length();
    int len_b = b.length();
    int maxdist = len_a + len_b;
    
    vector<vector<int>> d(len_a + 2, vector<int>(len_b + 2, maxdist));

    d[0][0] = maxdist;
    for (int i = 1; i <= len_a + 1; ++i) {
        d[i][0] = i - 1;
        d[i][1] = maxdist;
    }
    for (int j = 1; j <= len_b + 1; ++j) {
        d[0][j] = j - 1;
        d[1][j] = maxdist;
    }

    // Step 3: Calculate the distance
    for (int i = 1; i <= len_a; ++i) {
        int db = 0;
        for (int j = 1; j <= len_b; ++j) {
            int k = da.count(b[j-1]) ? da[b[j-1]] : 0;
            int l = db;

            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            if (cost == 0) db = j;

            
            d[i+1][j+1] = min({
                d[i][j] + cost,           // substitution
                d[i+1][j] + 1,            // insertion
                d[i][j+1] + 1,            // deletion
                d[k][l] + (i-k) + 1 + (j-l) // transposition
            });
        }
        da[a[i-1]] = i;
    }

    return d[len_a + 1][len_b + 1];
}

int main() {
    string a = "saturday";
    string b = "cate";
    int distance = damerauLevenshteinDistance(a, b);
    cout << "Damerau-Levenshtein distance between \"" << a << "\" and \"" << b << "\" is: " << distance << endl;
    return 0;
}
