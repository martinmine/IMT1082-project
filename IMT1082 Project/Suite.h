#if !defined(__SUITE_H)
#define  __SUITE_H

#include <string>
using namespace std;

#include "Room.h"

class Suite : public Room
{
private:
    int alteredBedCount; // How many beds have been removed/added to the rom
    int roomSize;        // Size in square meeter of the room

    string description;  // Detailed description of the room
public:
    Suite();                            // Default constructor
    Suite(ifstream& file);                // Read object data from file
    ~Suite();                            // Deconstructor
    void display();                        // Display suite and room data
    void writeToFile(ofstream& file);    // Write object to file
    int extraBed() const { return 5; }    // Allows extrabed
    void setExtraStatus(const int newCount);    // Set altered bed count
};
#endif