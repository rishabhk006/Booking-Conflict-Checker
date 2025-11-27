📌 Booking Conflict Checker using Interval Tree

A Room Booking System that uses an Interval Tree to efficiently detect and prevent booking conflicts.
Users can book rooms for specific time slots, cancel bookings, and view all current reservations in a clean and interactive UI.

🚀 Features

✔️ Book Rooms with time slots (check-in & check-out)

✔️ Automatically detects overlapping bookings using Interval Tree

✔️ Cancel existing bookings

✔️ View all current bookings in a clear visual format

✔️ Rooms displayed with color-coded availability

✔️ Fast and efficient conflict checking

✔️ Built with simple and lightweight technologies

🛠️ Tech Stack
Frontend

HTML5

Tailwind CSS

JavaScript

Axios (for API calls)

Backend

Node.js / Express (or mention your backend language)

Interval Tree Data Structure (custom implementation)

📂 Project Structure
/project-root
│
├── index.html          # Frontend UI
├── server.js           # Backend server (example)
├── intervalTree.js     # Interval tree logic
└── README.md           # Documentation

📸 Screenshots (Optional)

You can add screenshots using:

![Screenshot](images/screenshot1.png)

🧠 How It Works
⭐ Booking Logic

When a user tries to book a room:

The backend checks existing intervals for that room.

The interval tree searches for any overlap with the new booking.

If overlap exists → ❌ Booking rejected

If no overlap → ✔️ Booking added

⭐ Overlap Rule

Two intervals overlap if:

newStart < existingEnd  AND  newEnd > existingStart


Using an Interval Tree makes this search efficient (O(log n)).

⚙️ API Endpoints
POST /api
1. Book Room
{
  "action": "book",
  "room": "A1",
  "start": 10,
  "end": 12
}

2. Cancel Booking
{
  "action": "cancel",
  "room": "A1",
  "start": 10,
  "end": 12
}

3. Get All Bookings
{
  "action": "get_bookings"
}

💻 How to Run the Project
1. Clone the repository
git clone https://github.com/your-username/booking-conflict-checker.git

2. Install dependencies
npm install

3. Start the server
node server.js

4. Open the UI

Open index.html in your browser
(Or serve it with Live Server)

📈 Future Enhancements

Add user authentication

Add a complete calendar UI

Use a real database (MongoDB / SQLite)

Export bookings as PDF

Add notification system

Visualize interval tree structure

🤝 Contributions

Pull requests are welcome!
For major changes, please open an issue first.

📄 License

This project is licensed under the MIT License.
