//Contains main and all global values

#include "Global.h"
#include "Hotel.h"
#include "timer2.h"


#include "RoomCost.h"

Hotel* curHotel;
Timer timer;

int main()
{

    getHotel();

    //char k = readChar("mordi");

    bool exit = false;        // If the user has exited the program
    do
    {
        switch (toupper(readChar("Command")))    //Read command input
        {
        case 'B':    // Book room
            {
                if (curHotel != nullptr)
                    curHotel->bookRoom();
                break;
            }
        case 'A':    // Cancel room booking
            {
                if (curHotel != nullptr)
                    curHotel->cancelBooking();
                break;
            }
        case 'I':    // Check in
            {
                if (curHotel != nullptr)
                    curHotel->checkIn(readLine("Reservator", ReadFlag::ToUpper));
                break;
            }
        case 'U':    // Check out
            {
                if (curHotel != nullptr)
                    curHotel->checkOut(readValue<int>("Room nr: ", 0, 999));
                break;
            }
        case 'R':    // Register bill on a room
            {
                if (curHotel != nullptr)
                    curHotel->registerBill(readValue<int>("Room nr: ", 0, 999));
                break;
            }
        case 'E':    // Booking editing commands
            {
                switch (readChar(""))    // Read 2nd command char
                {
                case '1':    // Change booking arrival date
                    {
                        if (curHotel != nullptr)
                            curHotel->changeArrivalDate();
                        break;
                    }
                case '2':    // Change booking departure date
                    {
                        if (curHotel != nullptr)
                            curHotel->changeDepartureDate();
                        break;
                    }
                case '3':    // Change room
                    {
                        curHotel->switchReservation(); break;
                    }
                default:    // Unknown command
                    cout << "Unknown command, press ? for command overview\n";
                    break;
                }
                break;
            }
        case 'O':    // Info commands
            {
                switch (readChar(""))    // Read 2nd command char
                {
                case '1':    // Show hotel data
                    {
                        if (curHotel != nullptr)
                            curHotel->display(); break;
                    }
                case '2':    // Show description of all suites
                    {
                        if (curHotel != nullptr)
                            curHotel->displaySuites(); break;
                    }
                case '3':    // Show reservation for an entered name
                    {
                        if (curHotel != nullptr)
                            curHotel->customerReservations(); break;
                    }
                case '4':    // Show when a room is available
                    {
                        if (curHotel != nullptr)
                            curHotel->displayRoomAvailabillity();
                        break;
                    }
                case '5':    // Show all reservations on a room
                    {
                        if (curHotel != nullptr)
                            curHotel->displayRoomReservations();
                        break;
                    }
                case '6':    // Show data for a room reservator + bills
                    {
                        if (curHotel != nullptr)
                            curHotel->displayCurrentRoomReservationDetails();
                        break;
                    }
                            // Show all available rooms under a given category
                case '7':    // in a given timespan
                    {
                        if (curHotel != nullptr)
                            curHotel->displayAllRoomsInACategoryAndTimespan();
                        break;
                    }
                default:    // Unknown command
                    cout << "Unknown command, press ? for command overview\n";
                    break;
                }
                break;
            }
        case 'T':    // Save hotel data
            {
                writeHotel(); break;
            }
        case 'H':    // Change hotel
            {
                writeHotel(); getHotel(); break;
            }
        case '?':    // Show commands
            {
                displayCommands();
                break;
            }
        case 'Q':    // Exit program
            {
                exit = true;
                break;
            }
        default:    // Unknown command
            cout << "Unknown command, press ? for command overview\n";
            break;
        }
    }
    while (!exit);    // Loop as long as user doesn't exit

    return 0;
}
