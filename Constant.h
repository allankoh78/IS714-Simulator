#ifndef __CONSTANT_H_INCLUDED__   
#define __CONSTANT_H_INCLUDED__ 
static const int NUMBEROFRANDOMCHAR = 1000;
static const int TAILSIZE = 3;
static const int NUMOFRFID = 1000; //10;
static const int RANDOMSEED = 123456;
static const int NUMOFREADER = 4; //10;
static const int MAXCLONEPERCYCLE = 5;
static const int CLONEAGRESSIVE = 10; // Must be non-zero ; 1 is the most agressive ; Higher the number, lesser the agressiveness. 
const enum class PROCESS { intothechain = 1, shipping, receiving, stocking, outofthechain, cloning=9};
#endif

/* TODO::

Do a comparision if the clone event is missed due to similar tail value or other reasons.

What if we got only 4 reader.


*/