#include "Room.h"
#include "timer2.h"
#include "Reservation.h"
#include <fstream>
#include "Global.h"

extern Timer timer;

Room::Room() // Default constructor, ask user for member values
    : Num_element(readValue<int>("Room number", 0, 9001)),
      bedCount(readValue<int>("Amount of beds", 0, 9001)),
      breakfastIncluded(readChar("Breakfast included (y/n)") == 'y'),
      reservations(listtype::Sorted)
{ }

Room::Room(ifstream& file)    // Make new Room from records on file
    : Num_element(readValue<int>(file)),
      bedCount(readValue<int>(file)),
      breakfastIncluded(readChar(file) == 'y'),
      reservations(listtype::Sorted)
{ }

Room::~Room() // Deconstructor
{ }

void Room::display()    // Display all the data about the room
{
    cout << "\n\n\tRoom number: " << Num_element::number;
    cout << "\nCount of beds: " << bedCount;
    cout << "\nBreakfast included: " << (breakfastIncluded ? "yes" : "no");
}

void Room::writeToFile(ofstream& file)
{
    file << Num_element::number << endl;
    file << bedCount << ' ' << (breakfastIncluded ? 'y' : 'n') << endl;
}

void Room::writeReservations(ofstream& file)
{
    file << reservations.no_of_elements() << endl;
    Element* elem;        // Element we take in/out from list
    Reservation* res;    // elem casted to Reservation

                        // Loop over all the elements
    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);    // Take out the item
        res = (Reservation*)elem;    // Cast it
        res->writeToFile(file);                // Write data
        reservations.add(elem);        // Add it back to container
    }

}

void Room::readReservations(ifstream& file)
{
    int count;
    file >> count;

    for (int i = 0; i < count; i++)
        reservations.add(new Reservation(file));
}

void Room::displayReservations()    // Display all reservations on the room
{
    Element* elem;        // Element we take in/out from list
    Reservation* res;    // elem casted to Reservation

                        // Loop over all the elements
    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);    // Take out the item
        res = (Reservation*)elem;    // Cast it
        res->display();                // Display data
        reservations.add(elem);        // Add it back to container
    }
}

                                // Reservates the room in the given period
void Room::reservate(const int dateStart, const int dateEnd, 
                     RoomCost& costData, const RoomType type)
{                            // Make new reservation
    
    Reservation* res = new Reservation(dateStart, dateEnd, extraBed());
    res->setCost(costData, type);
    setExtraStatus(res->getExtraBedCount());
    reservations.add((Element*)res);    // Add to reservations
}

                            // Checks if the room is available in the period
bool Room::available(const int dateStart, const int dateEnd)
{
    Element* elem;
    Reservation* res;
    bool isAvailable = true;    // If any available room was found

    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);
        res = (Reservation*)elem;
        if (res->withinTimeSpan(dateStart, dateEnd))    // if occupied
            isAvailable = false;    // Set room to occupied

        reservations.add(elem);
    }

    return isAvailable;
}
                                // Returns true if reservator equals 'name'
bool Room::isReservator(const string& name)
{
    Element* elem;
    Reservation* res;
    bool found = false;    // If any users was found

    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);
        res = (Reservation*)elem;
        if (res->isVisitor(name))    // if name matches
            found = true;    // Set found

        reservations.add(elem);
    }

    return found;
}
                            // Tries to check in a person under a name and date
void Room::tryCheckIn(const string& name, const int date)
{
    Element* elem;        // Enumerate through the List
    Reservation* res;
    bool checkIn = false;

    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);
        res = (Reservation*)elem;

        if (res->canCheckin(name, date))    // If can check in here
        {
            cout << "\nCheckin on room " << number << "? ";
            if (toupper(readChar("(y/n)")) == 'Y')    // If wants to checkin
            {
                res->checkIn(extraBed());    // Check in
                cout << "\nChecked in on room number " << number << endl;
            
                i = j;        // Stop looping
            }
        }

        reservations.add(elem);
    }
}

void Room::cancelReservation(const string& name)
{
    Element* elem;        // Enumerate through the List
    Reservation* res;
    bool cancelReservation = false;    //If the user wanna cancel the room
                                    // Loop over each reservation
    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);    // Take out element
        res = (Reservation*)elem;    // cast it
        cout << "Reservator on room " << number << ": ";
        if (res->isReservator(name))    // If name is reservator
        {                                // Display data and ask user
            cout << "\nConfirm cancelation on room " << number << endl;
            cout << "Reservation data:\n";

            res->display();
            cout << "Cancel reservation? ";
            cancelReservation = toupper(readChar("(y/n)")) == 'Y';
        }

        if (cancelReservation)    // If wants to remove, 
        {
            cancelReservation = true;    // Don't put back element, and reset
            i--;
            j = reservations.no_of_elements() + 1;
            delete elem;
        }
        else
        {
            reservations.add(elem);    // If not, add back the reservation
        }
    }
}

void Room::changeArrivalDate(const string& name)
{
    Element* elem;        // Enumerate through the List
    Reservation* res;
    int date;            // Input date for arrival
                                    // Loop over each reservation
    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);    // Take out element
        res = (Reservation*)elem;    // cast it

        cout << "Current reservation details:\n";
        res->display();
        cout << "\nChange arrival date? ";
        if (toupper(readChar("(y/n)") == 'Y'))
        {
            do
                date = readDate("Arrival");
            while (!available(date, res->getDeparture())
                && timer.forskjell_datoer(date, res->getDeparture()) >= 0);

            res->setArrival(date);
        }
        reservations.add(elem);    //Add back the reservation
    }
}

void Room::changeDepartureDate(const string& name)
{
    Element* elem;        // Enumerate through the List
    Reservation* res;
    int date;
                                    // Loop over each reservation
    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++)
    {
        elem = reservations.remove_no(i);    // Take out element
        res = (Reservation*)elem;    // cast it
        
        cout << "Current reservation details:\n";
        res->display();
        cout << "\nChange departure date? ";
        if (toupper(readChar("(y/n)") == 'Y'))    // If change data
        {
            do
                date = readDate("Departure");    // Read date value
            while (!available(res->getArrival(), date)    // Loop untill valid
                && timer.forskjell_datoer(res->getArrival(), date) >= 0);

            res->setArrival(date);
        }
        reservations.add(elem);    // Add back the reservation
    }
}

bool Room::operator==(const int roomNumber)
{
    return number == roomNumber;
}

void Room::roomCheckOut(const string& name)
{
    ofstream file(name+".HST", fstream::app);
    Element* elem;                 // Enumerate through the list
    Reservation* res;
    int date = timer.hent_dato();  // Gets the current date
    bool checkOut = false;      // Bool to check if checking out
                                // Loops through all reservations
    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++) 
    {                     
        bool checkOut = false;
        elem = reservations.remove_no(i);
        res  = (Reservation*)elem;            // Take out element

        if (res->getDeparture() == date)
        {
            cout << "Reservation details of the room you'd like to check out.\n";
            res->display();
            cout << "Are you sure you want to check out?\n";
            if (toupper(readChar("(y/n)")) == 'Y')    // If user wants to check out.
            { 
                checkOut = true;            // Change value of bool checkOut
                res->writeToFile(file);    // write reservation data on .HST file
            }        
        }
        if (!checkOut)    // if not checking out add reservation back to the list
            reservations.add(elem);
        else
            delete elem;                    // If not, remove it from memory
    }
}

void Room::displayReservationsDetails()
{
    Element* elem = reservations.remove_no(1);    // Get reservation
    Reservation* res;            
    reservations.add(elem);

    if (elem == nullptr)                        // Show msg if not found
    {
        cout << "No reservation found\n";
    }
    else
    {
        res = (Reservation*)elem;
        res->displayDetails();                    // Show msg if not null
    }
}

void Room::displayAvailabillity()
{
    Element* elem;                    // Enumerate through the list
    Reservation* res;
    int start = timer.hent_dato();    // Start of 'free' period
    int end;                        // End of 'free' perid
    int periodLength;                // Length between 'start' and 'end'

    for (int i = 1, j = reservations.no_of_elements() + 1; i < j; i++) 
    {
        elem = reservations.remove_no(i);    // Get and cast elem from list
        res  = (Reservation*)elem;        

        end = res->getArrival();            // Get end period date

        if (end != start)            // end and start is not on same date
            cout << "\t" << start << "-->" << end << endl;

        start = res->getDeparture();        // Get start period date
        reservations.add(elem);                // Add back
    }
                                            // Show last date
    cout << "\t" << start << "-->Forever\n";
}

int Room::customerReservation(string name)    // Check reservations of given cust
{
    Reservation* pointer;                    // New reservation
    int numReservations, i, cusRes = 0;
    List custRes(listtype::FIFO);            // List of reservations

    numReservations = reservations.no_of_elements();    // Get num of res.

    for (i = 1; i <= numReservations; i++)    // Loop all reservations
    {
        pointer = (Reservation*) reservations.remove_no(i);    // Get res

        if (pointer->nameCheck(name))        // Check if name mach
            custRes.add(pointer);            // Add to temp list

        reservations.add(pointer);            // Add res back to normal list
    }

    cusRes = custRes.no_of_elements();        // Check reservations for this cust

    if (cusRes)                                // If had res in room
    {
        Room::display();                    // Display cur. room
        for (i = 0; i < cusRes; i++)        // Loop all res. to this customer
        {                                    // Remove it from temp list
            pointer = (Reservation*) custRes.remove();    
        
            pointer->display();                // Display this res.
        }
    }

    return cusRes;                            // Return number of res.
}

void Room::reservationSwitch(string name, RoomType type)
{
    int numRes, i, room;            // Int I'm going to use
    Reservation* pointer;            // Reservation I'm going to use
    char input;                        // Input char value.
    bool switchedRooms;                // if switch is done
                                    // Number of reservations on this room
    numRes = reservations.no_of_elements();    

    for (i = 0; i < numRes; i++)        // Loop all reservations
    {
        pointer = (Reservation*) reservations.remove_no(i);    // Get cur. res.
        switchedRooms = false;

        if (pointer->nameCheck(name))        // Check if it's for this customer
        {
            Room::display();                // Display current room
            pointer->display();                // Display current reservation
            input = 
                toupper(            // Ask user if he want to change reservation
                readChar("Do you wish to change this reservation? ((Y)es/(N)o)")
                );

            if (input == 'Y')                // Do so if yes
            {
                room = newRoom(type, pointer->arrivalDate(), 
                                pointer->arrivalDate(false), number);
                if (room)                            // If we found a new room
                {    
                    displayResSwitch(room, pointer);    // Do reservation switch
                    switchedRooms = true;
                }
                else
                {
                    cout << 
                    "\n\t\tSorry no other room is available"
                    << "in this time period.\n\n";
                                // Add reservation back in current room
                }
            }
        }
        if (!switchedRooms)
            reservations.add(pointer);    // Add reservation back in current room
    }
}

void Room::addReservation(Reservation* pointer)    // Add a reservation from ptr
{
    reservations.add(pointer);
}

void Room::roomRegisterBill()
{
    Element* elem;                 // Enumerate through the list
    Reservation* res;
    int date = timer.hent_dato();  // Gets the current date

    if (reservations.no_of_elements() == 0)
        cout << "No reservations on this room at the time." << endl;
    else
    {                     
        elem = reservations.remove_no(1);    // Get and cast elem from list
        res  = (Reservation*)elem;    

        if (res->isCheckedIn())
            res->registerInvoice();
        else
            cout << "Room is not occupied at the time. ";
        reservations.add(elem);
    }

}

int Room::getRoomNumber() const    // Return room# of room
{
    return number;
}