#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

// Structure to represent a posting list entry (docID and positions)
struct PostingListEntry {
    int docID; // Document ID
    vector<int> positions; // Positions of the term in the document
};

// Function to get the next element from the posting list
int next(const vector<int>& positions, int& index) {
    if (index < positions.size()) {
        return positions[index++];
    }
    return -1; // Return -1 if there are no more positions
}

// PositionalIntersect algorithm implementation
vector<tuple<int, int, int>> PositionalIntersect(const vector<PostingListEntry>& p1, const vector<PostingListEntry>& p2, int k) {
    vector<tuple<int, int, int>> answer; // Resulting list of matching document IDs and positions
    int i = 0, j = 0; // Indices for p1 and p2

    while (i < p1.size() && j < p2.size()) {
        if (p1[i].docID == p2[j].docID) {
            vector<int> l; // Temporary list to store valid positions
            vector<int> pp1 = p1[i].positions; // Positions list of p1
            vector<int> pp2 = p2[j].positions; // Positions list of p2
            int ii = 0, jj = 0;

            while (ii < pp1.size()) {
                while (jj < pp2.size()) {
                    if (abs(pp1[ii] - pp2[jj]) <= k) {

                        // printf("%d , %d = %d,%d\n",ii,jj,pp1[ii],pp2[jj]);
                        l.push_back(pp2[jj]);
                    } else if (pp2[jj] > pp1[ii]) {
                        break;
                    }
                    jj++;
                }

                while (!l.empty() && abs(l[0] - pp1[ii]) > k) {
                    l.erase(l.begin()); // Remove elements from l that are out of range
                }

                for (int ps : l) {
                    answer.emplace_back(p1[i].docID, pp1[ii], ps);
                }

                ii++;
            }

            i++;
            j++;
        } else if (p1[i].docID < p2[j].docID) {
            i++; // Move to the next document in p1
        } else {
            j++; // Move to the next document in p2
        }
    }

    return answer;
}

int main() {
    // Example usage
    vector<PostingListEntry> p1 = {
        {1, {1, 4, 7}},
        {2, {3, 6, 9}}
    };

    vector<PostingListEntry> p2 = {
        {1, {2, 5, 8}},
        {2, {4, 7, 10}}
    };

    int k = 2; // Maximum allowed distance between positions

    vector<tuple<int, int, int>> result = PositionalIntersect(p1, p2, k);

    // Print the result
    for (const auto& [docID, pos1, pos2] : result) {
        cout << "DocID: " << docID << ", Pos1: " << pos1 << ", Pos2: " << pos2 << endl;
    }

    return 0;
}
