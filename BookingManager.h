
#pragma once
#include "IntervalTree.h"
#include <map>
#include <string>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

class BookingManager
{
private:
    map<string, IntervalTree> roomBookings;

public:
    bool bookRoom(const string &room, int start, int end)
    {
        if (start < 0 || start > 23 || end < 0 || end > 23 || end <= start)
        {
            return false;
        }
        if (roomBookings[room].overlaps(start, end))
        {
            return false;
        }
        roomBookings[room].insert(start, end);
        return true;
    }

    bool isBooked(const string &room)
    {
        return roomBookings.find(room) != roomBookings.end() && roomBookings[room].hasBookings();
    }

    bool cancelBooking(const string &room, int start, int end)
    {
        if (roomBookings.find(room) == roomBookings.end() || !roomBookings[room].hasBookings())
            return false;

        // First check if exact slot exists before removing
        json intervals = roomBookings[room].getIntervals();
        bool found = false;
        for (const auto &interval : intervals)
        {
            if (interval["start"] == start && interval["end"] == end)
            {
                found = true;
                break;
            }
        }

        if (!found)
            return false;

        roomBookings[room].remove(start, end);
        return true;
    }

    json getAllBookings()
    {
        json bookings;
        for (const auto &pair : roomBookings)
        {
            if (pair.second.hasBookings())
            {
                bookings[pair.first] = pair.second.getIntervals();
            }
        }
        return bookings;
    }

    json processJsonRequest(const json &request, const vector<string> &rooms, vector<string> &booked)
    {
        json response;
        response["success"] = false;

        if (!request.contains("action"))
        {
            response["error"] = "No action specified";
            return response;
        }

        string action = request["action"].get<string>();

        if (action == "book")
        {
            if (!request.contains("room") || !request.contains("start") || !request.contains("end"))
            {
                response["error"] = "Missing required fields";
                return response;
            }

            string room = request["room"].get<string>();
            int start = request["start"].get<int>();
            int end = request["end"].get<int>();

            if (find(rooms.begin(), rooms.end(), room) == rooms.end())
            {
                response["error"] = "Invalid room";
                return response;
            }

            if (bookRoom(room, start, end))
            {
                response["success"] = true;
                response["message"] = "Room booked successfully";
                booked.push_back(room);
            }
            else
            {
                response["error"] = "Booking conflict or invalid time";
            }
        }

        else if (action == "get_bookings")
        {
            response["success"] = true;
            response["bookings"] = getAllBookings();
        }

        else if (action == "cancel")
        {
            if (!request.contains("room") || !request.contains("start") || !request.contains("end"))
            {
                response["error"] = "Enter slot to cancel";
                return response;
            }

            std::string room = request["room"].get<std::string>();
            int start = request["start"].get<int>();
            int end = request["end"].get<int>();

            if (cancelBooking(room, start, end))
            {
                response["success"] = true;
                response["message"] = "Slot cancelled successfully";
            }
            else
            {
                response["error"] = "Slot not found or invalid cancel attempt";
            }
        }
        else
        {
            response["error"] = "Invalid action";
        }

        return response;
    }
};