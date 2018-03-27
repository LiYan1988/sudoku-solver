#include "main.hpp"

using namespace std;

int main()
{
//    int rosterSize = 2;
//    Dog roster[rosterSize];
//    roster[0] = Dog();
//    roster[0].setName("dog0");
//    roster[1] = Dog();
//    roster[1].setName("dog1");
//    roster[0].setLicenseNumber(0);
//    roster[1].setLicenseNumber(1);
//    printRoster(roster, rosterSize);

    Matrix puzzleMatrix;
    Sudoku p;
    puzzleMatrix = p.readSud("sudoku.sud");
    p.setPuzzle(puzzleMatrix);
    p.printPuzzle();
//    vector<int> c = p.findValidElements(puzzleMatrix, 1, 4);
//    for(int i=0; i<c.size(); i++) cout<<c[i]<<endl;
//    cout<<c.size()<<endl;
    cout<<endl;
    Matrix puzzleSolution;
    bool flag = false;
    puzzleSolution = p.solvePuzzle(puzzleMatrix, flag);
    printPuzzle(puzzleSolution);
    flag = p.checkSolution(puzzleSolution);
    string val = flag? "valid" : "invalid";
    cout<<"solution is "<<val<<endl;
    return 0;
}
