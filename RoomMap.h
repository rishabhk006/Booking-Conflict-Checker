// File: src/RoomMap.h
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void displayRooms(const vector<string>& rooms, const vector<string>& booked) {
    cout << "\nAvailable Rooms:\n";
    for (int i = 0; i < rooms.size(); i++) {
        string room = rooms[i];
        bool isBooked = find(booked.begin(), booked.end(), room) != booked.end();

        if (isBooked) {
            cout << "[XX] ";
        } else {
            cout << "[" << room << "] ";
        }

        if ((i + 1) % 3 == 0) cout << "\n";
    }
}