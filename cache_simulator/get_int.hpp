/*
Beau Albritton

Gets a valid integer input from user. helper function for main.cpp

I built this last semester in Advanced Programming. This is basically
fancy input validation with C++ quirks. Just importing this rather than 
reinventing the wheel (when i dont need to)

All code is authored by me!
*/      
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <string>

using std::string, std::stringstream, std::cout, std::cin;

bool isPowerOfTwo(int n) {
    if (n <= 0){
         return false;
    }
    while (n % 2 == 0) {
        n /= 2;
    }
    return n == 1;
}

int get_int_from_user(bool checkIfPowerOfTwo) {
    /* boy oh boy.. was stuck on this for a while
     *
     *  This method uses the stringstream class to read input strings
     *  and convert into the appropriate integer type.
     *
     */

    string integerInput;
    int input;
    stringstream stringStream;
    bool validated = false;

    /*
     * While loop checking if "validated"
     * This boolean represents whether or not a valid integer has been
     * reached. This will loop until one is input.
     */
    while(!validated) {
        cout << "Enter a number:";
        getline(cin,integerInput);
        //assigning string input to the string stream
        stringStream << integerInput;

        //reading from the string for proper input. stringStream >> input will return
        //false if there is an incorrect input for type int
        if(stringStream >>input) {
            //using this char to see any additional characters appended
            //to an input, i.e. (123 abc) for example output in readme.md
            char overflow_test;

            //then reading from stringstream to see if those appended characters exist
            if(stringStream>>overflow_test) {
                cout << "Invalid input. ";
            }
            //else, then there is no problem with the input string, therefore validated
            else{
                validated = true;
                //Now checking if its a power of two based on the flag
                if(checkIfPowerOfTwo)
                {
                    if(!isPowerOfTwo(input))
                    {
                        cout << "Not a power of two.";
                        validated = false;
                    }
                }
            }


            
        }
        //Do not validate if the given input is empty.
        else if (integerInput.empty())
            cout<<"No input. ";
        //if stringstream>>input is false, this will loop back
        else
            cout<<"Invalid input.";

        //stringStream.clear() wouldn't work on its own. So I had to use
        //both of these statements. Found this solution on stack overflow
        stringStream.clear();
        stringStream.str("");
    }
    return input;
}