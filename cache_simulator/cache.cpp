

#include <cmath>
#include <iostream>
#include "cache.hpp"
using namespace std;

CacheBlock::CacheBlock(int cacheBlockSize){
    /**
     * Constructor for CacheBlock, sets tag to -1
     * and initializes char* data to a new array of cacheBlockSize
     */
    this->tag = -1;
    this->data = new unsigned char[cacheBlockSize];

    //new fields, valid and clean (logical negation of dirty)
    this->valid = false;
    this->clean = true;
}

CacheSet::CacheSet(int cacheBlockSize, int associativity)
{

    /**
     * Constructor for CacheSet, instantiates a new crray of CacheBlocks of size associativity.
     * Since associativity is = 1 for part 1, this doesn't vary much.
     */
    this->blocks = new CacheBlock*[associativity];
    this->associativity = associativity;
    for(int i = 0; i < associativity; ++i)
    {
        //Then initializes each index to a be a new CacheBlock
        blocks[i] = new CacheBlock(cacheBlockSize);
    }

}

Cache::Cache(int numSets, int associativity, int cacheBlockSize, unsigned int memorySize, int cacheSize)
{
    /**
     * Constructor for Cache, takes in several parameters according to assignment specifications.
     * Lots of the "this" keyword below. Beware if allergic to boilerplate code.
     */

    this-> cacheBlockSize = cacheBlockSize;
    this-> numSets = numSets;
    //Similar to processes for CacheBlock and CacheSet. First instantiating a new array of numsize
    this-> set = new CacheSet*[numSets];
    //Then initializing each value in i to a new set 
    for(int i = 0; i < numSets; ++i)
    {
        set[i] = new CacheSet(cacheBlockSize,associativity);
    }
    //Heavy of logbase 2 here, derived from specifications in 'algorithms and info' for this assignment.
    this-> memorySizeBits = logbase2(memorySize);
    this->cacheSizeBits =  logbase2(cacheSize);
    this->cacheBlockSizeBits = logbase2(cacheBlockSize);
    this->indexLength = logbase2(numSets);
    this->blockOffsetLength = logbase2(cacheBlockSize);

    //Instantiating a new array of chars, this time for memory
    this->memory = new unsigned char[memorySize];
    //Initialize memory, i moves in increments of four
    for(int i = 0; i <memorySize; i+=4) //moving 4 bytes at a time
    {
        /**
         * This is supposed to model the 'little endian'  system described in the assignment spec.
         * 0, 1, 2, ..., m, where m = memSize/4 − 1.
         */
        memory[i] = i;
        memory[i+1] = (i>>8);
        memory[i+2] = (i>>16);
        memory[i+3] = (i>>24);
    }
}

int Cache::readWord(unsigned int address)
{
    //Retrieving calculated tag, index, and blockOffset for the given address
    int* decodedAdress = decodeAddress(address);

    //Declaring some local variables for later use.
    // Word is important as its the value returned 
    int word, tag, index, blockOffset;

    tag = decodedAdress[0];
    index = decodedAdress[1];
    blockOffset = decodedAdress[2];

    //instantiating pointers to the target set and target block
    CacheSet* setTarget;
    CacheBlock* blockTarget;
    
    //Then initializing their values,
    //The target 'set' is the first index (or row) in the 'matrix'/2d array CacheSet ** set
    setTarget = this->set[index];

    //Then finding the block in the target set with M associativity 
    for(int m = 0; m < setTarget->associativity; ++m)
    {
        blockTarget = setTarget->blocks[m];
        if(blockTarget->tag == tag && blockTarget->valid)
        {
            //hit
            //need to ensure no duplicate tags for set

            /**
             * `iterator` is a typedef (to int) in C++ that helps with easy iteration of some
             * data structures in C++
             * Setting the iterator to start at the beginning of the LRU queue
             * 
             * SEE: https://cplusplus.com/reference/iterator/
             */
            deque<int>::iterator iter = setTarget->tagQueue.begin();

            while (iter != setTarget->tagQueue.end())
            {
                //pointer to the iterator 
                if (* iter == tag)
                {
                    //remove old tag reference so it can be moved up (Last used)
                    setTarget->tagQueue.erase(iter);
                    break;
                }
                //Then increase iterator
                ++iter;
            }

            //Printing tag queue
            setTarget->tagQueue.push_back(tag); 
            cout << "[tag queue after hit] ";
            for (int t : setTarget->tagQueue)
                cout << t << ", ";
            cout << endl;
        
            //cache hit, return a word (which is 4 "bytes")
            cout << "cache hit at " << address << ": "<< endl;
            cout << "value: ";
            word = 0;

            //for loop, i represents current 'byte' number, and sets the blockTarget's data
            //accordingly
            for(int i = 0; i < 4; ++i)
            {
                unsigned char byte = blockTarget->data[blockOffset+i];
                //multiplying by 8 because 8 bits in each byte,
                //0th (first byte) will get the least significant byte. The logic here is 0*8=0, then 2^0 = 1. This shifts the least
                word += (byte << (i*8));
            }

            cout << "word: " << word << endl;
            return word;
        }
    }
    //Otherwise, its a cache miss
    cout << "cache miss at " << address << ": " << endl;

    //all these values are integers so they will be truncated when divided
    int blockStart = (address/ cacheBlockSize) * cacheBlockSize;
    int blockEnd = blockStart + cacheBlockSize -1;

    //Checking if queue is full
    int blockToReplace = 0;
    if(setTarget->tagQueue.size() >= setTarget->associativity)
    {
        //Tag to evict if tagQueue full is the least recently used tag in the queue (the head)
        int evictedTag = setTarget->tagQueue.front();
        setTarget->tagQueue.pop_front();
        cout << "[evict] Evicting tag: " << evictedTag
          << " from index: " << index << endl;
        //iterating thorugh target set's blocks to find the index 'j'
        for(int j = 0; j<setTarget->associativity; j++)
        {
            if(setTarget->blocks[j]->tag == evictedTag)
            {
                blockToReplace = j;
                //break since the blockToReplace has been found 
                break;
            }
        }
        
    }
    //then setting blockTarget to a new block in the target set (with index j)
    blockTarget = setTarget->blocks[blockToReplace];

    //block is dirty
    if(!blockTarget->clean)
    {
        //Calculating A from slides, A is given by ( 𝑡𝑗 << (sets_bits + block_size_bits) ) + ( 𝑖 << block_size_bits )
        unsigned int A = (setTarget->blocks[blockToReplace]->tag << (logbase2(numSets) + cacheBlockSizeBits)) + (index << cacheBlockSizeBits);

        cout << "Writing back dirty block to memory: " << A << " to " 
        << A + cacheBlockSize - 1 << endl;
        //Then iterating through mem[A] for the length of the cacheBlock, setting that to the target blocks data
        for (int i = 0; i < cacheBlockSize; ++i)
        {
            memory[A + i] = blockTarget->data[i];
        }
    }



    /**
     * Since this is a miss, block needs to be added (or *loaded*) into the cache
     * itself.
     */

    cout << "loading block " << blockStart << " to " << blockEnd <<endl;

    //Loading block from memory and setting the target blocks data to it;
    for(int i = 0; i < cacheBlockSize; ++i)
    {
        blockTarget->data[i] = memory[blockStart+i];
    }

    //Setting the target blocks tag as well
    blockTarget->tag = tag;
    blockTarget->clean = true;
    blockTarget->valid = true;


    //printing queue
    setTarget->tagQueue.push_back(tag);
    cout << "[tag queue after miss] ";
    for (int t : setTarget->tagQueue)
        cout << t << ", ";
    cout << endl;

    word = 0;
    //Similar for loop for returning cache hit (see above)
    for(int i = 0; i < 4; ++i)
    {
        //Starts at smallest, least significant byte (starting at bit 0), and moves towards most significant byte (starting at bit 24 e.g., (i =3 * 8))
        //Pseudocode: return the word at positions b, b+1, b+2, b+3 from the block in set i
        unsigned char byte = blockTarget->data[blockOffset+i];
        word += (byte << (i*8));
    }

    cout << "word: " << word << endl;

    return word;
}

/**
 * IMPORTANT FOR DOCUMENTATION
 * 
 * WRITEWORD IS VERY SIMILIAR TO READWORD, AS A RESULT OF THE SIMILAR LOGIC
 * COMMENTS HERE WILL BE (SLIGHTLY) SPARSE, STILL ROBUST ENOUGH TO UNDERSTAND
 * THE HOW/WHY OF CODE.
 * 
 * 
 */

/**
 * this implementation of writeWord uses a boolean flag @param writeThrough
 * to switch from WriteBack to WriteThrough functionality 
 */
void Cache::writeWord(unsigned int address, int word, bool writeThrough)
{
    int* decodedAdress = decodeAddress(address);

    //Declaring some local variables for later use.
    //Now word isnt declared but read into memory/block data
    int tag, index, blockOffset;

    tag = decodedAdress[0];
    index = decodedAdress[1];
    blockOffset = decodedAdress[2];

    CacheSet* setTarget;
    CacheBlock* blockTarget; 

    setTarget = this->set[index];

    //Again iterating through the targetSets blocks for m associativity 
    for(int m = 0; m < setTarget->associativity; ++m)
    {
        blockTarget = setTarget->blocks[m];
        if(blockTarget->tag == tag && blockTarget->valid)
        {
            //Cache hit
            cout << "Write [cache hit] Address: " << address << ", Tag: " << tag 
          << ", Index: " << index << ", Block Offset: " << blockOffset << endl;


            //same tagqueue logic for iterator, just easier readibility
            deque<int>::iterator iter = setTarget->tagQueue.begin();

            while (iter != setTarget->tagQueue.end())
            {
                //pointer to the iterator 
                if (* iter == tag)
                {
                    //remove old tag reference so it can be moved up (Last used)
                    setTarget->tagQueue.erase(iter);
                    break;
                }
                ++iter;
            }
            setTarget->tagQueue.push_back(tag); 
            // Print updated tag queue for the set (write hit)
            cout << "[tag queue after write hit] ";
            for (int t : setTarget->tagQueue)
                cout << t << " ";
            cout << endl;

            //if writeback, then change the block to dirty            
            if(!writeThrough){
                blockTarget->clean = false;
            }
             
            //for loop, i represents current 'byte' number, and sets the blockTarget's data
            //accordingly
            for(int b = 0; b < 4; ++b)
            {
                //multiplying by 8 because 8 bits in each byte,
                //0th (first byte) will get the least significant byte. The logic here is 0*8=0, then 2^0 = 1. This shifts the least

                blockTarget->data[blockOffset + b] = word >> (b * 8);

                if(writeThrough)
                {
                    this->memory[address+b] = (word >> (b*8));
                }
                
            }
            //Returns even though void, stops the function from running both hit and miss logic on
            //the same address and word
            return;
        }
    }

    //Miss
    //Calculating block start and end
    int blockStart = (address/ cacheBlockSize) * cacheBlockSize;
    int blockEnd = blockStart + cacheBlockSize -1;
    //Then printing 
    cout << "[cache miss] Address: " << address << ", Tag: " << tag 
          << ", Index: " << index << ", Loading block: " << blockStart
          << " - " << blockEnd << endl;


    //Similar to readWord, finding the index and then breaking from it
    int blockToReplace = 0;
    for(int j = 0; j < setTarget->associativity; ++j)
    {   
        int oldestTag = setTarget->tagQueue.front();
        if(setTarget->blocks[j]->tag == oldestTag)
        {
            blockToReplace = j;
            break;
        }
    }
    /**
     *  writeBack, functionality: if the dirty flag of block j in set i is true (and also not writeThrough)
     */
    if (!setTarget->blocks[blockToReplace]->clean && !writeThrough)
    {
        //𝐴 is given by ( 𝑡𝑗 << (sets_bits + block_size_bits) ) + ( 𝑖 << block_size_bits )
        unsigned int A = (setTarget->blocks[blockToReplace]->tag << (indexLength + cacheBlockSizeBits)) + (index << cacheBlockSizeBits);
            cout << "[write back (" << A << " - " << (A + cacheBlockSize - 1) << ")]" << endl;
        
        for (int i = 0; i < cacheBlockSize; ++i) {
            //Wriitng block_size bytes of block j of set i to memory at A to A + blocksize - 1
            memory[A + i] = setTarget->blocks[blockToReplace]->data[i];
        }
    }

    //reading the blocksize bytes of memory from start to start + blocksize - 1 into block j of set i
    for(int i = 0; i < cacheBlockSize; ++i)
    {
        blockTarget->data[i] = this->memory[blockStart+i];
    }
    setTarget->blocks[blockToReplace]->valid = true;
    setTarget->blocks[blockToReplace]->tag = tag;
    setTarget->blocks[blockToReplace]->clean = writeThrough;
    
    setTarget->tagQueue.push_back(tag);
    cout << "[tag queue after write miss] ";
    for (int t : setTarget->tagQueue)
        cout << t << ", ";
    cout << endl;

    //writing to block data (and memory if writeThrough)
    for(int b = 0; b < 4; ++b)
    {
        //multiplying by 8 because 8 bits in each byte,
        //0th (first byte) will get the least significant byte. The logic here is 0*8=0, then 2^0 = 1. This shifts the least
        
        //right bitshift operator instead of left since not reading word, but writing it into data
        setTarget->blocks[blockToReplace]->data[b] = (word >> (b*8));
        
        /*
            if writethrough, then write the given word (the four bytes) in positions addr, addr+1, addr+2, addr+3 of memory
        */
        if(writeThrough)
        {
            this->memory[address+b] = (word >> (b*8));
        }
    }
}

int* Cache::decodeAddress(unsigned int address)
{
    cout << "address:" << address << endl;
    /*
        Finding block offset first, Masking the address with & operation by (1<<blockOffset) -1.
        Effectively this is 2 to the power of block offset -1. For a 6 bit blockOffset,
        1 << 6 yields 64. (2^6), then -1    
    */
    int blockOffset = address&((1<<blockOffsetLength)-1);
    cout << "block offset:" << blockOffset << endl;

    /*
        Then finding index. Which is just shifted down by the length of the offset,
        and  masking that part of the address with the 1 << indexLength - 1,
        again this is effecively 2^N or 2^indexLength  
    */
    int index = (address >> blockOffsetLength )&((1 << indexLength)-1);
    cout << "index:" << index << endl;
    
    /*
        After that, the tag is easy to find. Just shifting down (>>) by offSetLength+indexLength
    */
    int tag =  address >> (blockOffsetLength+indexLength);
    cout << "tag:" << tag << endl;
    
    //Returning an array (of size 3) containing tag, index and blockOffset.
    int* decoded  = new int[3];

    decoded[0] = tag;
    decoded[1] = index;
    decoded[2] = blockOffset;

    return decoded;
    
}

