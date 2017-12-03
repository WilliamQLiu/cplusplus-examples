/*
Compile with: g++ -std=c++17 hello.cpp
Run with: ./a.out
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


// Functions - return type, then parameters with default values
int addNumbers(int firstNum, int secondNum=0) {
    int combinedValue = firstNum + secondNum;
    return combinedValue;
}
// can overload functions (same name, different attributes)
int addNumbers(int firstNum, int secondNum, int thirdNum) {
    return firstNum + secondNum + thirdNum;
}

// Pointers and Referencers,
// Use a pointer if value needs to be assigned a new address value
//   E.g. you don't want to initialize at declaration what the address value is
// Use Reference if the variable does not need to be reassigned again (since its only a memory address)

// Passing by Pointer *
void makeMeYoung(int* age) {
    cout << "I used to be " << *age << endl;
    *age = 21;
}

// Passing by Reference &
void actYourAge(int& age) {
    age = 35;
    cout << "Act your age: " << age << endl;
}

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
        //getline(cin, numberGuessed);  // get input from user
        numberGuessed = "4";  // cheating here
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

    // vectors (arrays, but their size can change)
    vector <int> lotteryNumVect(10);
    int lotteryNumArray[5] = {4, 13, 14, 24, 34};
    lotteryNumVect.insert(lotteryNumVect.begin(), lotteryNumArray, lotteryNumArray+3);
    lotteryNumVect.insert(lotteryNumVect.begin()+5, 44);
    cout << lotteryNumVect.at(5) << endl;
    lotteryNumVect.push_back(6);  // add data to the end of the vector
    cout << "Is empty: " << lotteryNumVect.empty() << endl;  // check if vector is empty

    // calling functions
    cout << "Calling Function: " << addNumbers(1) << endl;
    cout << "Calling Overloaded Function: " << addNumbers(2, 5, 6) << endl;  // calls the overloaded function

    // Write files
    string willQuote = "A day without sunshine is night!";
    ofstream writer1("willquote.txt");
    if (! writer1){
        cout << "Error opening file" << endl;
        return -1;
    } else {
        writer1 << willQuote << endl;
        writer1.close();
    }

    // opens a stream to append to whats there with ios:app
    // ios::binary : treats the file as binary
    // ios::in : open af ile to read input
    // ios::trunc : default
    // ios::out : open a file to write output
    ofstream writer2("willquote.txt", ios::app);
    if (! writer2) {
        cout << "Error opening file" << endl;
        return -1;
    } else {
        writer2 << "\n -Steve Martin" << endl;
        writer2.close();
    }

    // reading files
    char letter;
    ifstream reader1("willquote.txt");
    if(! reader1) {
        cout << "Error opening file" << endl;
        return -1;
    } else {
        for(int i=0; ! reader1.eof(); i++) {
            reader1.get(letter);
            cout << letter;
        }
        cout << endl;
        reader1.close();
    }

    // try-catch
    int number = 0;
    try{
        if(number != 0) {
            cout << 2/number << endl;
        } else throw(number);  //a throw will look for a catch
    }
    catch(int number){
        cout << number << " is not valid" << endl;
    }

    // Pointers (*) and Reference (&)
    int someAge = 33;
    char someGrade = 'A';
    cout << "Size of int " << sizeof(someAge) << endl;
    cout << "Size of char " << sizeof(someGrade) << endl;
    cout << "someAge is located at " << &someAge << endl;  // 0x..... memory address
    // Remeber that functions can be either passed by value or passed by reference
    int* agePtr = &someAge;  // create pointer that references that memory address
    cout << "Address of pointer " << agePtr << endl;  // 0x....
    cout << "Data at memory address " << *agePtr << endl;

    // More about Pointers
    int someNums[5] = {4, 13, 14, 24, 34};
    int* numArrayPtr = someNums;
    cout << "Address " << numArrayPtr << " Value " << *numArrayPtr << endl;
    numArrayPtr++;
    cout << "Address " << numArrayPtr << " Value " << *numArrayPtr << endl;
    makeMeYoung(&someAge);  // pass someAge by reference
    cout << "I'm " << someAge << " years old now " << endl;
    int& ageRef = someAge;  // & to reference
    cout << "Age: " << someAge << endl;
    ageRef++;
    cout << "Age: " << someAge << endl;
    actYourAge(ageRef);
    cout << "Age after pass by reference function: " << someAge << endl;

    return 0;
}
