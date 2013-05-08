#if !defined(__DOUBLEROOM_H)
#define  __DOUBLEROOM_H

#include "Room.h"

class DoubleRoom : public Room
{
private:
	bool allowsExtraBed; //If an extra bed is allowed in the room

public:
	DoubleRoom();  //Default constructor
	DoubleRoom(ifstream& file);	//Makes a new DoubleRoom object from file
	void display();	// Displays double room data
	void writeToFile(ofstream& file);	// Write data to file
	int extraBed() const { return allowsExtraBed ? 1 : 0; }
};

#endif