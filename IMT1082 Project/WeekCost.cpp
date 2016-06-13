#include "WeekCost.h"
#include "timer2.h"

extern Timer timer;

WeekCost::WeekCost(const int start, const int end, const float cst)
    : CostInterval(start, end, cst)
{ }

bool WeekCost::inTimespan(const int date) const
{
    if (CostInterval::inTimespan(date))        // If is in 'global' timespan
    {
        int weekNumber = timer.ukedagsnr(date);    // and check if is weekday
        if (weekNumber < 6 && weekNumber > 0)
            return true;
    }
    return false;
}