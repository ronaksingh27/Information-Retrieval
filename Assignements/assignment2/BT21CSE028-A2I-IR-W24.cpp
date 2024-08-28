#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Matrix{
public:
    int substitution;
    int insertion;
    int deletion;
    int transposition;
    int ans;
    int row;
    int col;
    int n;

    // Default constructor
    Matrix() : substitution(0), insertion(0), deletion(0),transposition(0), ans(0), n(-1) {}

    // Parameterized constructor
    Matrix(int rows, int cols, int i, int j)
    {
        substitution = rows + cols;
        insertion = rows + cols;
        deletion = rows + cols;
        transposition = rows + cols;
        ans = -1;
        row = i;
        col = j;
        n = -1;
    }
};

// Global vector to store Matrix objects
vector<Matrix> arr;

void print_matrix(int len1 ,int len2)
{
    int row = 0;

    // Print each Matrix in the reversed vector
    for (int i = 0; i < arr.size(); ++i) {
        
        
        Matrix& m = arr[i];
        printf("(%d,%d) : ",m.row,m.col);
        m.n == 0 ? cout << "*S: " << m.substitution << " , " :  cout << "S: " << m.substitution << " , ";
        m.n == 1 ? cout << "*I: " << m.insertion << " , " : cout << "I: " << m.insertion << " , ";
        m.n == 2 ? cout << "*D: " << m.deletion << " , " : cout << "D: " << m.deletion << " , ";
        m.n == 3 ? cout << "*T: " << m.transposition << " , " : cout << "T: " << m.transposition << " , ";
        cout << "MIN : " << m.ans << " | ";
        cout << endl;

        if( (i+1) % len2 == 0) {
            row++;
            printf("------------------------------------------------");
        }
        cout << endl;
    }
}

class RCS{
    public:
    int row;
    int col;
    string s;

    RCS(){};

    RCS(int row, int col , string s)
    {
        (*this).row = row;
        (*this).col = col;
        (*this).s = s;
    }
};

vector<RCS> ans;
void print_ops()
{
    Matrix t = arr[arr.size()-1];

    int row = t.row;
    int col = t.col;

    RCS *rcs;
    while( row != 0 || col != 0 )
    {
        if( t.n == 0 )
        {
            row = row-1;
            col = col-1;

            rcs = new RCS(row,col,"S"); 
            
        }
        else if( t.n == 1 )
        {
            col = col-1;
            rcs = new RCS(row,col,"I"); 
        }
        else if( t.n == 2 )
        {
            row = row-1;
            rcs = new RCS(row,col,"D"); 
        }
        else if( t.n == 3 )
        {
            row = row-2;
            col = col-2;
            rcs = new RCS(row,col,"T"); 
        }

        ans.emplace_back(*rcs);
        free(rcs);
        rcs = NULL;

        for( int i = 0; i < arr.size() ; i++ )
        {
            if( arr[i].row == row && arr[i].col == col )
            {
                t.n = arr[i].n;
                break;
            }
        }
    }

    reverse(ans.begin(),ans.end());
    for( auto it : ans )
    {   
        cout<<it.s<<" : ";

        if( it.s == "S" ) printf("(%d,%d) -> (%d,%d) \n",it.row,it.col,it.row+1,it.col+1);
        else if( it.s == "I" ) printf("(%d,%d) -> (%d,%d) \n",it.row,it.col,it.row,it.col+1);
        else if( it.s == "D" ) printf("(%d,%d) -> (%d,%d) \n",it.row,it.col,it.row+1,it.col);
        else if( it.s == "T" ) printf("(%d,%d) -> (%d,%d) \n",it.row,it.col,it.row+2,it.col+2);
        

    }cout<<endl;

}

// Function to calculate the Damerau-Levenshtein distance
int damerauLevenshteinDistance(const string& s1, const string& s2) {
    int len1 = s1.size();
    int len2 = s2.size();

    // Create a 2D vector to store the distances
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    // Initialize the dp table
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }

    // Fill the dp table
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            // Check if the characters are equal
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            int substitution = dp[i - 1][j - 1] + cost;
            int insertion = dp[i][j - 1] + 1;
            int deletion = dp[i - 1][j] + 1;
            int transposition = len1 + len2 ;

            // Calculate the minimum cost for substitution, insertion, and deletion
            dp[i][j] = min({deletion, insertion, substitution});

            // Check for adjacent transposition
            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2] + 1); // Transposition
                transposition = dp[i][j];
            }

            // Create a Matrix object with the current costs
            Matrix *m = new Matrix(len1, len2,i,j);
            m->substitution = substitution;
            m->insertion = insertion;
            m->deletion = deletion;
            m->transposition = transposition;
            

            // Determine the smallest operation cost and set `n`
            int minCost = min({substitution, insertion, deletion,transposition});
            if (minCost == substitution) {
                m->n = 0; // Substitution
            } else if (minCost == insertion) {
                m->n = 1; // Insertion
            } else if (minCost == deletion) {
                m->n = 2; // Deletion
            } 
            else if (minCost == transposition) {
                m->n = 3; // Transposition
            } 
            m->ans = minCost;

            // Add the matrix to the vector
            arr.push_back(*m);
            delete m;
        }
    }

    // reverse(arr.begin(), arr.end());
    print_matrix(len1, len2);
    print_ops();

    // Return the calculated Damerau-Levenshtein distance
    return dp[len1][len2];
}

int main() {
    string s1 ;
    string s2 ;

    bool dft = true;

    printf("use default strings ? \nPress 1 for Yes \nPress 0 for No\n");
    printf("Choose option : ");

    int n;
    cin>>n;

    n == 1 ? dft = true : dft = false;

    if( dft )
    {
        s1 = "saturday";
        s2 = "sunday";
    }
    else
    {
        printf("Enter string 1 : ");
        cin>>s1;

        printf("Enter string 2 : ");
        cin>>s2;
    }
    


    // Calculate the Damerau-Levenshtein distance
    int distance = damerauLevenshteinDistance(s1, s2);
    cout << "Damerau-Levenshtein Distance: " << distance << endl;

    return 0;
}
