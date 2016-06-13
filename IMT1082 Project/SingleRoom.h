#if !defined(__SINGLEROOM_H)
#define  __SINGLEROOM_H

#include "Room.h"

class SingleRoom : public Room
{
public:
    SingleRoom(ifstream& file);    // Read object data from file
    int extraBed() const { return 0; }    // Doesn't allow bed
};


#endif