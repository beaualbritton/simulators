//for tagqueue (deque is just a 'double ended' queue)
//implementing my own priority queue seemed impractical for this project
#include <deque>

using namespace std;

class CacheBlock
{
    /**
     * CacheBlock class according to instructions for this assignment.
     * Has a tag, and an array of chars for data (in this case they are unsigned)
     */

    public:
    int tag;
    unsigned char* data;
    bool clean; 
    bool valid;
    
    CacheBlock(int cacheBlockSize);
};
class CacheSet
{
    /**
     * CacheSet class according to instructions for this assignment.
     * Has a block size, and an array of chars for blocks, except CacheBlock
     * is a double pointer (this is similar to a 2d array conceptually)
     */


    public:
    int cacheBlockSize;
    CacheBlock** blocks;
    deque<int> tagQueue; 

    int associativity;

    CacheSet(int cacheBlockSize, int associativity);
};
class Cache{

    /**
     * Cache class according to instructions for this assignment.
     * Has several fields, naming should be robust and readable. 
     * 
     * Importantly, contains an array of bytes (in this case unsigned chars) to represent memory,
     * and an array of CacheSets, a double pointer for intuitive access in implementation.
     * 
     * Also contains the methods for readWord() which returns a 'word' (4 byte number)
     * and decodeAddress() returns an array of three values, {tag, index, blockOffset}
     */

    public:
    int numSets,memorySizeBits,cacheBlockSize,cacheSizeBits,cacheBlockSizeBits,indexLength,blockOffsetLength;

    CacheSet** set;
    unsigned char* memory;

    Cache(int numSets, int associativity,int cacheBlockSize, unsigned int memorySize, int cacheSize);

    int readWord(unsigned int address);

    void writeWord(unsigned int address, int word, bool writeThrough);

    int* decodeAddress (unsigned int address);


};  

/**
 * Calculates logbase2 using right bitshift 
 */
inline int logbase2(int x){
    int i = 0;
    while (x > 0){
        i = i+1;
        x = x >> 1;
    }
    return (i-1);
}