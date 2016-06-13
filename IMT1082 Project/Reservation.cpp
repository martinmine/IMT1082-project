#include "Reservation.h"
#include "RoomCost.h"
#include <fstream>
#include <iostream>
#include "timer2.h"

extern Timer timer;

Reservation::Reservation(const int arrival, const int departure, const int mxBed)
    : Num_element(arrival),
      departureDate(departure),
      duration(timer.forskjell_datoer(arrival, departure) + 1),
      visitorCount(1),
      cost(new float[duration]), 
      visitors(new string[1]),
      invoice(listtype::FIFO),
      checkedIn(false)
{
    readLine(visitors[0], "Reservator name");
    upperString(visitors[0]);

    if (mxBed)
        extraBedStatus = readValue<int>("Extra beds", 0, mxBed);
    else
        extraBedStatus = 0;
}

Reservation::Reservation(ifstream& file) // cctor that reads data from file
    : Num_element(readValue<int>(file)),
      departureDate(readValue<int>(file)),
      duration(readValue<int>(file)),
      extraBedStatus(readValue<int>(file)),
      visitorCount(readValue<int>(file)),
      checkedIn(readValue<char>(file) == 'C'),
      cost(new float[duration]), 
      visitors(new string[visitorCount]),
      invoice(listtype::FIFO)
{
    for (int i = 0; i < duration; i++)
        cost[i] = readValue<float>(file);

    file.ignore();
    for (int i = 0; i < visitorCount; i++)
    {
        readLine(file, visitors[i]);
    }

    int invoiceCount;
    file >> invoiceCount;
    file.ignore();

    for (int i = 0; i < invoiceCount; i++)
        invoice.add(new InvoiceElement(file));
}

void Reservation::writeToFile(ofstream& file) // write all data to file
{
    file << number << ' ' << departureDate << endl
         << duration << ' ' << extraBedStatus << endl
         << visitorCount <<  ' '  << (checkedIn ? 'C' : 'B') << endl;

    for (int i = 0; i < duration; i++)
        file << cost[i] << ' ';
    file << endl;

    for (int i = 0; i < visitorCount; i++)    // Write all visitors
        file << visitors[i] << endl;

    file << invoice.no_of_elements() << endl;    // Write count of invoices

    for (int i = 1, j = invoice.no_of_elements() + 1; i < j; i++)
    {
        Element* elem = invoice.remove();            // Enumerate through all
        ((InvoiceElement*)elem)->writeToFile(file);    // and write to file
        invoice.add(elem);
    }
}

Reservation::~Reservation() // Deconstructor
{
    delete[] visitors;
    delete[] cost;
}

void Reservation::display() // Displays all data
{
    cout << "Arrival date: " << number << endl
         << "Departure date: " << departureDate << endl
         << "Duration: " << duration << " days" << endl
         << "Extra beds: " << extraBedStatus << endl
         << "Amount of visitors: " << visitorCount << endl
         << "Visitors:\n";
    
    for (int i = 0; i < visitorCount; i++)
        cout << '\t' << visitors[i] << endl;

    cout << "\nCosts:\n";
    for (int i = 0; i < duration; i++)
        cout << "\tDay " << (i + 1) << ": " << cost[i] << endl;
}

                            // Returns if room is free in 'start'->'end' period
bool Reservation::withinTimeSpan(const int dateStart, const int dateEnd)
{    
                            // If current reservation is before 'dateStart' 
                            // AND the 'dateEnd' is before departure date
    return number <= dateStart && dateEnd >= departureDate;
}

void Reservation::registerInvoice()
{

    InvoiceElement* invo;
    invo = new InvoiceElement;
    invoice.add(invo);
}

bool Reservation::isVisitor(const string& name)
{
    int i = 0;                    // Counter
    while (i < visitorCount)    // While not found and within range
    {
        if (visitors[i++] == name)    // If equals
            return true;            // return true
    }
}

bool Reservation::canCheckin(const string& res, const int date)
{                        // Return true if same data, and res = reservator
    if (checkedIn) return false;
    else return (number == date && res == visitors[0]);
}

void Reservation::checkIn(const int maxVisitors)
{
    int newCount = 0;
    if (maxVisitors) // Ask how many visitors there will be
        newCount = 
    readValue<int>("Amount of visitors except the reservator", 0, maxVisitors);

    string* newVisitors = new string[newCount + 1];
    newVisitors[0] = visitors[0];

    delete[] visitors;
    visitors = newVisitors;
    visitorCount = newCount + 1;
                        // and read their names
    for (int i = 1; i < visitorCount; i++)
        readLine(visitors[i], "Visitor name");
    checkedIn = true;
}
                                        // Return true if res is reservator
bool Reservation::isReservator(const string& res)    
{
    cout << visitors[0];
    return visitors[0] == res;
}

void Reservation::setArrival(const int date)
{
    number = date;
}

void Reservation::setDeparture(const int date)
{
    departureDate = date;
}

int Reservation::getDeparture() const
{
    return departureDate;
}

int Reservation::getArrival() const
{
    return number;
}

void Reservation::displayDetails()
{
    display();
    Element* elem;                // Enumerate through the list
    InvoiceElement* bill;
    
    for (int i = 0, j = invoice.no_of_elements(); i < j; i++) 
    {
        elem = invoice.remove();
        bill = (InvoiceElement*)elem;
        bill->display();        // Call display on the invoice

        invoice.add(elem);
    }
}

bool Reservation::nameCheck(const string& name)    // Check if res mach name
{
    int i;

    for (i = 0; i < visitorCount; i++)    // List all visitors
    {
        if (visitors[i] == name)        // Check if it mach a visitor
            return true;                
    }

    return false;
}

int Reservation::arrivalDate(bool arrival)
{
    if (arrival)
        return number;
    else
        return departureDate;
}

void Reservation::setCost(RoomCost& costData, const RoomType type)
{
    costData.setCost(cost, number, departureDate, type);
}