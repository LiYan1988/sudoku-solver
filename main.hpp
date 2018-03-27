#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<vector<int> > Matrix;

class Dog
{
    string name;
    int licenseNumber;

public:
    Dog();
    void setName(string nameIn);
    void setLicenseNumber(int licenseNumberIn);
    string getName();
    int getLicenseNumber();
    void printInfo();
};

Dog::Dog()
{
    name = "unknown";
}

void Dog::setName(string nameIn)
{
    name = nameIn;
}

void Dog::setLicenseNumber(int licenseNumberIn)
{
    licenseNumber = licenseNumberIn;
}

string Dog::getName()
{
    return name;
}

int Dog::getLicenseNumber()
{
    return licenseNumber;
}

void Dog::printInfo()
{
    cout<<name<<" "<<licenseNumber;
}

void printRoster(Dog * roster, int size)
{
    for(int i=0; i<size; i++)
    {
//        Dog dog = *(roster + i);
        roster->printInfo();
        cout<<endl;
    }
}

class Sudoku
{
    const int cellSize = 3;
    const int totalSize = 9;
    Matrix puzzle;
    bool flagValid;

public:
    Sudoku();
    Matrix readSud(char * fileName);
    void setPuzzle(Matrix puzzleIn);
    vector<int> findValidElements(Matrix puzzleIn, int rowIndex, int columnIndex);
    void printPuzzle(void);
    Matrix solvePuzzle(Matrix puzzleIn, bool & flagValid);
    bool checkSolution(Matrix puzzleIn);
};

Sudoku::Sudoku()
{
    cout<<"A puzzle created."<<endl;
}

void Sudoku::setPuzzle(Matrix puzzleIn)
{
    for(int i=0; i<totalSize; i++)
    {
        vector<int> rowTmp;
        for(int j=0; j<totalSize; j++)
        {
            rowTmp.push_back(puzzleIn[i][j]);
        }
        puzzle.push_back(rowTmp);
    }
}

Matrix Sudoku::solvePuzzle(Matrix puzzleIn, bool & flagValid)
{
    // copy the input puzzle
    Matrix puzzleOut = puzzleIn;

    // search for the first blank element
    int columnIndexNext = 0;
    int rowIndexNext = 0;
    bool flag_found = false;
    for(int i=0; i<totalSize; i++)
    {
        for(int j=0; j<totalSize; j++)
        {
            if (puzzleOut[i][j]==0)
            {
                columnIndexNext = j;
                rowIndexNext = i;
                flag_found = true;
                break;
            }
        }
        if (flag_found)
            break;
    }

    // if no blank element, check if the puzzle is viable
    if (!flag_found)
    {
        bool flagValidCheck = checkSolution(puzzleIn);
        if (flagValidCheck) flagValid = true;
        else flagValid = false;
//        cout<<"No blank element found. flag = "<<flagValid<<endl;
        return puzzleIn;
    }

    // if there is blank element, find its candidates
    vector<int> candidate =findValidElements(puzzleOut, rowIndexNext, columnIndexNext);
    // if no candidate
    if(candidate.size()==0)
    {
        flagValid = false;
//        cout<<"Blank element found at ("<<rowIndexNext<<", "<<columnIndexNext<<"), but candidate list is empty. flag = "<<flagValid<<endl;
        return puzzleIn;
    }

    // if there are candidates
    bool flagValidNext = false;
    while((candidate.size()!=0)&!flagValidNext)
    {
        // while there is still candidate and no valid solution found
        // get one candidate and try
        puzzleOut[rowIndexNext][columnIndexNext] = candidate.back();
        candidate.pop_back();
        Matrix puzzleTmp = solvePuzzle(puzzleOut, flagValidNext);
        if(flagValidNext)
        {
            // if find valid solution
            flagValidNext = true;
            flagValid = true;
            return puzzleTmp;
        }
    }

    flagValid = false;
    return puzzleIn;
}

bool Sudoku::checkSolution(Matrix puzzleIn)
{
    // all possible elements
    vector <int> allElements;
    for (int i=0; i<totalSize; i++) allElements.push_back(i+1);

    // rows
    for (int i=0; i<totalSize; i++)
    {
        vector <int> rowTmp = allElements;
        for (int j=0; j<totalSize; j++)
        {
            int tmp = puzzleIn[i][j];
            if(find(rowTmp.begin(), rowTmp.end(), tmp) != rowTmp.end())
                rowTmp.erase(remove(rowTmp.begin(), rowTmp.end(), tmp), rowTmp.end());
        }
        if(rowTmp.size()!=0) return false;
    }

    // columns
    for (int j=0; j<totalSize; j++)
    {
        vector <int> columnTmp = allElements;
        for (int i=0; i<totalSize; i++)
        {
            int tmp = puzzleIn[i][j];
            if(find(columnTmp.begin(), columnTmp.end(), tmp) != columnTmp.end())
                columnTmp.erase(remove(columnTmp.begin(), columnTmp.end(), tmp), columnTmp.end());
        }
        if(columnTmp.size()!=0) return false;
    }

    // cells
    for (int k=0; k<totalSize; k++)
    {
        vector <int> cellTmp = allElements;
        for (int i=0; i<cellSize; i++)
        {
            for (int j=0; j<cellSize; j++)
            {
                int cellRow = k / cellSize;
                int cellColumn = k % cellSize;
                int tmp = puzzleIn[cellRow * cellSize + i][cellColumn * cellSize + j];
                if(find(cellTmp.begin(), cellTmp.end(), tmp) != cellTmp.end())
                    cellTmp.erase(remove(cellTmp.begin(), cellTmp.end(), tmp), cellTmp.end());
            }
        }
        if (cellTmp.size()!=0) return false;
    }

    return true;
}

vector<int> Sudoku::findValidElements(Matrix puzzleIn, int rowIndex, int columnIndex)
{

    vector<int> validElements;

    // cell index
    int cellRowIndex = rowIndex/cellSize;
    int cellColumnIndex = columnIndex/cellSize;
    // first populate the list with all possible numbers
    for(int j=0; j<totalSize; j++)
    {
        validElements.push_back(j+1);
    }
    // for row
    for(int j=0; j<totalSize; j++)
    {
        if (j==columnIndex) continue;
        int tmp = puzzleIn[rowIndex][j];
//        cout<<"column "<<j<<" equals "<<tmp<<endl;
        if(find(validElements.begin(), validElements.end(), tmp) != validElements.end())
            validElements.erase(remove(validElements.begin(), validElements.end(), tmp), validElements.end());
    }
//    for(int i=0; i<validElements.size(); i++) cout<<validElements[i]<<", ";
//    cout<<endl;
    // for column
    for(int j=0; j<totalSize; j++)
    {
        if (j==columnIndex) continue;
        int tmp = puzzleIn[j][columnIndex];
//        cout<<"row "<<j<<" equals "<<tmp<<endl;
        if(find(validElements.begin(), validElements.end(), tmp) != validElements.end())
            validElements.erase(remove(validElements.begin(), validElements.end(), tmp), validElements.end());
    }
//    for(int i=0; i<validElements.size(); i++) cout<<validElements[i]<<", ";
//    cout<<endl;
    // cell
    for(int i=0; i<cellSize; i++)
    {
        for(int j=0; j<cellSize; j++)
        {
            int rowIndexTmp = cellRowIndex * cellSize + i;
            int columnIndexTmp = cellColumnIndex * cellSize + j;
            if ((rowIndexTmp==rowIndex)&(columnIndexTmp==columnIndex)) continue;
            int tmp = puzzleIn[rowIndexTmp][columnIndexTmp];
//            cout<<"row "<<rowIndexTmp<<", column "<<columnIndexTmp<<" equals "<<tmp<<endl;
            if(find(validElements.begin(), validElements.end(), tmp) != validElements.end())
                validElements.erase(remove(validElements.begin(), validElements.end(), tmp), validElements.end());
        }
    }
//    for(int i=0; i<validElements.size(); i++) cout<<validElements[i]<<", ";
//    cout<<endl;
    // remove itself
    if(puzzleIn[rowIndex][columnIndex]!=0) validElements.clear();

    return validElements;
}

Matrix Sudoku::readSud(char* fileName)
{
    ifstream myfile(fileName);
    string line;
    Matrix puzzleOut;
    vector<int> rowTmp;
    if(myfile.is_open())
    {
        int n = 0;
        while(getline(myfile, line))
        {
            if(n++==2)
            {
                cout<<line<<endl;
                int rowIndex = 0;
                int columnIndex = 0;
                for(size_t i=0; i<line.length(); i++)
                {
                    if(line[i]>='1' & line[i]<='9')
                    {
                        cout<<(int)(line[i] - '0')<<endl;
                        rowTmp.push_back((int)(line[i] - '0'));
                    }
                    else if(line[i]=='s')
                    {
                        rowTmp.push_back(0);
                    }

                    if (rowTmp.size()==totalSize)
                    {
                        puzzleOut.push_back(rowTmp);
                        rowTmp.clear();
                    }
                    if (puzzleOut.size()==totalSize)
                        break;
                }
            }
        }
        myfile.close();
    }
    return puzzleOut;
}

void Sudoku::printPuzzle(void)
{
    for(int i=0; i<totalSize; i++)
    {
        for(int j=0; j<totalSize; j++)
        {
            cout<<puzzle[i][j]<<", ";
        }
        cout<<endl;
    }
}

void printPuzzle(Matrix puzzleIn)
{
    for(int i=0; i<puzzleIn.size(); i++)
    {
        for(int j=0; j<puzzleIn[i].size(); j++)
        {
            cout<<puzzleIn[i][j]<<", ";
        }
        cout<<endl;
    }
}
