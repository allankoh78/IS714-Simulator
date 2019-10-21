#ifndef __CONSTANT_H_INCLUDED__   
#define __CONSTANT_H_INCLUDED__ 
static const int NUMBEROFRANDOMCHAR = 1000;
static const int TAILSIZE = 3;
static const int NUMOFRFID = 10;
static const int RANDOMSEED = 123456;
static const int NUMOFREADER = 8;
static const int CLONEAGRESSIVE = 5; // Must be non-zero ; 1 is the most agressive ; Higher the number, lesser the agressiveness. 
const enum class PROCESS { intothechain = 1, shipping, receiving, stocking, outofthechain, cloning=9};
#endif
