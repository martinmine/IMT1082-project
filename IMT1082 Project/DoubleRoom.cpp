#include "DoubleRoom.h"
#include <fstream>

DoubleRoom::DoubleRoom()
    : Room(),
      allowsExtraBed(readChar("Allows extrabed (y/n)") == 'y')
{ }

DoubleRoom::DoubleRoom(ifstream& file)
    : Room(file),
      allowsExtraBed(readChar(file) == 'y')
{ }

void DoubleRoom::display()
{
    Room::display();    // Display data
    cout << "\nAllows extrabed: " << (allowsExtraBed ? "yes" : "no") << endl;
}

void DoubleRoom::writeToFile(ofstream& file)
{
    Room::writeToFile(file);    // Write to file
    file << (allowsExtraBed ? 'y' : 'n') << endl;
}