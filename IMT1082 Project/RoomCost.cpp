#include "RoomCost.h"
#include "Global.h"
#include "CostInterval.h"
#include "timer2.h"
#include "WeekCost.h"
#include "WeekendCost.h"

extern Timer timer;

RoomCost::RoomCost() // Default ctor for RoomCost, makes empty object
{
	for (int i = 0; i < ROOM_TYPES; i++)	// Set all fields to 0
	{
		defaultWeekCost[i]		= 0;
		defaultWeekendCost[i]	= 0;
		customCosts[i]			= new List(listtype::Sorted);
	}
}

RoomCost::~RoomCost()
{
	for (int i = 0; i < ROOM_TYPES; i++)
		delete customCosts[i];
}

RoomCost::RoomCost(ifstream& file)
{
	string val;

	for (int i = 0; i < ROOM_TYPES; i++)	// Init weekend costs
		customCosts[i] = new List(listtype::Sorted);

	while (!file.eof())
	{
		getline(file, val);	// Read first line header

		if (val == "Standard, uke:")	// if is standard 
		{
			setType(file, defaultWeekCost);
		}
		else if (val == "Standard, helg:")	// if not
		{
			setType(file, defaultWeekendCost);	// read weekend
			setCustom(file);	// Which is always followed by custom costs
		}
	}
}

void RoomCost::setType(ifstream& file, float costs[])
{
	string type;
	float cost;

	for (int i = 0; i < ROOM_TYPES; i++)	// Go over all categories
	{
		getline(file, type, ' ');	// Get type
		file >> cost;	// Read cost
		file.ignore();	// Ignore newline
		costs[getType(type)] = cost;	// set cost
	}
}

void RoomCost::setCustom(ifstream& file)
{
	int periodBegin;	// Period start
	int periodEnd;		// Period end
	int count;			// Count of entries under start - end (count)
	float cost;			// Cost for one entry
	string roomType;	// Type of room
	string costType;	// Cost

	CostInterval* interval;

	file >> periodBegin;
	file >> periodEnd;
	file >> count;
	file.ignore();

	for (int i = 0; i < count; i++)	// Go over all entries
	{
		getline(file, roomType, ' ');
		getline(file, costType, ' ');
		file >> cost;
		file.ignore();
							// Thanks Ubama for not adding switch for string
		if (costType == "uke")	// If is week cost
			interval = new WeekCost(periodBegin, periodEnd, cost);
		else if (costType == "helg")	// if weekend cost
			interval = new WeekendCost(periodBegin, periodEnd, cost);
		else   // or everything else
			interval = new CostInterval(periodBegin, periodEnd, cost);

		customCosts[getType(roomType)]->add((Element*)interval);
	}
}

void RoomCost::setCost(float* costs, const int start, 
					   const int end, const RoomType type)
{
	int i = 0,	// Position in costs array
		dayNr,	// Number of current day in the weekend
		currentDate = start;	// The first date in between <start, end>
	Element* elem;				// Element taken out from sorted list
	CostInterval* cost;			// Casted element

	do
	{
		dayNr = timer.ukedagsnr(currentDate);
		// Set default costs
		if (dayNr == 6 || dayNr == 0)	// Is weekend
			costs[i] = defaultWeekendCost[type];
		else	// Normal weekday
			costs[i] = defaultWeekCost[type];

		// Set custom costs
		for (int k = 1, j = customCosts[type]->no_of_elements() + 1; k < j; k++)
		{
			elem = customCosts[type]->remove_no(k);
			cost = (CostInterval*)elem;

			if (cost->inTimespan(currentDate))
				costs[i] = cost->getCost();

			customCosts[type]->add(elem);
		}

		i++;
		currentDate = timer.nestedato(currentDate);
	}			// Loop untill the 'current date' date is on same day as 'end'
	while (currentDate < timer.nestedato(end));	
}

RoomType RoomCost::getType(const string& str)
{
	if (str == "singel")
		return RoomType::Single;
	else if (str == "dobbel")
		return RoomType::Double;
	else
		return RoomType::SuiteRoom;
}