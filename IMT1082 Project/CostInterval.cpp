#include "CostInterval.h"
#include "timer2.h"
#include "Global.h"

extern Timer timer;

CostInterval::CostInterval(const int st, const int nd, const float cst)
	: Num_element(st),
	  periodEnd(nd),
	  cost(cst)
{ }

bool CostInterval::inTimespan(const int date) const
{
	// If start period is on the same date, or is after start period
	// AND date is on the same or before end period, return true
	return (getStartPeriod() <= date && getEndPeriod() >= date);
}