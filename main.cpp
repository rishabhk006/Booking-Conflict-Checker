// File: src/main.cpp
#include "BookingManager.h"
#include "RoomMap.h"
#include <json.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

int main()
{
    BookingManager manager;
    vector<string> rooms = {
        "A1", "A2", "A3",
        "B1", "B2", "B3",
        "C1", "C2", "C3",
        "D1", "D2", "D3",
        "E1", "E2", "E3"};
    vector<string> booked;

    cout << "Welcome to the Room Booking System!\n";

    while (true)
    {
        displayRooms(rooms, booked);
        cout << "\nOptions:\n";
        cout << "1. Book a room\n";
        cout << "2. Cancel booking\n";
        cout << "3. View bookings\n";
        cout << "4. Process JSON request\n";
        cout << "5. Exit\n";
        cout << "Enter choice (1-5): ";

        int choice;
        cin >> choice;
        cin.ignore(); // Clear buffer

        if (choice == 5)
            break;

        switch (choice)
        {
        case 1:
        {
            string room, input;
            int start, end;

            cout << "\nEnter room no. (e.g., A1): ";
            getline(cin, room);

            // Validate room
            if (find(rooms.begin(), rooms.end(), room) == rooms.end())
            {
                cout << "Invalid room number!\n";
                continue;
            }

            cout << "Enter check-in time (0-23): ";
            getline(cin, input);
            try
            {
                start = stoi(input);
                if (start < 0 || start > 23)
                    throw out_of_range("Invalid time");
            }
            catch (...)
            {
                cout << "Invalid check-in time!\n";
                continue;
            }

            cout << "Enter check-out time (0-23): ";
            getline(cin, input);
            try
            {
                end = stoi(input);
                if (end < 0 || end > 23 || end <= start)
                    throw out_of_range("Invalid time");
            }
            catch (...)
            {
                cout << "Invalid check-out time!\n";
                continue;
            }

            if (manager.bookRoom(room, start, end))
            {
                cout << "Room booked successfully!\n";
                booked.push_back(room);
            }
            else
            {
                cout << "Conflict: Room already booked in that time range.\n";
            }
            break;
        }
        case 2:
        {
            string room, input;
            int start, end;

            cout << "\nEnter room to cancel booking (e.g., A1): ";
            getline(cin, room);

            // Validate room
            if (find(rooms.begin(), rooms.end(), room) == rooms.end())
            {
                cout << "Invalid room number!\n";
                break;
            }

            cout << "Enter check-in time of the slot to cancel (0-23): ";
            getline(cin, input);
            try
            {
                start = stoi(input);
                if (start < 0 || start > 23)
                    throw out_of_range("Invalid time");
            }
            catch (...)
            {
                cout << "Invalid check-in time!\n";
                break;
            }

            cout << "Enter check-out time of the slot to cancel (0-23): ";
            getline(cin, input);
            try
            {
                end = stoi(input);
                if (end < 0 || end > 23 || end <= start)
                    throw out_of_range("Invalid time");
            }
            catch (...)
            {
                cout << "Invalid check-out time!\n";
                break;
            }

            if (manager.cancelBooking(room, start, end))
            {
                cout << "Slot cancelled successfully!\n";

                // Remove from booked list if no slots remain
                if (!manager.getAllBookings()[room].size())
                {
                    booked.erase(remove(booked.begin(), booked.end(), room), booked.end());
                }
            }
            else
            {
                cout << "No such booking slot found!\n";
            }
            break;
        }
        case 3:
        {
            cout << "\nCurrent Bookings:\n";
            json bookings = manager.getAllBookings();
            cout << bookings.dump(2) << "\n";
            break;
        }
        case 4:
        {
            cout << "\nEnter JSON request: ";
            string json_input;
            getline(cin, json_input);

            try
            {
                json request = json::parse(json_input);
                json response = manager.processJsonRequest(request, rooms, booked);
                cout << "Response: " << response.dump(2) << "\n";
            }
            catch (const json::exception &e)
            {
                cout << "Invalid JSON: " << e.what() << "\n";
            }
            break;
        }
        default:
            cout << "Invalid choice!\n";
        }
    }

    cout << "Thanks for using the booking system!\n";
    return 0;
}