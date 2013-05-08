#if !defined(__WEEKCOST_H)
#define  __WEEKCOST_H

#include "CostInterval.h"

class WeekCost : public CostInterval
{
public:
	WeekCost(const int start, // Creates a new week cost object
			const int end, 
			const float cst);
	bool inTimespan(const int date) const;	// Returns true if is in timespan
};
#endif