#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int n;
int rows;
int cols;

vector<vector<int> > kmap;


// ============================================================
// GRAY CODE
// ============================================================

int gray(int x)
{
    return x ^ (x >> 1);
}


// ============================================================
// READ INPUT FILE
// ============================================================

bool readKMap()
{
    ifstream file("input.txt");

    if(!file)
    {
        cout << "Cannot open input.txt" << endl;
        return false;
    }

    file >> n;

    int rowBits = n / 2;
    int colBits = n - rowBits;

    rows = 1 << rowBits;
    cols = 1 << colBits;

    kmap.resize(rows);

    for(int i = 0; i < rows; i++)
    {
        kmap[i].resize(cols);

        for(int j = 0; j < cols; j++)
        {
            file >> kmap[i][j];
        }
    }

    file.close();

    return true;
}


// ============================================================
// GET MINTERM NUMBER
// ============================================================

int getMinterm(int r, int c)
{
    int rowBits = n / 2;
    int colBits = n - rowBits;

    int rvalue = gray(r);
    int cvalue = gray(c);

    return (rvalue << colBits) | cvalue;
}


// ============================================================
// CHECK WHETHER A MINTERM IS IN K-MAP AS 1
// ============================================================

bool isOne(int minterm)
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            if(getMinterm(r,c) == minterm)
            {
                if(kmap[r][c] == 1)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


// ============================================================
// GET ALL 1 MINTERMS
// ============================================================

void getOnes(vector<int> &ones)
{
    for(int i = 0; i < (1 << n); i++)
    {
        if(isOne(i))
        {
            ones.push_back(i);
        }
    }
}


// ============================================================
// CHECK WHETHER GROUP IS SAME
// ============================================================

bool sameGroup(
    vector<int> a,
    vector<int> b)
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(int i = 0; i < a.size(); i++)
    {
        bool found = false;

        for(int j = 0; j < b.size(); j++)
        {
            if(a[i] == b[j])
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            return false;
        }
    }

    return true;
}


// ============================================================
// CHECK GROUP IS VALID
// ============================================================

bool validGroup(vector<int> group)
{
    for(int i = 0; i < group.size(); i++)
    {
        if(!isOne(group[i]))
        {
            return false;
        }
    }

    return true;
}


// ============================================================
// ADD GROUP
// ============================================================

void addGroup(
    vector<vector<int> > &groups,
    vector<int> group)
{
    if(!validGroup(group))
    {
        return;
    }

    for(int i = 0; i < groups.size(); i++)
    {
        if(sameGroup(groups[i], group))
        {
            return;
        }
    }

    groups.push_back(group);
}


// ============================================================
// GENERATE ALL POSSIBLE K-MAP GROUPS
//
// 0 = fixed 0
// 1 = fixed 1
// 2 = changing
//
// There are 3^n possible patterns.
// ============================================================

void generateGroups(
    vector<vector<int> > &groups)
{
    int numberOfPatterns = 1;

    for(int i = 0; i < n; i++)
    {
        numberOfPatterns *= 3;
    }

    vector<int> pattern(n);


    for(int p = 0;
        p < numberOfPatterns;
        p++)
    {
        int temp = p;


        // Create pattern

        for(int i = 0; i < n; i++)
        {
            pattern[i] = temp % 3;

            temp /= 3;
        }


        // Find minterms matching pattern

        vector<int> group;


        for(int minterm = 0;
            minterm < (1 << n);
            minterm++)
        {
            bool matches = true;


            for(int bit = 0;
                bit < n;
                bit++)
            {
                int value =
                    (minterm >> (n - 1 - bit)) & 1;


                if(pattern[bit] != 2 &&
                   pattern[bit] != value)
                {
                    matches = false;
                    break;
                }
            }


            if(matches)
            {
                group.push_back(minterm);
            }
        }


        // Add only groups containing 1s

        if(group.size() > 0)
        {
            addGroup(groups, group);
        }
    }
}


// ============================================================
// NUMBER OF LITERALS
// ============================================================

int literals(vector<int> group)
{
    int size = group.size();

    int result = n;

    while(size > 1)
    {
        size /= 2;
        result--;
    }

    return result;
}


// ============================================================
// CHECK WHETHER GROUP CONTAINS MINTERM
// ============================================================

bool contains(
    vector<int> group,
    int minterm)
{
    for(int i = 0; i < group.size(); i++)
    {
        if(group[i] == minterm)
        {
            return true;
        }
    }

    return false;
}


// ============================================================
// GET PATTERN OF GROUP
// ============================================================

void getPattern(
    vector<int> group,
    vector<int> &pattern)
{
    pattern.resize(n);

    int first = group[0];


    for(int i = 0; i < n; i++)
    {
        pattern[i] =
            (first >> (n - 1 - i)) & 1;
    }


    for(int i = 1;
        i < group.size();
        i++)
    {
        int current = group[i];


        for(int j = 0;
            j < n;
            j++)
        {
            int value =
                (current >> (n - 1 - j)) & 1;


            if(pattern[j] != value)
            {
                pattern[j] = -1;
            }
        }
    }
}


// ============================================================
// MAKE TERM
// ============================================================

string makeTerm(vector<int> group)
{
    vector<int> pattern;

    getPattern(group, pattern);

    string term = "";


    for(int i = 0; i < n; i++)
    {
        if(pattern[i] == -1)
        {
            continue;
        }


        char variable = 'A' + i;

        term += variable;


        if(pattern[i] == 0)
        {
            term += "'";
        }
    }


    if(term == "")
    {
        term = "1";
    }


    return term;
}


// ============================================================
// ALL 1s COVERED?
// ============================================================

bool allCovered(
    vector<int> ones,
    vector<vector<int> > selected)
{
    for(int i = 0;
        i < ones.size();
        i++)
    {
        bool covered = false;


        for(int j = 0;
            j < selected.size();
            j++)
        {
            if(contains(
                   selected[j],
                   ones[i]))
            {
                covered = true;
                break;
            }
        }


        if(!covered)
        {
            return false;
        }
    }


    return true;
}


// ============================================================
// MAKE EXPRESSION
// ============================================================

string makeExpression(
    vector<vector<int> > selected)
{
    string expression = "";


    for(int i = 0;
        i < selected.size();
        i++)
    {
        if(i != 0)
        {
            expression += " + ";
        }


        expression +=
            makeTerm(selected[i]);
    }


    return expression;
}


// ============================================================
// CHECK DUPLICATE ANSWER
// ============================================================

bool exists(
    vector<string> answers,
    string expression)
{
    for(int i = 0;
        i < answers.size();
        i++)
    {
        if(answers[i] == expression)
        {
            return true;
        }
    }

    return false;
}


// ============================================================
// FIND ALL MINIMUM SOLUTIONS
// ============================================================

void findSolutions(
    vector<vector<int> > &groups,
    vector<int> &ones,
    vector<vector<int> > selected,
    int start,
    int currentCost,
    int &bestCost,
    vector<string> &answers)
{
    // --------------------------------------------------------
    // All 1s covered
    // --------------------------------------------------------

    if(allCovered(ones, selected))
    {
        string expression =
            makeExpression(selected);


        // Better solution

        if(currentCost < bestCost)
        {
            bestCost = currentCost;

            answers.clear();

            answers.push_back(expression);
        }


        // Another minimum solution

        else if(currentCost == bestCost)
        {
            if(!exists(
                   answers,
                   expression))
            {
                answers.push_back(expression);
            }
        }


        return;
    }


    // --------------------------------------------------------
    // Stop if already worse
    // --------------------------------------------------------

    if(currentCost > bestCost)
    {
        return;
    }


    // --------------------------------------------------------
    // Try all groups
    // --------------------------------------------------------

    for(int i = start;
        i < groups.size();
        i++)
    {
        int cost =
            literals(groups[i]);


        // ----------------------------------------------------
        // Check whether group covers
        // a new 1
        // ----------------------------------------------------

        bool useful = false;


        for(int j = 0;
            j < ones.size();
            j++)
        {
            if(contains(
                   groups[i],
                   ones[j]))
            {
                bool alreadyCovered = false;


                for(int k = 0;
                    k < selected.size();
                    k++)
                {
                    if(contains(
                           selected[k],
                           ones[j]))
                    {
                        alreadyCovered = true;
                        break;
                    }
                }


                if(!alreadyCovered)
                {
                    useful = true;
                    break;
                }
            }
        }


        if(!useful)
        {
            continue;
        }


        // ----------------------------------------------------
        // Select group
        // ----------------------------------------------------

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


        // ----------------------------------------------------
        // Remove group
        // ----------------------------------------------------

        selected.pop_back();
    }
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    // Read input file

    if(!readKMap())
    {
        return 0;
    }


    // Get all 1s

    vector<int> ones;

    getOnes(ones);


    // No 1s

    if(ones.size() == 0)
    {
        cout << "0" << endl;

        return 0;
    }


    // All cells are 1

    if(ones.size() == (1 << n))
    {
        cout << "1" << endl;

        return 0;
    }


    // Generate groups

    vector<vector<int> > groups;

    generateGroups(groups);


    // Find all minimum solutions

    vector<vector<int> > selected;

    vector<string> answers;


    int bestCost = 1000000;


    findSolutions(
        groups,
        ones,
        selected,
        0,
        0,
        bestCost,
        answers
    );


    // --------------------------------------------------------
    // PRINT ALL MINIMUM EXPRESSIONS
    // --------------------------------------------------------

    for(int i = 0;
        i < answers.size();
        i++)
    {
        cout << answers[i] << endl;
    }


    return 0;
}
