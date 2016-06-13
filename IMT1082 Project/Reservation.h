#if !defined(__RESERVATION_H)
#define  __RESERVATION_H

#include <string>
#include "Global.h"
#include "InvoiceElement.h"
#include "listtool2.h"
#include "RoomCost.h"
using namespace std;

class Reservation : public Num_element
{
private:
    int departureDate;  // Date of departure
    int duration;       // Duration of stay in days
    int extraBedStatus; // Status if reservation has extra bed
    int visitorCount;   // Amount of visitors
    bool checkedIn;

    float*  cost;       // Cost for each day, length = 'duration'
    string* visitors;   // Name of each visitors, length = 'visitorCount'
    List    invoice;   // list of innvoices.
    Reservation();        // Don't allow default ctor to be executed

public:
                        // Creates a reservation from 'start' date to
                        // 'end' date and reads data needed
    Reservation(const int start, const int end, const int maxBeds);
    Reservation(ifstream& file); // cctor that reads all data from a file
    ~Reservation();     // Deconstructor
    void display();     // Displays all data
    void writeToFile(ofstream& file); // writes all data to a file
                        // Checks if the reservation is in the given timespan
    bool withinTimeSpan(const int dateStart, const int dateEnd);
    void registerInvoice();
    bool isVisitor(const string& name);    // Returns true if reservator is name
                        // Checks in the reservator with name res if the
                        // arrival date equals 'date'
    bool canCheckin(const string& res, const int date);
    void checkIn(const int maxVisitors);    // Checks in (todo)
    bool isReservator(const string& res);    // Returns true if res is reservator
    void setArrival(const int date);    // Sets arrival date to 'date'
    void setDeparture(const int date);    // Sets departure date to 'date'
    int getDeparture() const;    // Returns departure date
    int getArrival() const;        // Returns arrival date
    void displayDetails();        // Displays reservation details + bills
    bool nameCheck(const string& name);    // Check if res. belong to name
    int arrivalDate(bool arrival = true); // Return arival or dep. date
    void setCost(RoomCost& costData, // Set the cost for the different days
                const RoomType type);
    int getExtraBedCount() const { return extraBedStatus; }
    bool isCheckedIn() const { return checkedIn; }
};
#endif