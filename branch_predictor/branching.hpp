
/**
 * Beau Albritton
 * branching.hpp
 * 
 * This header contains basic setup for Static, One Bit, Two Bit
 * and Three Bit branch prediction using polymorphism in C++
 */

#include <string>
#include <vector>
using namespace std;


/**
 * Parent class for the predictors. Contains arrays for 
 * addresses passed through, as well as counters for each 
 * address (defined by bit buffer size/bit branch size in assignment specification)
 */
class BranchPredictor
{

    public: 
        vector<string> addresses;
        vector<int> counters;

        virtual bool predict(string address, int result) = 0;
        BranchPredictor(vector<string> pAddresses, int count);
        //default constructor, polymorphism in C++ isweird
        BranchPredictor() {};
        vector<string> getAddresses();
};


/**
 * Warning: 
 * Boilerplate below. Overriding the constructor and predictor
 * for each derived BranchPredictor class. Implementation in `branching.cpp`
 */

class StaticPredictor : public BranchPredictor
{
    public: 
        bool predict(string address, int result) override;
        StaticPredictor(vector<string> pAddresses, int count);
};

class OneBitPredictor : public BranchPredictor
{
    public: 
        bool predict(string address, int result) override;
        OneBitPredictor(vector<string> pAddresses, int count);
};

class TwoBitPredictor : public BranchPredictor
{
    public: 
        bool predict(string address, int result) override;
        TwoBitPredictor(vector<string> pAddresses, int count);
};

class ThreeBitPredictor : public BranchPredictor
{
    public:
        bool predict(string address, int result) override;
        ThreeBitPredictor(vector<string> pAddresses, int count);
};


/*
  SEE: branching.cpp
  Declaring a function that I implemented in advanced programming (c++). Integer input validation
*/
int get_int_from_user();