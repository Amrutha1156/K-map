#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int kmap[4][4];

int gray[4][2] =
{
    {0,0},
    {0,1},
    {1,1},
    {1,0}
};

// ------------------------------------------------------------
// Read the K-map from input.txt
// ------------------------------------------------------------

void readKMap()
{
    ifstream file("input.txt");

    if (!file)
    {
        cout << "File not found!" << endl;
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            file >> kmap[i][j];
        }
    }

    file.close();
}

// ------------------------------------------------------------
// Check whether a group is valid
// ------------------------------------------------------------

bool validGroup(vector<pair<int,int> > group)
{
    for (int i = 0; i < group.size(); i++)
    {
        int r = group[i].first;
        int c = group[i].second;

        if (kmap[r][c] != 1)
            return false;
    }

    return true;
}

// ------------------------------------------------------------
// Check whether this group is already present
// ------------------------------------------------------------

bool sameGroup(vector<pair<int,int> > a,
               vector<pair<int,int> > b)
{
    if (a.size() != b.size())
        return false;

    for (int i = 0; i < a.size(); i++)
    {
        bool found = false;

        for (int j = 0; j < b.size(); j++)
        {
            if (a[i].first == b[j].first &&
                a[i].second == b[j].second)
            {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

// ------------------------------------------------------------
// Add group if it is not already there
// ------------------------------------------------------------

void addGroup(vector<vector<pair<int,int> > > &groups,
              vector<pair<int,int> > group)
{
    if (!validGroup(group))
        return;

    for (int i = 0; i < groups.size(); i++)
    {
        if (sameGroup(groups[i], group))
            return;
    }

    groups.push_back(group);
}

// ------------------------------------------------------------
// Generate all possible K-map groups
// ------------------------------------------------------------

void generateGroups(vector<vector<pair<int,int> > > &groups)
{
    // --------------------------------------------------------
    // 16 cells
    // --------------------------------------------------------

    vector<pair<int,int> > group;

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            group.push_back({r,c});
        }
    }

    addGroup(groups, group);

    // --------------------------------------------------------
    // Groups of 8 cells
    // --------------------------------------------------------

    // 2 rows x 4 columns
    for (int r = 0; r < 4; r += 2)
    {
        group.clear();

        for (int i = 0; i < 2; i++)
        {
            for (int c = 0; c < 4; c++)
            {
                group.push_back({(r+i)%4,c});
            }
        }

        addGroup(groups, group);
    }

    // 4 rows x 2 columns
    for (int c = 0; c < 4; c += 2)
    {
        group.clear();

        for (int r = 0; r < 4; r++)
        {
            for (int j = 0; j < 2; j++)
            {
                group.push_back({r,(c+j)%4});
            }
        }

        addGroup(groups, group);
    }

    // --------------------------------------------------------
    // Groups of 4 cells
    // --------------------------------------------------------

    // 1 row x 4 columns
    for (int r = 0; r < 4; r++)
    {
        group.clear();

        for (int c = 0; c < 4; c++)
        {
            group.push_back({r,c});
        }

        addGroup(groups, group);
    }

    // 4 rows x 1 column
    for (int c = 0; c < 4; c++)
    {
        group.clear();

        for (int r = 0; r < 4; r++)
        {
            group.push_back({r,c});
        }

        addGroup(groups, group);
    }

    // 2 rows x 2 columns
    for (int r = 0; r < 4; r++)
    {
        group.clear();

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                group.push_back({
                    (r+i)%4,
                    j
                });
            }
        }

        addGroup(groups, group);

        group.clear();

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                group.push_back({
                    (r+i)%4,
                    (j+2)%4
                });
            }
        }

        addGroup(groups, group);
    }

    // --------------------------------------------------------
    // Groups of 2 cells
    // --------------------------------------------------------

    // Horizontal pairs
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            group.clear();

            group.push_back({r,c});
            group.push_back({r,(c+1)%4});

            addGroup(groups, group);
        }
    }

    // Vertical pairs
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            group.clear();

            group.push_back({r,c});
            group.push_back({(r+1)%4,c});

            addGroup(groups, group);
        }
    }

    // --------------------------------------------------------
    // Groups of 1 cell
    // --------------------------------------------------------

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            group.clear();

            group.push_back({r,c});

            addGroup(groups, group);
        }
    }
}

// ------------------------------------------------------------
// Get A B C D values of a cell
// Columns = AB
// Rows = CD
// ------------------------------------------------------------

void getABCD(int row, int col, int value[])
{
    value[0] = gray[col][0];   // A
    value[1] = gray[col][1];   // B
    value[2] = gray[row][0];   // C
    value[3] = gray[row][1];   // D
}

// ------------------------------------------------------------
// Convert a group into a Boolean term
// ------------------------------------------------------------

string makeTerm(vector<pair<int,int> > group)
{
    int first[4];

    getABCD(group[0].first,
            group[0].second,
            first);

    bool same[4];

    for (int i = 0; i < 4; i++)
        same[i] = true;

    for (int i = 1; i < group.size(); i++)
    {
        int current[4];

        getABCD(group[i].first,
                group[i].second,
                current);

        for (int j = 0; j < 4; j++)
        {
            if (first[j] != current[j])
                same[j] = false;
        }
    }

    string term = "";

    char variables[4] = {'a','b','c','d'};

    for (int i = 0; i < 4; i++)
    {
        if (same[i])
        {
            term += variables[i];

            if (first[i] == 0)
                term += "'";
        }
    }

    return term;
}

// ------------------------------------------------------------
// Number of literals in a group
//
// 1 cell  -> 4 literals
// 2 cells -> 3 literals
// 4 cells -> 2 literals
// 8 cells -> 1 literal
// 16 cells -> 0 literals
// ------------------------------------------------------------

int literals(vector<pair<int,int> > group)
{
    if (group.size() == 1)
        return 4;

    if (group.size() == 2)
        return 3;

    if (group.size() == 4)
        return 2;

    if (group.size() == 8)
        return 1;

    return 0;
}

// ------------------------------------------------------------
// Check whether all 1s are covered
// ------------------------------------------------------------

bool allCovered(vector<pair<int,int> > ones,
                vector<vector<pair<int,int> > > selected)
{
    for (int i = 0; i < ones.size(); i++)
    {
        bool covered = false;

        for (int j = 0; j < selected.size(); j++)
        {
            for (int k = 0; k < selected[j].size(); k++)
            {
                if (ones[i].first == selected[j][k].first &&
                    ones[i].second == selected[j][k].second)
                {
                    covered = true;
                    break;
                }
            }

            if (covered)
                break;
        }

        if (!covered)
            return false;
    }

    return true;
}

// ------------------------------------------------------------
// Check whether a group covers a particular cell
// ------------------------------------------------------------

bool contains(vector<pair<int,int> > group,
              pair<int,int> cell)
{
    for (int i = 0; i < group.size(); i++)
    {
        if (group[i].first == cell.first &&
            group[i].second == cell.second)
        {
            return true;
        }
    }

    return false;
}

// ------------------------------------------------------------
// Make expression from selected groups
// ------------------------------------------------------------

string makeExpression(vector<vector<pair<int,int> > > selected)
{
    string expression = "";

    for (int i = 0; i < selected.size(); i++)
    {
        string term = makeTerm(selected[i]);

        if (term == "")
            term = "1";

        if (i != 0)
            expression += " + ";

        expression += term;
    }

    return expression;
}

// ------------------------------------------------------------
// Find all minimum solutions
// ------------------------------------------------------------

void findSolutions(
    vector<vector<pair<int,int> > > &groups,
    vector<pair<int,int> > &ones,
    vector<vector<pair<int,int> > > selected,
    int start,
    int currentCost,
    int &bestCost,
    vector<string> &answers)
{
    // If everything is covered
    if (allCovered(ones, selected))
    {
        string expression = makeExpression(selected);

        if (currentCost < bestCost)
        {
            bestCost = currentCost;

            answers.clear();
            answers.push_back(expression);
        }
        else if (currentCost == bestCost)
        {
            bool already = false;

            for (int i = 0; i < answers.size(); i++)
            {
                if (answers[i] == expression)
                {
                    already = true;
                    break;
                }
            }

            if (!already)
                answers.push_back(expression);
        }

        return;
    }

    // If already worse than best solution
    if (currentCost >= bestCost)
        return;

    // Try every possible group
    for (int i = start; i < groups.size(); i++)
    {
        int cost = literals(groups[i]);

        // If this group gives no benefit, skip
        bool useful = false;

        for (int j = 0; j < ones.size(); j++)
        {
            if (contains(groups[i], ones[j]))
            {
                bool alreadyCovered = false;

                for (int k = 0; k < selected.size(); k++)
                {
                    if (contains(selected[k], ones[j]))
                    {
                        alreadyCovered = true;
                        break;
                    }
                }

                if (!alreadyCovered)
                {
                    useful = true;
                    break;
                }
            }
        }

        if (!useful)
            continue;

        selected.push_back(groups[i]);

        findSolutions(
            groups,
            ones,
            selected,
            i + 1,
            currentCost + cost,
            bestCost,
            answers
        );

        selected.pop_back();
    }
}

// ------------------------------------------------------------
// Get all cells containing 1
// ------------------------------------------------------------

void getOnes(vector<pair<int,int> > &ones)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (kmap[r][c] == 1)
            {
                ones.push_back({r,c});
            }
        }
    }
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{
    readKMap();

    vector<pair<int,int> > ones;

    getOnes(ones);

    if (ones.size() == 0)
    {
        cout << "F = 0" << endl;
        return 0;
    }

    vector<vector<pair<int,int> > > groups;

    generateGroups(groups);

    vector<vector<pair<int,int> > > selected;

    vector<string> answers;

    int bestCost = 1000;

    findSolutions(
        groups,
        ones,
        selected,
        0,
        0,
        bestCost,
        answers
    );

    cout << endl;
    cout << "All possible minimum Boolean expressions:" << endl;
    cout << endl;

    for (int i = 0; i < answers.size(); i++)
    {
        cout << "F = " << answers[i] << endl;
    }

    cout << endl;
    cout << "Minimum number of literals = "
         << bestCost << endl;

    return 0;
}
