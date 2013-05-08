#if !defined(__ROOM_H)
#define  __ROOM_H

#include "Global.h"
#include "listtool2.h"
#include "Reservation.h"
//#include "Hotel.h"

class Room : public Num_element
{
private:
	int bedCount;           // Amount of beds in the room
	bool breakfastIncluded; // If cost of breakfast is included with room
	List reservations;      // List of reservations()
public:
	Room();					// Default constructor
	Room(ifstream& file);
	virtual ~Room();		// Deconstructor
	virtual void display();		// Display the room data
	virtual void writeToFile(ofstream& file);	//Write room data to file
	void writeReservations(ofstream& file);	// Writes reservations to file
	void readReservations(ifstream& file);	// Read reservations to file
	void displayReservations();	// Calls display on each reservation
								// Reservates a room in the given time period
	void reservate(const int dateStart, const int dateEnd, 
				RoomCost& costData, const RoomType type);
								// Returns true if a room is available within
								// the given time period, otherwise false
	bool available(const int dateStart, const int dateEnd);
	bool isReservator(const string& name);	// Returns if name equals reservator
								// Checks if anyone with the given name
								// on the given date has made a reservation
								// and checks in the person
	void tryCheckIn(const string& name, const int date);
	void cancelReservation(const string& name);	// Cancels the reservation
						// Change arrival date on rooms which 'name' has booked
	void changeArrivalDate(const string& name);	
						// Change dep. date on rooms which 'name' has booked
	void changeDepartureDate(const string& name);

	bool operator==(const int roomNumber);	// Return if equal to room number
	void roomCheckOut(const string& name);  // checks out from a reservation
	void displayReservationsDetails();	// Show current reservator + bills
	void displayAvailabillity();	// Show when room is available
						// Find reservations of given customer
	int customerReservation(string name);	
	void reservationSwitch(string name, RoomType type);	// Switch a reservation
	void roomRegisterBill();
	int newReservation(string name, const int arrival, const int departure);
											// Add a reservation from pointer
	void addReservation(Reservation* pointer);	// Adds reservation
	int getRoomNumber() const;	// Returns the room number
	virtual int extraBed() const = 0;
						// Sets the altered bed count if any
	virtual void setExtraStatus(const int newCount) { };
};


#endif