// This file contains global function declarations used in the program.
// Note: ALL definitions of the functions will be in 'Global.cpp'
//       as stated in the assignment papers (except template functions)

#if !defined(__GLOBAL_H)
#define  __GLOBAL_H

#include <string>
#include <iostream>
#include <fstream>
#include "EnumConst.h"
#include "RoomCost.h"

using namespace std;


                                // Read a string from given filestream into src
void readLine(ifstream& file, string& src);    

template <typename T>
T readValue(ifstream & file);

char readChar(const string & opt);      // Reads a char from user.
char readChar(ifstream& file);            // Reads the next char from 'file'
                                // Returns a string which is what the user
                                // typed on the console after 'opt' was 
                                // displayed, if user input is empty, it will
                                // return defaultVal
string readLine(const string & opt, const string & defaultVal); // else the read string
                                // Returns a string which is what the user
                                // typed on the console after 'opt' was 
                                // displayed
string readLine(const string & opt, const ReadFlag flag = ReadFlag::None);
                                // Reads the user input after displaying 
                                // 'opt' on console into 'src', if empty,
                                // src will be set to defautlVal
void readLine(string & src, const string & opt, const string & defaultVal);
                                // Reads the user input after displaying
                                // 'opt' on console into 'src'
void readLine(string & src, const string & opt);
bool isNumber(const string& w);    // Returns true if w is number


string monthName(int month, bool small = false);    // Return navn of month
void upperString(string& text);            // Convert a string to uppercase
void lowerString(string& text);            // Convert a string to lowercase
void displayCommands();                    // Display all available commands
void trim(string& str);                    // Trim a string
void getHotel();                        // Get filename from given hotelname
void writeHotel();                        // Writes current hotel to file
int readDate(const string& opt);        // Reads a date value from console input
bool isFirst(const int d1, const int d2);    // Returns true if d1 is first
int newRoom(RoomType type, const int arrival,        // Return room# of new room
            const int depature, const int notRoom);
void displayResSwitch(const int room, const void* pointer);

template <typename T>
T readValue(ifstream & file)            // Read a number from given filestream.
{
    T number;                            
    file >> number;                        // Read number from file
    file.ignore();                        // Jump to next line
    return number;                        // Return the number
}
                            // Returns a RoomCost object from the .PRS object
                            // where 'hotel' indicates the name
RoomCost getRoomCost(const string& hotel);
                            // Reads a value with type T from console input, 
                            // has to be within range <MIN, MAX> 
template <typename T>        // and prints 'opt'
T readValue(const string & opt, const T MIN, const T MAX)
{
    string buffer;            // Buffer/temp string
    T       number = 0;        // Return value
    bool   isNumber = true;    // If 'number' is a number
    do
    {
        isNumber = true;
        cout << '\n' << opt << " (" << MIN << '-' << MAX << "): ";
        getline (cin, buffer);   // Reads a string array.

        for (int i = 0, j = buffer.length(); i < j; i++)
        {                           // Checks if array only contains numbers.
                                    // If is digit, and if number is a float,
                                    // check if has comma
            if (!isdigit(buffer[i]))
                if (buffer[i] != '.' && typeid(number) != typeid(float))
                    isNumber = false;   // If string contains invalid number.            
        }
                                        //Convert string to appropriate value
        number = typeid(number) == typeid(float) ? stof(buffer) : stoi(buffer);
    }                            // While int is outside allowed range, loop.
    while (number < MIN || number > MAX || !isNumber || buffer.length() == 0);
    return number;                      // Returns number.
}

#endif