//Contains definition of all global functions

#include "Global.h"
#include "Hotel.h"
#include "EnumConst.h"
#include <fstream>
#include <algorithm>                    // transform(), string to uppercase
#include "timer2.h"

extern Hotel* curHotel;
extern Timer timer;


void readLine(ifstream& file, string& src)
{
    getline (file, src);            // Get string from file
}

void readLine(string& src, const string& opt, const string& defaultVal)
{
    readLine(opt, src);                // Read the line
    if (src.empty())                // if no input,
        src = defaultVal;            // set to default value
}

void readLine(string& src, const string& opt)
{
    if (!opt.empty())                    // If anything to display,
        cout << '\n' << opt << ": ";    // display it
    getline(cin, src);                    // get user input
}

string readLine(const string& opt, const string&  defaultVal)
{
    string buffer = readLine(opt);        // Read line
    if (buffer.empty())                    // if input was empty
        buffer = defaultVal;            // set default value
    return buffer;                        // return the input
}

string readLine(const string& opt = "", const ReadFlag flag)
{
    string buffer;                        // Value to read into
    readLine(buffer, opt);                // read the line

    if (flag & ReadFlag::Trim)            // Trim if trim flag was set
        trim(buffer);
    if (flag & ReadFlag::ToUpper)        // Make toupper if toupper flag was set
        upperString(buffer);
    else if (flag & ReadFlag::ToLower)    // Make tolower if tolower flag was set
        lowerString(buffer);

    return buffer;                        // return the input
}

char readChar(const string & opt = "")    // Reads a character from user.
{
    char character;
    if (!opt.empty())
        cout << '\n' << opt << ": ";    // Outputs optional text
    cin  >> character;                  // Gets character from user
    cin.ignore();
    return character;                   // Returns character.
}


char readChar(ifstream& file)
{
    char character;
    file >> character;
    file.ignore();
    return character;
}

string monthName(int month, bool small)    // Return navn of month
{                                            // List of full monthnames
    string names[12] = {    "January", "February", "March", "April", 
                            "May", "June", "July", "August", "September", 
                            "October", "November", "December"    };
                                            // List of short month names.
    string smallNames[12] = {    "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"    };

    if ( ! ( month >= 1 && month <= 12 ) )    // Check if input month is valig
        return "NA";                        // Return NA if not

    return (small) ? smallNames[month-1] : names[month-1];    // Return month name
}

void upperString(string& str)                // Conv. a string to uppercase
{                        
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void lowerString(string& str)                // Conv. a string to lowercase
{                        
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void displayCommands()    // Display all available commands
{
    cout << "Available commands:\n";
    cout << "\tB - Book room\n";
    cout << "\tA - Cancel room booking\n";
    cout << "\tI - Check in\n";
    cout << "\tU - Check out\n";
    cout << "\tR - Register bill on a room\n\n";
    cout << "\tBooking edit commands:\n";
    cout << "\t E 1 - Display hotel details\n";
    cout << "\t E 2 - Change checked in departure date\n";
    cout << "\t E 3 - Change room\n\n";
    cout << "\tInformation commands:\n";
    cout << "\t O 1 - Hotel data\n";
    cout << "\t O 2 - Description of all suites\n";
    cout << "\t O 3 - All reservations for entered name\n";
    cout << "\t O 4 - When an entered room number is available\n";
    cout << "\t O 5 - All reservations for a specific room\n";
    cout << "\t O 6 - All data for one room visitor including bills\n";
    cout << "\t O 7 - All available rooms under a category ";
    cout << "in a given time period\n\n";
    cout << "\tT - Save hotel information\n";
    cout << "\tH - Change hotel\n";
    cout << "\t? - Command list\n";
    cout << "\tQ - Quit\n\n";
}

void trim(string& str)        // Trim string, remove spaces infront and after text
{
    remove (str.begin(), str.end(), ' ');
}

void getHotel()                            // Load hotel to memory from given name
{
    string hotelName = "NA";            // Default hotel name is "Not Available"
    string fileName;                        // Get filename of hotel
    string name;                            // Get userinput name
    bool hotelFound = false;

    while (!hotelFound)
    {
        name = readLine("Please enter name of hotel");    // Get hotelname from user
        //trim(name);                                // Trim the name

        if (name.length())                        // Check if any name
        {
            ifstream file("HOTELS.dta");                // Open list of all hotels

            upperString(name);                    // Convert name to upperstring

            while (!file.eof() && hotelName != name)    // Check every name i file
            {
                getline(file, fileName, ' ');        // Get the hotel short name
                getline (file, hotelName);            // Get the hotel long name
            }

            file.close();                        // Close the file

            if (hotelName == name)                // Check if hotelnames match
            {
                ifstream hotel(fileName + ".DTA");    // Open correct hotelfile
                curHotel = new Hotel(hotel);        // Read new hotel from file
                hotel.close();                        // Close hotelfile

                cout << "\n\n\t\tHotel successfully loaded to memory!\n\n";
                hotelFound = true;
            }
            else
            {
                cout << "\n\n\t\tThis hotel was not found!\n\n";
            }
        }
        else
        {
            cout << "\n\n\t\tNo valid name was entered!\n\n";
        }
    }
}

int readDate(const string& opt)    // Reads a date value from the console
{
    int date;
    string buffer;

    do
    {
        cout << opt << " (YYYYMMDD): ";
        getline(cin, buffer);

        if (buffer.empty())
            return timer.hent_dato();

        date = stoi(buffer);
    }
    while (!timer.dagnummer(date) || date < timer.hent_dato());

    return date;
}

void writeHotel()
{
    curHotel->write();                // Write hotel to file

    cout << "\n\n\n\t\tHotel was written to file!\n\n";
}

RoomCost getRoomCost(const string& filename)
{
    ifstream file(filename);    // Open input file stream to file
    if (file)                    // if it finds it
        return RoomCost(file);    // read its data
    else                        // if not, return a default one
        return RoomCost();    
}

                        // Get roomnumber from given infromation
int newRoom(RoomType type, const int arrival, 
            const int depature, const int notRoom)
{
    if (curHotel->getFreeRoom(type, arrival, depature, notRoom) == nullptr)
        return 0;
    else
        return curHotel->getFreeRoom(type, arrival, 
                                    depature, notRoom)->getRoomNumber();
}
                                                    // Switch res. to new room
void displayResSwitch(const int room, const void* pointer)
{
    curHotel->doResSwitch(room, pointer);            // Switch res. with room given
}

bool isNumber(const string& w)
{
    if (w.empty())
        return false;

    for (int i = 0, j = w.length(); i < j; i++)
        if (!isdigit(w[i]))
            return false;
    return true;
}