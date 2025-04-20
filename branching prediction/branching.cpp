/**
 * Beau Albritton
 * branching.cpp
 * 
 * This source file contains the implementation for Static, One Bit, Two Bit
 * and Three Bit branch prediction using polymorphism in C++
 */


//includes 
#include <sstream>
#include <string> 
#include <fstream>
#include <iostream>
#include <vector>
#include "branching.hpp"
using namespace std;


/**
 * Default constructor for BranchPredictor class. Each subsequent branch predictor
 * will use this constructor.
 */
BranchPredictor::BranchPredictor(vector<string> pAddresses, int count)
{
    //Setting addresses to parameter.
    addresses = pAddresses;

    //Counters is an array of size (bit buffer / bit size). Setting each value to false or 0 at first
    counters = vector<int>(count, 0);
}

//Default constructor for Static Branch Predictor. Calls parent constructor.
StaticPredictor::StaticPredictor(vector<string> pAddresses, int count) : BranchPredictor(pAddresses, count){};

/**
 * This predict() method overrides the BranchPredictor base class. 
 * Static branch predicts always return false in this case
 */
bool StaticPredictor::predict(string address, int result)
{
    return false;
}

//Default constructor for One Bit Branch Predictor. Calls parent constructor.
OneBitPredictor::OneBitPredictor(vector<string> pAddresses, int count) : BranchPredictor(pAddresses, count){};

/**
 * More overriding. This One Bit Predictor finds
 * the index in array counters for a corresponding address, A
 */
bool OneBitPredictor::predict(string address, int result)
{

    /**
     * I had some trouble with this. Initially, I just
     * used integers for hex conversion. 
     * However, I had out of bounds exceptions out the
     * wazoo. Next, I tried long, same thing. 
     * After scraping C++ docs, I found that `long long`
     * doesn't cause any sort of errors, since it's 64 bit in size.
     * A little cumbersome.
     * 
     * https://en.cppreference.com/w/cpp/language/types
     */

    //Using string to unsigned long long method in base 16 (hexadecimal conversion).
    unsigned long long A = stoull(address, nullptr, 16);
    int N = counters.size();

    int index = A % N;
    
    // Evaluate previous history. Simplifying if statements
    bool prediction = counters[index] == 1;
    
    //Then set counters for next prediction
    counters[index] = result;
    
    return prediction;
}
//Default constructor for Two Bit Branch Predictor. Calls parent constructor.
TwoBitPredictor::TwoBitPredictor(vector<string> pAddresses, int count) : BranchPredictor(pAddresses, count){};

/**
 * Again, more overriding. Uses saturated counter. f the value at counters[index] is 2 or 3, then predict taken;
 */
bool TwoBitPredictor::predict(string address, int result)
{
    //See above for explanation. Tricky C++ implementation
    unsigned long long A = stoull(address, nullptr, 16);
    int N = counters.size();

    int index = A % N;

    //evaluating if counter[index] matches saturated counter
    bool taken = counters[index] >= 2;

    if(result == 1)
    {
        counters[index] = min(counters[index]+1,3);
    }
    else
    {
        counters[index] = max(counters[index]-1, 0);
    }

    return taken;

}
//Default constructor for Three Bit Branch Predictor. Calls parent constructor.
ThreeBitPredictor::ThreeBitPredictor(vector<string> pAddresses, int count) : BranchPredictor(pAddresses, count){};

/**
 * Last override!
 * Similar to Two Bit Predictor, this uses a saturated counter,
 * but with three bits, so we can get up to (2^3)-1 bits. 7!
 */
bool ThreeBitPredictor::predict(string address, int result)
{
    //See body of OneBitPredictor if confused. Tricky implementation
    unsigned long long A = stoull(address, nullptr, 16);

    int N = counters.size();

    int index = A % N;
    //evaluating if counter[index] meets saturated spec
    bool taken = counters[index] >= 4;

    if(result == 1)
    {
        //This time we're getting the max val of saturated counter, 7 instead of 3
        counters[index] = min(counters[index]+1,7);
    }
    else
    {
        counters[index] = max(counters[index]-1, 0);
    }

    return taken;

}

/**
 * I built this method in my advanced programming class. Borrowing it here
 * This is pretty much just input validation for integers.
 */
int get_int_from_user() {
    /* boy oh boy.. was stuck on this for a while
     *
     *  This function uses the stringstream class to read input strings
     *  and convert into the appropriate integer type.
     *
     */

    string integerInput;
    int input;
    stringstream stringStream;
    bool validated;

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