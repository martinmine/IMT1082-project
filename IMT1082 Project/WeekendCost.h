#if !defined(__WEEKENDCOST_H)
#define  __WEEKENDCOST_H

#include "CostInterval.h"

class WeekendCost : public CostInterval
{
public:
	WeekendCost(const int start,  // Creates new weekend cost object
			const int end, 
			const float cst);
	bool inTimespan(const int date) const; // Returns true if data is in timespan
};
#endif