#if !defined(__COSTINTERVAL_H)
#define  __COSTINTERVAL_H

#include "listtool2.h"

class CostInterval : public Num_element
{
private:
    int periodEnd;        // The end of the period the cost is valid for
    float cost;            // Cost in interval
public:                    // Default ctor
    CostInterval(const int start, const int end, const float cst);
    float getCost() const { return cost; };    // Returns cost for period
                    // Returns true if date is a valid date within the timespan
    virtual bool inTimespan(const int date) const;
                    // Returns the start period date
    int getStartPeriod() const { return Num_element::number; }
                    // Returns the end period date
    int getEndPeriod() const { return periodEnd; }
};

#endif
