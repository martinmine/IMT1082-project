#include "WeekendCost.h"
#include "timer2.h"

#include <iostream>

extern Timer timer;

WeekendCost::WeekendCost(const int start, const int end, const float cst)
    : CostInterval(start, end, cst)
{ }

bool WeekendCost::inTimespan(const int date) const
{
    if (CostInterval::inTimespan(date))    // If is within 'global' timepsan
    {
        int weekNumber = timer.ukedagsnr(date);    // and is a weekend day
        if (weekNumber == 6 || weekNumber == 0)
            return true;
    }
    return false;
}