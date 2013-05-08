#include "Hotel.h"
#include "DoubleRoom.h"
#include "SingleRoom.h"
#include "Suite.h"
#include "timer2.h"

extern Hotel* curtHotel;
extern Timer timer;

Hotel::Hotel(ifstream & file)					// Construct read hotel from file
{
	readLine(file, name);			// Get name of hotel,
	readLine(file, fileName);		// the file name,
	readLine(file, streetAddress);	// street address,
	readLine(file, postalAddress);	// postal address,
	readLine(file, email);			// and the email

	phoneNumber = readValue<int>(file);		// Get phonenumber from file
	faxNumber = readValue<int>(file);		// Get the fax number from file
	breakfastCost = readValue<int>(file);	// Get Cost of breakfast
	extrabedCost = readValue<int>(file);	// Get cost of extra bed
	facilityCount = readValue<int>(file);	// Get how many facilities they have

	facilities = new string[facilityCount];	// Make the facility strings

	int i;
	Room* room;									// Create a room pointer

	for (i = 0; i < facilityCount; i++)			// Loop all the facilities
		readLine(file, facilities[i]);			// Get name of facility

	for (i = 0; i < ROOM_TYPES; i++)			// Set each list to sorted
		rooms[i] = new List(listtype::Sorted);	// initilize room lists

	readRooms<SingleRoom>(file, rooms[RoomType::Single]);	// Read single room
	readRooms<DoubleRoom>(file, rooms[RoomType::Double]);	// double rooms
	readRooms<Suite>(file, rooms[RoomType::SuiteRoom]);		// and suites
}

template <class T>
inline void Hotel::readRooms(ifstream& file, List* storage)
{
	int numRooms = readValue<int>(file);	// Get number of rooms
	Room* room;
	for (int i = 0; i < numRooms; i++)			// Read all rooms
	{		
		room = new T(file);					// Create new room
		room->readReservations(file);
		if (room->getRoomNumber() > highestRoomNum)
			highestRoomNum = room->getRoomNumber();
		storage->add(room);					// Put in correct list
	}
}

Hotel::~Hotel() // Deconstructor
{
	for (int i = 0; i < ROOM_TYPES; i++)
		delete rooms[i];

	delete[] facilities;
}

void Hotel::display()
{
	cout	<< "\n\nHotelname: " << name << '\n'				// print name
			<< "Street address: " << streetAddress				// Address
			<< "\nPostal address: " << postalAddress << '\n'	// Postal addr
			<< "Phone number: " << phoneNumber					// Phonenumber
			<< "\nEmail: " << email								// Hotel email
			<< "\nFax number: " << faxNumber << '\n'			// Fax number
			<< "Facilities: ";					

	for (int i = 0; i < facilityCount; i++)				// Will list the fac.
		cout << "\n\t\t" << facilities[i];				// Print one facility
}

void Hotel::write()					// Write given hotel to file
{
	int i, j, numRooms;
	Room* pointer;

	ofstream file(fileName + ".DTA");		// Open file

	file	<< name << '\n'					// Wirte Hotelname to file
			<< fileName << '\n'				// Filenavn to file
			<< streetAddress << '\n'		// Streetaddress to file
			<< postalAddress << '\n'		// Postaddress
			<< email << '\n'				// Hotel email
			<< phoneNumber << '\n'			// Hotel's phone number
			<< faxNumber << '\n'			// Hotel's fax number
			<< breakfastCost << '\n'		// Price of breakfast
			<< extrabedCost << '\n'			// Price of extra bed
			<< facilityCount << '\n';		// How many facilitys the hotel have

	for (i = 0; i < facilityCount; i++)		// Write all the diffrent facilitys
		file << facilities[i] << '\n';		// Write currecnt facility in loop

	for (i = 0; i < ROOM_TYPES; i++)
	{
		numRooms = rooms[i]->no_of_elements();	// NUm of rooms

		file << numRooms << '\n';				// Print number to file

		for (j = 1; j < numRooms + 1; j++)		// Print all the rooms to file
		{
			pointer = (Room*) rooms[i]->remove_no(j);// Get rooms pointer

			pointer->writeToFile(file);			// Write room to file
			pointer->writeReservations(file);

			rooms[i]->add(pointer);	// Add room back into list
		}
	}

	file.close();
}

void Hotel::bookRoom()
{
	cout << "Available room types:\n";
	cout << "1: Single room\n";
	cout << "2: Double room\n";
	cout << "3: Suite\n";
												// Read room type
	RoomType type = (RoomType)(readValue<int>("Select a room type", 1, 3) - 1);
	int startPeriod = readDate("Arrival date");	// Read date
	int endPeriod = readDate("Departure date");	// period
	if (startPeriod > endPeriod)
	{
		cout << "End period can't be before start date\n";
	}
	else
	{
		Room* room = getFreeRoom(type, startPeriod, endPeriod);	// Get the room
		if (room == nullptr)	// Show message if nothing found
			cout << "Fully booked in this period for this room type\n";
		else	// If not, reservate the room
		{
			RoomCost costData = getRoomCost(fileName + string(".PRS"));
			room->reservate(startPeriod, endPeriod, costData, type);
		}
	}
}

Room* Hotel::getFreeRoom(const RoomType type, const int startPeriod, 
						 const int endPeriod, const int notRoom)
{
	Element* elem;				// Element we take out from list
	Room* room;					// Room we get from the list
	Room* validRoom = nullptr;	// Available room in the time period
	for (int i = 1, j = rooms[type]->no_of_elements() + 1; i < j; i++)
	{
		elem = rooms[type]->remove_no(i);	//Remove from right category
		room = (Room*)elem;									// If available
		if (room->available(startPeriod, endPeriod) && !(*room == notRoom))
		{
			validRoom = room;	// Set the valid room
			i = j;				// Stop the loop
		}
		rooms[type]->add(elem);	// Add back to list
	}

	return room;
}



void Hotel::cancelReservation()
{
	string reservator = readLine("Reservator");

	for (int i = 0; i < ROOM_TYPES; i++)
		cancelReservation((RoomType)i, reservator);
}

void Hotel::cancelReservation(const RoomType type, const string& name)
{
	Element* elem;				// Element we take out from list
	Room* room;					// Room we get from the list
	for (int i = 1, j = rooms[type]->no_of_elements() + 1; i < j; i++)
	{
		elem = rooms[type]->remove_no(i);	//Remove from right category
		room = (Room*)elem;
		if (room->isReservator(name) && 
			readChar("Cancel reservation? (y/n)") == 'y')	// If available
		{
			delete room;
		}
		else
		{
			rooms[type]->add(elem);	// Add back to list
		}
	}
}

void Hotel::printAllRooms()
{
	for (int i = 0; i < ROOM_TYPES; i++)
		printRooms((RoomType)i);
}

void Hotel::printRooms(const RoomType type)
{
	Element* elem;				// Element we take out from list
	Room* room;					// Room we get from the list
	for (int i = 1, j = rooms[type]->no_of_elements() + 1; i < j; i++)
	{
		elem = rooms[type]->remove_no(i);	//Remove from right category
		room = (Room*)elem;
		room->display();	// Display the room

		rooms[type]->add(elem);
	}
}

void Hotel::checkIn(const string& res)	// Checks in a reservator
{
	int date = timer.hent_dato();			// Todays date
	for (int i = 0; i < ROOM_TYPES; i++)
		checkIn(res, (RoomType)i, date);	// Try and check in the person
}

						// Checks in a person in a given room type whose
						// reservator equals name and reserved date 
						// equals date
void Hotel::checkIn(const string& res, const RoomType type, const int date)
{
	Element* elem;				// Element we take out from list
	Room* room;					// Room we get from the list
	for (int i = 1, j = rooms[type]->no_of_elements() + 1; i < j; i++)
	{
		elem = rooms[type]->remove_no(i);	// Remove from right category
		room = (Room*)elem;
		room->tryCheckIn(res, date);		// Try check in

		rooms[type]->add(elem);
	}
}

void Hotel::cancelBooking()
{
	Element* elem;	// Element taken out from the room list
	Room* room;		// elem casted to a Room*
											// Name of reservator
	string reservator = readLine("Reservator", ReadFlag::ToUpper);	

	for (int type = 0; type < ROOM_TYPES; type++)	// Go over each type
	{												// and each room
		for (int i = 1, j = rooms[type]->no_of_elements() + 1; i < j; i++)
		{
			elem = rooms[type]->remove_no(i);	// Take out the room
			room = (Room*)elem;					// Cast it
			room->cancelReservation(reservator);	// Ask if cancel if any
			rooms[type]->add(room);				// add back
		}
	}
}

void Hotel::changeDepartureDate()
{													// Room to change
	Room* room = getRoom(readValue<int>("Room number", 0, highestRoomNum));
	string reservator = readLine("Reservator");		// Room reservator         

	if (room != nullptr)							// If room found
		room->changeDepartureDate(reservator);		// Change data
	else
		cout << "\n404: Room not found\n";
}

void Hotel::changeArrivalDate()
{													// Room to change
	Room* room = getRoom(readValue<int>("Room number", 0, highestRoomNum));
											// Room reservator
	string reservator = readLine("Reservator", ReadFlag::ToUpper);		

	if (room != nullptr)							// If room found
		room->changeArrivalDate(reservator);		// Change data
	else
		cout << "\n404: Room not found\n";
}

Room* Hotel::getRoom(const int roomNumber)
{
	Element* elem;	// Element taken out from the room list
	Room* room;		// elem casted to a Room*

	for (int type = 0; type < ROOM_TYPES; type++)	// Go over each type
	{												// and each room
		for (int i = 1, j = rooms[type]->no_of_elements() + 1; i < j; i++)
		{
			elem = rooms[type]->remove_no(i);	// Take out the room
			room = (Room*)elem;					// Cast it
			rooms[type]->add(room);				// add back

			if (*room == roomNumber)			// If room numbers equals
				return room;					// return its value
		}
	}

	return nullptr;
}

void Hotel::checkOut(const int roomnr)
{
	Room* room = getRoom(roomnr);  

	if (room != nullptr)
		room->roomCheckOut(fileName);
	else
		cout << "\nRoom not found.\n";
}

void Hotel::registerBill(const int roomnr)
{
	Room* room = getRoom(roomnr);

	if (room != nullptr)
		room->roomRegisterBill();
	else
		cout << "\nRoom not found.\n";
}

void Hotel::displayRoomReservations()
{
	int roomNumber = readValue<int>("Room number", 0, highestRoomNum);
	Room* room = getRoom(roomNumber);
	
	if (room != nullptr)
		room->displayReservations();
	else
		cout << "404: Room not found\n";
}

void Hotel::displayCurrentRoomReservationDetails()
{
	int roomNumber = readValue<int>("Room number", 0, highestRoomNum);
	Room* room = getRoom(roomNumber);
	
	if (room != nullptr)
		room->displayReservationsDetails();
	else
		cout << "404: Room not found\n";
}

void Hotel::displayRoomAvailabillity()
{
	int roomNumber = readValue<int>("Room number", 0, highestRoomNum);
	Room* room = getRoom(roomNumber);
	
	if (room != nullptr)
		room->displayAvailabillity();
	else
		cout << "404: Room not found\n";
}

void Hotel::displaySuites()	// Display hotel suites
{
	int numRooms = rooms[RoomType::SuiteRoom]->no_of_elements() + 1,
		i = 1;
	Suite* pointer;			// Current suite

	if (numRooms > 0)		// Check if any suites
	{
		for (i; i < numRooms; i++)	// Loop all suites
		{							// Get room from list
			pointer = (Suite*) rooms[RoomType::SuiteRoom]->remove_no(i);
			pointer->display();		// Display info about this suite
			rooms[RoomType::SuiteRoom]->add((Element*)pointer);	// Add back
		}
	}
	else	
	{									// No suites was found
		cout << "\n\n\t\tNo Suites found on this hotel!\n\n";
	}
}

void Hotel::customerReservations()	// Check reservations of given customer
{
	string customer;				// Customer
	int i, j, numRooms, hits = 0;
	Room* pointer;					// Current room

	customer = readLine("Enter name of customer");	// Get name of customer

	upperString(customer);			// Convert to upperstring

	for (i = 0; i < ROOM_TYPES; i++)	// Loop all romm types
	{
										// Get num of room of given roomtype
		numRooms = rooms[i]->no_of_elements();	

		for (j = 1; j <= numRooms; j++)		// Loop all rooms of type
		{
			pointer = (Room*) rooms[i]->remove_no(j);	// Get a room
							// Add how many reservations she had in given room
			hits += pointer->customerReservation(customer);	

			rooms[i]->add(pointer);			// Add room back to list
		}
	}
										// Print how many reservations user had.
	cout << "\n\t" << customer << " had " << hits << " reservation(s)\n";
}

void Hotel::switchReservation()	// Switch reservations to given customer
{
	string customer;			// Name of customer
	int i, j, numRooms;			// Int's used
	Room* pointer;				// Current room of customer

	customer = readLine("Enter name of customer");	// Get cus. name

	upperString(customer);		// Convert name to uppercase

	for (i = 0; i < ROOM_TYPES; i++)	// Loop all roomtypes
	{
		numRooms = rooms[i]->no_of_elements();

		for (j = 0; j < numRooms; j++)	// Loop all rooms in given type
		{
			pointer = (Room*) rooms[i]->remove_no(j);	// Get room
												// Check for reservations in room
			pointer->reservationSwitch(customer, (RoomType)i);	

			rooms[i]->add(pointer);					// Add room back to list
		}
	}
}

void Hotel::displayAllRoomsInACategoryAndTimespan()
{
	Element* elem;	// Element taken out from the room list
	Room* room;		// elem casted to a Room*
	char roomType;  
	int arrDate;    
	int depDate;
	int roomTypes;
	do              // asks for a roomtype
		roomType = 
		toupper(readChar("Type of room: \nR - single\nD - double\nS - suite\n"));
	while (roomType != 'R' && roomType != 'D' && roomType != 'S');
	
	arrDate = readDate("Arrival date");             // asks for arrival date
	depDate = readDate("Departure date");           // asks for departure date

	switch (roomType)         // roomType char, decides value of int roomTypes
	{
		case 'R': { roomTypes = 0; break; }
		case 'D': { roomTypes = 1; break; }
		case 'S': { roomTypes = 2; break; }
	}					// Could probably be solved better with the use of enum

	for (int i = 1, j = rooms[roomTypes]->no_of_elements()+1; i < j; i++)
	{
		elem = rooms[roomTypes]->remove_no(i);	// Remove from right category
		room = (Room*)elem;

		if (room->available(arrDate, depDate))
			room->display();
		
		rooms[roomTypes]->add(elem);
	}
}
											// Switch reservation to new room
void Hotel::doResSwitch(const int number, const void* pointer)	
{
	Room* room;								// New room

	room = getRoom(number);					// Get room from room#

	room->addReservation((Reservation*) pointer);	// Add the reservation to it

	cout << "\n\t\tThe reservation have been changed to this room:\n\n";

	room->display();							// Display new room
}