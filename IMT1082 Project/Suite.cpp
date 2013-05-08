#include "Suite.h"
#include <fstream>

Suite::Suite() // Default constructor, makes empty suite
	: Room(),	  
	  roomSize(readValue<int>("Size of room in square meeters", 10, 900)),
	  alteredBedCount(0)
{ 
	readLine(description, "Room description");
}

Suite::Suite(ifstream& file)
	: Room(file),
	  alteredBedCount(readValue<int>(file)),
	  roomSize(readValue<int>(file))
{ 
	readLine(file, description);
}

Suite::~Suite() // Deconstructor
{
}

void Suite::display()
{
	Room::display();
	cout << "\nRoom size: " << roomSize << endl;
	cout << "Detailed room description: " << description << endl;
	cout << "Beds added/removed: " << alteredBedCount << endl;
}

void Suite::writeToFile(ofstream& file)
{
	Room::writeToFile(file);
	file << alteredBedCount << ' ' << roomSize << endl;
	file << description << endl;
}

void Suite::setExtraStatus(const int newCount)
{
	alteredBedCount = newCount;
}