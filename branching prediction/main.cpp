
/**
 * Beau Albritton
 * main.cpp for branching prediction
 * 
 * This main file runs branch prediction.
 *  It then calls a python script to graph results.
 * 
 * Takes an input file, (provided in directory)
 * Bit predictor size (integer)
 * Buffer size (integer)
 */

using namespace std;
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "branching.hpp"


int main()
{

    /**
     * Getting user input. Pretty self explanatory. Output file,
     * (not really validated. Be specific!)
     * 
     * Integer input for bit count and buffer size
     */
    cout << "enter filename: " << endl;

    string inputString;
    //Using getline. c standard lib functions ftw
    getline(std::cin,inputString);

    cout << "Enter Bit Size for Branch Predictor. ";
    int bitCountAmt = get_int_from_user();
    cout << "Bit Size: " << bitCountAmt <<endl;

    cout << "Enter Buffer Size (in bits) for Branch Predictor. ";
    int bitBufferAmt = get_int_from_user();
    cout << bitBufferAmt <<endl;
    //Creating an input file object for given trace file

    std::ifstream inputFile(inputString);
    string line;

    //Setting up addresses to be parsed.
    vector<string> addresses;
    vector<int> results;

    //Iterating through each line. Better with while loop (rather than getting file line size)
    while(getline(inputFile,line))
    {
        //String stream for string manipulation. Need to splice input file string. 
        stringstream stream(line);
        string address;
        int result;
        
        //Finding index of whitespace int the line. This is the delimeter
        int whitespace = line.find(' ');

        //Substring manipulation
        address = line.substr(0,whitespace);
        result = stoi(line.substr(whitespace+1));

        //Adding to vectors
        addresses.push_back(address);
        results.push_back(result);
    }

    //Setting counter size to be passed to Branch Predictor objects
    int numCounters;
    if(bitCountAmt!=0)
    {
        numCounters = bitBufferAmt/bitCountAmt;
    }
    
    //Using pointer for easier polymorphism 
    BranchPredictor* predictor;

    /**
     * Big chunk of else if. Pretty much just setting
     * the Branch Predictor object to a specific 
     * child class based on input. (0 for static, 1 for one bit, etc.)
     */
    if(bitCountAmt == 0 )
    {
        predictor = new StaticPredictor(addresses,0);
    }
    else if (bitCountAmt == 1)
    {
        predictor = new OneBitPredictor(addresses,numCounters);
    }
    else if (bitCountAmt == 2)
    {
        predictor = new TwoBitPredictor(addresses,numCounters);
    }
    
    else if (bitCountAmt == 3)
    {
        predictor = new ThreeBitPredictor(addresses,numCounters);
    }

    //Initializing counters for graphing later
    int numCorrect = 0;
    int numIncorrect = 0;
    int totalCount = 0;

    for(int i = 0; i <addresses.size(); ++i)
    {
        //Getting predicted result for each address. Then comparing to actual result later.
        bool prediction = predictor->predict(addresses[i],results[i]);

        //Compare prediction vs actual and increment accordingly
        if(prediction == results[i])
        {
            ++numCorrect;
        }
        else 
        {
            ++numIncorrect;
        }
        ++totalCount;
    }

    
    //double num = numCorrect / totalCount;

    //Outputting correct/incorrect
    cout<< "correct: " << numCorrect << endl;
    cout<< "incorrect: " << numIncorrect << endl;
    //Closing inputFile
    inputFile.close();

    /**
     * Using system calls to run a python script with simple matplotlib 
     * to graph a pie chart of correct vs incorrect for each run of the
     * predictor 
     */

    string filepath = "graph.py";
    //Chunky command to run
    string command = "py " + filepath + " " + to_string(totalCount) + " " + to_string(numCorrect) + " " + to_string(bitCountAmt) + " " +inputString;

    /**
     * Calling system method. Using c_str() since it adds a terminatiing character for proper output to python
     * 
     * see: https://cplusplus.com/reference/string/string/c_str/
     */
    system(command.c_str());

    //Deleting pointer
    delete predictor;

    return 0;
}

