#if !defined(__HOTEL_H)
#define  __HOTEL_H

#include <string>
using namespace std;

#include "EnumConst.h"
#include "listtool2.h"
#include "Global.h"
#include "Room.h"

class Hotel
{
private:
	string  name;			// Name of the hotel
	string  streetAddress;	// Streed address of hotel
	string  fileName;		// File name where data is stored
	string  postalAddress;	// Postal code + postal address
	string  email;			// Email address to the hotel

	int     phoneNumber;	// Phone number to the hotel
	int     faxNumber;		// Fax number to the hotel
	int     breakfastCost;	// How much breakfast costs 
	int     extrabedCost;	// The cost for an extra bed
	int     facilityCount;	// Length of 'facilities'
	int		highestRoomNum;	// Highest room number

	string* facilities;			// Names of each faicility in the hotel
	List*   rooms[ROOM_TYPES];	// Sorted lists of all the room by room number

	void checkIn(const string& res,	// Checks in a reservator with the name res
				const RoomType type,	// and which category the room is in
				const int date);	// and which date
public:
	Hotel(ifstream & file);		// Default constructor
	~Hotel();					// Deconstructor

	template<class T>
	inline void readRooms(ifstream& file, List* storage);
	void display();				// Display hotel infromation
	void write();				// Write hotel to file
	void bookRoom();			// Books a room
	void cancelReservation();	// Cancels a reservation
								// Cancels a reservation for 
								// all rooms with type = 'type
	void cancelReservation(const RoomType type, const string& name);
	void printAllRooms();		// Prints all rooms of all types
								// Prints all rooms in the given type cat.
	void printRooms(const RoomType type); 
								// Checks in a reservator with the name res
	void checkIn(const string& res);	
	void checkOut(const int roomnr);  // Checks out a reservator from a room.
	void cancelBooking();		// Cancels bookings for user room detauls
	void registerBill(const int roomnr);   // registers a bill.
	void changeDepartureDate();	// Change a specific booking's departure date
	void changeArrivalDate();	// Change a specific booking's arrival date
					// Returns room with the room number, if not found, null
	Room* getRoom(const int roomNumber);
	void displayRoomReservations();	// Displays all reservations for a room
								// Display data about room reservator + bills
	void displayCurrentRoomReservationDetails();
	void displayRoomAvailabillity();	// Displays when a room is available
	void displaySuites();				// Display hotels suites.
	void customerReservations();		// Display reservation of customer
	void switchReservation();			// Switch a customers reservation
	void displayAllRoomsInACategoryAndTimespan();
										// switch room on res.
	void doResSwitch(const int number, const void* pointer);	

								// Returns a room which is not occupied
								// in the given period, if no Room is free,
								// it returns a nullptr.
	Room* getFreeRoom(const RoomType type, const int startPeriod,
						const int endPeriod, const int notRoom = -1);
};

#endif