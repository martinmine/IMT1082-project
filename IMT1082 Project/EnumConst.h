// This file contains all consts and enums used in the program

#if !defined(__ENUMCONST_H)
#define  __ENUMCONST_H

#include <string>
using namespace std;

const int ROOM_TYPES = 3;	//Amount of types of rooms (single, double, suite)
const string CURRENCY = "NOK"; //Current currency used in the program
const string HOTELS = "HOTELS.DTA";	// File with filename of all hotels

enum RoomType
{
	Single	= 0,
	Double	= 1,
	SuiteRoom	= 2
};

enum ReadFlag
{
	None = 0,
	Trim = 1,
	ToLower = 2,
	ToUpper = 4,
};

#endif