#if !defined(__ROOMCOST_H)
#define  __ROOMCOST_H

#include "EnumConst.h"
#include "listtool2.h"

class RoomCost
{
private:
	float defaultWeekCost[ROOM_TYPES];		// Default costs in the week days
	float defaultWeekendCost[ROOM_TYPES];	// Default costs in the weekends

	List* customCosts[ROOM_TYPES];			// Custom room costs

	int periodStart; // Date when the cost start applying to the rooms
	int periodEnd;   // Last date when the cost applies to the rooms
	RoomType type;   // Which room type this applies to

	RoomType getType(const string& str);
	void setType(ifstream& file, float costs[]);
	void setCustom(ifstream& file);
public:
				// Default constructor, creates a default room cost object
	RoomCost();	// with default values, all costs is set to 0
								// Reads the file structure from the 'file'
	RoomCost(ifstream& file);	// into the roomcost object
	~RoomCost();				// Destructor
	void setCost(float* costs, const int start,		// Reads in the cost  
				const int end, const RoomType type);// data into 'costs'
};

#endif