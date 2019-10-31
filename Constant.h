#ifndef __CONSTANT_H_INCLUDED__   
#define __CONSTANT_H_INCLUDED__ 
// Based on the example given in the paper, this simulator assumed Tail Size (t) = 3 , Tail Symbol Size (s) = 8.    
static const int NUMBEROFRANDOMCHAR = 1000;  // Pool of random symbols (8 bits) assigned for tail.
static const int TAILSIZE = 3; // Length of the tail (t)
static const int NUMOFRFID = 1000; //10; // Number of RFID simulated
static const int RANDOMSEED = 123456; // Random seed to generate the pool of random symbols (8 bits)
static const int NUMOFREADER = 4; //10; // Number of Reader simulated
static const int MAXCLONEPERCYCLE = 2; // Maximum number of consecutive clone that can be created for an RFID. This prevents an infinite loop of clone created by aggressive cloning (CLONEAGRESSIVE=1).
static const int CLONEAGRESSIVE = 2; // Must be non-zero ; 1 is the most agressive ; Higher the number, lesser the agressiveness. 
const enum class PROCESS { intothechain = 1, shipping, receiving, stocking, outofthechain, cloning=9}; // RFID operations.
using namespace std;

#define WINDOWS // uncomment this line to use it for windows.
//#define TESTER  // Uncomment this line to use the tester to check if the functions are running correctly.
#endif
