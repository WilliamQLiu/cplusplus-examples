/*
Compile with: g++ -std=c++17 hello.cpp
Run with: ./a.out
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(){

    cout << "Hello world!" << endl;

    // variable assignments
    const double PI = 3.1415926535;
    char myGrade = 'A';  // uses one byte of memory
    bool isIntoxicated = false;  // same as 0
    int myAge = 33;
    float favNum = 3.141592;

    cout << "Size of int is: " << sizeof(myAge) << endl;
    myAge += 5;

    // if-else statement
    if((myAge >= 1) && (myAge< 16)){
        cout << "You can't drive" << endl;
    } else if(isIntoxicated) {
        cout << "You can't drive you drunk" << endl;
    } else {
        cout << "You can drive" << endl;
    }

    // switch-statement
    int greetingOption = 2;
    switch(greetingOption){
        case 1:
            cout << "Hello" << endl;
            break;
        case 2:
            cout << "Bonjour" << endl;
            break;
        default:
            cout << "Halo" << endl;
            break;
    }

    // variable = (condition) ? true : false
    int largestNum = (5>2) ? 5: 2;
    cout << "Largest Number is: " << largestNum << endl;

    // one-dimensional and two-dimensional arrays
    int myFavNums[5];  // one-dimensional array
    int badNums[5] = {4, 13, 14, 24, 34};  // starts with index of 0
    cout << "Bad Number 1: " << badNums[0] << endl;
    char myName[5][5] = {{'W', 'i', 'l', 'l'},  // two-dimensional array
                         {'L', 'i', 'u'}};
    cout << "My Name at: " << myName[0][0] << endl;  // 'W'
    myName[0][3] = 'k';

    // for-loops
    for(int i=0; i<10; i++){
        cout << i << endl;
    }

    for(int j=0; j<5; j++){
        for(int k=0; k<5; k++){
            cout << myName[j][k];
        }
    }
    cout << endl;

    // while-loops (used when you don't know when loop will end)
    int randNum = (rand() % 100) + 1;  // create random 1-100
    while(randNum != 100){
        cout << randNum << ",";
        randNum = (rand() % 100) + 1;
    }
    cout << endl;

    // do-while loop
    string numberGuessed;
    int intNumberGuessed = 0;
    do {
        cout << "Guess between 1 and 10: ";
        getline(cin, numberGuessed);  // get input from user
        intNumberGuessed = stoi(numberGuessed);  // string to integer, use stod for double

    } while(intNumberGuessed !=4);
    cout << "You Win" << endl;

    // string functions
    string whatName;
    cout << "Is string empty: " << whatName.empty() << endl;
    cout << "Size of string " << whatName.size() << endl;
    string dogName = "dog";
    string catName = "cat";
    cout << dogName.compare(catName) << endl;  // get results of -1, 0, 1 if less than, equal, greater than
    string subName = dogName.assign(catName, 0, 2);  // substring name 'ca'
    cout << subName << endl;
    int findString = dogName.find("og", 0);  // start at index 0
    cout << "Index from find is " << findString << endl;
    dogName.insert(2, "_scooby");
    cout << "Dog Name is: " << dogName << endl;
    dogName.erase(1,2);
    cout << "Dog Name is: " << dogName << endl;
    dogName.replace(3, 4, "XYZ");
    cout << "Dog Name is: " << dogName << endl;

    return 0;
}
