#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;


// ============================================================
// GRAY CODE
// Columns -> AB
// Rows    -> CD
// ============================================================

string gray[4] = {"00", "01", "11", "10"};


// ============================================================
// READ K-MAP FROM FILE
// ============================================================

bool readKMap(string filename, int kmap[4][4])
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Cannot open input.txt\n";
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            file >> kmap[i][j];

            // Only 0 and 1 are allowed
            if (kmap[i][j] != 0 && kmap[i][j] != 1)
            {
                cout << "Invalid input. Only 0 and 1 are allowed.\n";
                return false;
            }
        }
    }

    return true;
}


// ============================================================
// GET A B C D VALUES OF A CELL
// ============================================================

vector<int> getABCD(int row, int col)
{
    vector<int> value(4);

    // Column gives AB
    value[0] = gray[col][0] - '0';   // A
    value[1] = gray[col][1] - '0';   // B

    // Row gives CD
    value[2] = gray[row][0] - '0';   // C
    value[3] = gray[row][1] - '0';   // D

    return value;
}


// ============================================================
// CHECK WHETHER A GROUP CONTAINS ONLY 1s
// ============================================================

bool isValidGroup(
    int kmap[4][4],
    vector<pair<int, int>> cells)
{
    for (auto cell : cells)
    {
        int row = cell.first;
        int col = cell.second;

        if (kmap[row][col] == 0)
            return false;
    }

    return true;
}


// ============================================================
// CHECK WHETHER TWO GROUPS ARE SAME
// ============================================================

bool sameGroup(
    vector<pair<int, int>> group1,
    vector<pair<int, int>> group2)
{
    set<pair<int, int>> a(
        group1.begin(),
        group1.end()
    );

    set<pair<int, int>> b(
        group2.begin(),
        group2.end()
    );

    return a == b;
}


// ============================================================
// ADD GROUP WITHOUT DUPLICATES
// ============================================================

void addGroup(
    vector<vector<pair<int, int>>> &groups,
    vector<pair<int, int>> newGroup)
{
    for (auto group : groups)
    {
        if (sameGroup(group, newGroup))
            return;
    }

    groups.push_back(newGroup);
}


// ============================================================
// CREATE BOOLEAN TERM FROM A GROUP
// ============================================================

string makeTerm(vector<pair<int, int>> cells)
{
    vector<vector<int>> values;

    // Get ABCD values of every cell
    for (auto cell : cells)
    {
        values.push_back(
            getABCD(cell.first, cell.second)
        );
    }

    string variableName = "abcd";

    string term = "";

    // Compare A, B, C and D
    for (int variable = 0; variable < 4; variable++)
    {
        bool same = true;

        for (int i = 1; i < values.size(); i++)
        {
            if (values[i][variable] !=
                values[0][variable])
            {
                same = false;
                break;
            }
        }

        // If the variable is same in every cell
        if (same)
        {
            if (values[0][variable] == 1)
            {
                term += variableName[variable];
            }
            else
            {
                term += variableName[variable];
                term += "'";
            }
        }
    }

    return term;
}


// ============================================================
// GENERATE ALL POSSIBLE GROUPS
// ============================================================

vector<vector<pair<int, int>>> generateGroups(
    int kmap[4][4])
{
    vector<vector<pair<int, int>>> groups;


    // ========================================================
    // GROUP OF 16
    // ========================================================

    {
        vector<pair<int, int>> cells;

        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                cells.push_back({r, c});
            }
        }

        if (isValidGroup(kmap, cells))
            addGroup(groups, cells);
    }


    // ========================================================
    // GROUPS OF 8
    // ========================================================

    // 2 x 4
    for (int r = 0; r < 4; r++)
    {
        vector<pair<int, int>> cells;

        int r1 = r;
        int r2 = (r + 1) % 4;

        for (int c = 0; c < 4; c++)
        {
            cells.push_back({r1, c});
            cells.push_back({r2, c});
        }

        if (isValidGroup(kmap, cells))
            addGroup(groups, cells);
    }


    // 4 x 2
    for (int c = 0; c < 4; c++)
    {
        vector<pair<int, int>> cells;

        int c1 = c;
        int c2 = (c + 1) % 4;

        for (int r = 0; r < 4; r++)
        {
            cells.push_back({r, c1});
            cells.push_back({r, c2});
        }

        if (isValidGroup(kmap, cells))
            addGroup(groups, cells);
    }


    // ========================================================
    // GROUPS OF 4
    // ========================================================

    // 1 x 4
    for (int r = 0; r < 4; r++)
    {
        vector<pair<int, int>> cells;

        for (int c = 0; c < 4; c++)
        {
            cells.push_back({r, c});
        }

        if (isValidGroup(kmap, cells))
            addGroup(groups, cells);
    }


    // 4 x 1
    for (int c = 0; c < 4; c++)
    {
        vector<pair<int, int>> cells;

        for (int r = 0; r < 4; r++)
        {
            cells.push_back({r, c});
        }

        if (isValidGroup(kmap, cells))
            addGroup(groups, cells);
    }


    // 2 x 2
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            vector<pair<int, int>> cells;

            int r1 = r;
            int r2 = (r + 1) % 4;

            int c1 = c;
            int c2 = (c + 1) % 4;

            cells.push_back({r1, c1});
            cells.push_back({r1, c2});
            cells.push_back({r2, c1});
            cells.push_back({r2, c2});

            if (isValidGroup(kmap, cells))
                addGroup(groups, cells);
        }
    }


    // ========================================================
    // GROUPS OF 2
    // ========================================================

    // Horizontal pair
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            vector<pair<int, int>> cells;

            cells.push_back({r, c});
            cells.push_back({r, (c + 1) % 4});

            if (isValidGroup(kmap, cells))
                addGroup(groups, cells);
        }
    }


    // Vertical pair
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            vector<pair<int, int>> cells;

            cells.push_back({r, c});
            cells.push_back({(r + 1) % 4, c});

            if (isValidGroup(kmap, cells))
                addGroup(groups, cells);
        }
    }


    // ========================================================
    // SINGLE CELLS
    // ========================================================

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (kmap[r][c] == 1)
            {
                vector<pair<int, int>> cells;

                cells.push_back({r, c});

                addGroup(groups, cells);
            }
        }
    }


    return groups;
}


// ============================================================
// CHECK IF ONE GROUP IS INSIDE ANOTHER GROUP
// ============================================================

bool isSubset(
    vector<pair<int, int>> small,
    vector<pair<int, int>> large)
{
    set<pair<int, int>> A(
        small.begin(),
        small.end()
    );

    set<pair<int, int>> B(
        large.begin(),
        large.end()
    );

    for (auto cell : A)
    {
        if (B.find(cell) == B.end())
            return false;
    }

    return true;
}


// ============================================================
// KEEP ONLY LARGEST POSSIBLE GROUPS
// ============================================================

vector<vector<pair<int, int>>> getUsefulGroups(
    vector<vector<pair<int, int>>> groups)
{
    vector<vector<pair<int, int>>> usefulGroups;

    for (int i = 0; i < groups.size(); i++)
    {
        bool smallerGroup = false;

        for (int j = 0; j < groups.size(); j++)
        {
            if (i == j)
                continue;

            // If another group is larger
            // and contains this group
            if (groups[j].size() > groups[i].size())
            {
                if (isSubset(groups[i], groups[j]))
                {
                    smallerGroup = true;
                    break;
                }
            }
        }

        if (!smallerGroup)
        {
            usefulGroups.push_back(groups[i]);
        }
    }

    return usefulGroups;
}


// ============================================================
// GET ALL CELLS WHICH CONTAIN 1
// ============================================================

set<pair<int, int>> getAllOnes(int kmap[4][4])
{
    set<pair<int, int>> ones;

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (kmap[r][c] == 1)
            {
                ones.insert({r, c});
            }
        }
    }

    return ones;
}


// ============================================================
// FIND ALL POSSIBLE COVERS
// ============================================================

void findCovers(
    vector<vector<pair<int, int>>> &groups,
    int index,
    set<pair<int, int>> covered,
    set<pair<int, int>> target,
    vector<int> selected,
    vector<vector<int>> &solutions)
{
    // All 1s are covered
    if (covered == target)
    {
        solutions.push_back(selected);
        return;
    }

    if (index >= groups.size())
        return;


    // --------------------------------------------------------
    // TAKE CURRENT GROUP
    // --------------------------------------------------------

    set<pair<int, int>> newCovered = covered;

    for (auto cell : groups[index])
    {
        newCovered.insert(cell);
    }

    selected.push_back(index);

    findCovers(
        groups,
        index + 1,
        newCovered,
        target,
        selected,
        solutions
    );

    selected.pop_back();


    // --------------------------------------------------------
    // DON'T TAKE CURRENT GROUP
    // --------------------------------------------------------

    findCovers(
        groups,
        index + 1,
        covered,
        target,
        selected,
        solutions
    );
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    int kmap[4][4];


    // --------------------------------------------------------
    // READ INPUT FILE
    // --------------------------------------------------------

    string filename = "input.txt";

    if (!readKMap(filename, kmap))
    {
        return 0;
    }


    // --------------------------------------------------------
    // DISPLAY K-MAP
    // --------------------------------------------------------

    cout << "K-Map:\n\n";

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            cout << kmap[r][c] << " ";
        }

        cout << endl;
    }


    // --------------------------------------------------------
    // DISPLAY VARIABLE ORDER
    // --------------------------------------------------------

    cout << "\nColumn order (AB): ";
    cout << "00 01 11 10\n";

    cout << "Row order (CD):    ";
    cout << "00 01 11 10\n";


    // --------------------------------------------------------
    // GENERATE GROUPS
    // --------------------------------------------------------

    vector<vector<pair<int, int>>> groups =
        generateGroups(kmap);


    // Keep largest useful groups
    groups = getUsefulGroups(groups);


    // --------------------------------------------------------
    // GET ALL 1s
    // --------------------------------------------------------

    set<pair<int, int>> ones =
        getAllOnes(kmap);


    // If there are no 1s
    if (ones.empty())
    {
        cout << "\nBoolean Expression: 0\n";
        return 0;
    }


    // --------------------------------------------------------
    // FIND ALL COVERS
    // --------------------------------------------------------

    vector<vector<int>> solutions;

    findCovers(
        groups,
        0,
        {},
        ones,
        {},
        solutions
    );


    if (solutions.empty())
    {
        cout << "\nNo solution found.\n";
        return 0;
    }


    // --------------------------------------------------------
    // FIND MINIMUM NUMBER OF GROUPS
    // --------------------------------------------------------

    int minimumGroups =
        solutions[0].size();

    for (auto solution : solutions)
    {
        if (solution.size() < minimumGroups)
        {
            minimumGroups =
                solution.size();
        }
    }


    // --------------------------------------------------------
    // CREATE EXPRESSIONS
    // --------------------------------------------------------

    set<string> finalExpressions;


    for (auto solution : solutions)
    {
        // Only minimum-group solutions
        if (solution.size() != minimumGroups)
            continue;


        vector<string> terms;


        for (int groupIndex : solution)
        {
            string term =
                makeTerm(groups[groupIndex]);


            // Avoid duplicate terms
            if (find(
                    terms.begin(),
                    terms.end(),
                    term
                ) == terms.end())
            {
                terms.push_back(term);
            }
        }


        // Sort terms
        sort(
            terms.begin(),
            terms.end()
        );


        // Join using +
        string expression = "";

        for (int i = 0; i < terms.size(); i++)
        {
            if (i > 0)
            {
                expression += " + ";
            }

            expression += terms[i];
        }


        finalExpressions.insert(expression);
    }


    // --------------------------------------------------------
    // PRINT RESULT
    // --------------------------------------------------------

    cout << "\nAll possible minimized Boolean expressions:\n\n";


    int count = 1;

    for (string expression : finalExpressions)
    {
        cout << count << ". "
             << expression << endl;

        count++;
    }


    return 0;
}