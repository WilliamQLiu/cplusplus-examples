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

    return 0;
}
