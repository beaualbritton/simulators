/**
 * Beau Albritton
 * Cache Simulator 
 * main.cpp
 * 
 * This program aims to simulate a cache (currently read only) according
 * to the instructions given for this assignment in C++.
 * 
 * Gets input from user for memory size, cachesize, block size, and associativity,
 * then creates a new cache object with those parameters. Then, reads each address and prints 
 * the word read.
 */


//Including statements
#include <iostream>
#include <cmath>
//see this if curious, just a helper function
#include "get_int.hpp"
#include "cache.hpp"

int main ()
{
    //size of the memory address, in bits. then memsize = 2**size
    std:: cout << "Enter the memory address size in bits. ";
    int memSizeBits = get_int_from_user(false); // 'false' when checking if power of two, just get any size
    int memSize = pow(2,memSizeBits);
    //size of the cache in bytes (must be a power of two)
    std:: cout << "Enter the cache size in bytes. Must be power of two ";
    int cacheSize = get_int_from_user(true);
    //size of the cache block, in bytes (must be a power of two)
    std:: cout << "Enter the block size in bytes. Must be power of two ";
    int cacheBlockSize = get_int_from_user(true);
    //associativity of a cache
    std:: cout << "Enter the associativity. Must be power of two ";
    int associativity = get_int_from_user(true);
    //finding numsets with the input cachesize
    int numSets = cacheSize/(cacheBlockSize*associativity);

    //Creating a new cache with the input parameters
    Cache cache(numSets,associativity,cacheBlockSize,memSize,cacheSize);
    

    //cout (print) statemement for all inputs, similar to the one found in the instructions
    std::cout << "~~~~~~~~~~~~~~~~~~~~~" << std::endl
          << "cache size = " << cacheSize << std::endl
          << "block size = " << cacheBlockSize << std::endl
          << "num blocks = " << (cacheSize / cacheBlockSize) << std::endl
          << "num sets = " << numSets << std::endl
          << "associativity = " << associativity << std::endl
          << "tag length = " << (cache.memorySizeBits - cache.indexLength - cache.blockOffsetLength) << std::endl
          << "~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    //given addresses to test
    //running each test twice, one for WB the other for WT 


    /**
     * FOR THE FOLLOWING ADDRESSES, EITHER COMMENT OUT SECTION BY SECTION AND COMPARE
     * .OUT FILES VIA ./RUNME.EXE  > OUTPUT_X.OUT
     */


    /**
     * PART 2 ADDRESSES 1 (WB)
     */
    std::cout << "-------------------------------------" << std::endl<<
    "PART 2 ADDRESSES 1 (WRITE BACK)" <<  std::endl
    << "-------------------------------------" << std::endl;

    cache.readWord(1152);
    cache.readWord(2176);
    cache.readWord(3200);
    cache.readWord(4224);
    cache.readWord(5248);
    cache.readWord(7296);
    cache.readWord(4224);
    cache.readWord(3200);
    cache.writeWord(7312, 17, false);
    cache.readWord(7320);
    cache.readWord(4228);
    cache.readWord(3212);
    cache.writeWord(5248, 5, false);
    cache.readWord(5248);
    cache.writeWord(8320, 7, false);
    cache.readWord(8324);
    cache.readWord(9344);
    cache.readWord(11392);
    cache.readWord(16512);
    cache.readWord(17536);
    cache.readWord(8320);
    cache.readWord(17536);
    cache.readWord(17532);

    /**
     * PART 2 ADRESSES 1 (WT)
     */
    std::cout << "-------------------------------------" << std::endl<<
    "PART 2 ADDRESSES 1 (WRITE THROUGH)" <<  std::endl
    << "-------------------------------------" << std::endl;
    cache.readWord(1152);
    cache.readWord(2176);
    cache.readWord(3200);
    cache.readWord(4224);
    cache.readWord(5248);
    cache.readWord(7296);
    cache.readWord(4224);
    cache.readWord(3200);
    cache.writeWord(7312, 17, true);
    cache.readWord(7320);
    cache.readWord(4228);
    cache.readWord(3212);
    cache.writeWord(5248, 5, true);
    cache.readWord(5248);
    cache.writeWord(8320, 7, true);
    cache.readWord(8324);
    cache.readWord(9344);
    cache.readWord(11392);
    cache.readWord(16512);
    cache.readWord(17536);
    cache.readWord(8320);
    cache.readWord(17536);
    cache.readWord(17532);

    std::cout << "-------------------------------------" << std::endl<<
    "PART 2 ADDRESSES 2 (WRITE BACK)" <<  std::endl
    << "-------------------------------------" << std::endl;
    cache.readWord(46916);
    cache.readWord(46932);
    cache.readWord(12936);
    cache.readWord(13964);
    cache.writeWord(46956, 40, false);
    cache.readWord(46956);
    cache.readWord(56132);

     /**
     * PART 2 ADRESSES 2 (WT)
     */
    std::cout << "-------------------------------------" << std::endl<<
    "PART 2 ADDRESSES 2 (WRITE THROUGH)" <<  std::endl
    << "-------------------------------------" << std::endl;
    cache.readWord(46916);
    cache.readWord(46932);
    cache.readWord(12936);
    cache.readWord(13964);
    cache.writeWord(46956, 40, true);
    cache.readWord(46956);
    cache.readWord(56132);


    /**
     * PART 2 ADRESSES 3 (read only)
     */
    std::cout << "-------------------------------------" << std::endl<<
    "PART 2 ADDRESSES 3 (read-only)" <<  std::endl
    << "-------------------------------------" << std::endl;
    cache.readWord(0);
    cache.readWord(13432);
    cache.readWord(13388);
    cache.readWord(62540);
    cache.readWord(4);
    cache.readWord(16452);
    cache.readWord(32900);
    cache.readWord(49348);
    cache.readWord(260);
    cache.readWord(16708);
    cache.readWord(33156);
    cache.readWord(49604);
    cache.readWord(516);







    return 0;
}