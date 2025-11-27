// File: src/server.cpp
#include "../include/httplib.h"
#include "../src/BookingManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

using namespace std;
using namespace httplib;
namespace fs = std::filesystem;

BookingManager manager;
vector<string> rooms = {
    "A1","A2","A3",
    "B1","B2","B3",
    "C1","C2","C3",
    "D1","D2","D3",
    "E1","E2","E3"
};
vector<string> booked;

// Read file contents safely; returns empty string if file missing
string readFile(const string& path) {
    ifstream file(path, ios::binary);
    if (!file.is_open()) return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Very small mime mapping for static files
string contentTypeForExt(const string& ext) {
    if (ext == ".html") return "text/html";
    if (ext == ".js")   return "application/javascript";
    if (ext == ".css")  return "text/css";
    if (ext == ".json") return "application/json";
    if (ext == ".png")  return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".svg")  return "image/svg+xml";
    if (ext == ".txt")  return "text/plain";
    return "application/octet-stream";
}

// Add CORS headers to a response
void addCorsHeaders(Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    Server svr;

    // Basic OPTIONS handler for CORS preflight
    svr.Options(R"(/.*)", [](const Request& /*req*/, Response& res) {
        addCorsHeaders(res);
        res.status = 200;
    });

    // Serve index.html at root
    svr.Get("/", [](const Request&, Response& res) {
        string html = readFile("index.html");
        if (html.empty()) {
            res.status = 404;
            res.set_content("index.html not found", "text/plain");
        } else {
            addCorsHeaders(res);
            res.set_content(html, "text/html");
        }
    });

    // Serve any static file requested under root, e.g. /app.js or /styles.css
    svr.Get(R"(/(.*))", [](const Request& req, Response& res) {
        string rel = req.matches[1]; // matched portion after first slash
        // Prevent directory traversal:
        if (rel.find("..") != string::npos) {
            res.status = 400;
            res.set_content("Bad request", "text/plain");
            return;
        }

        // if empty (shouldn't happen because "/" handled above), treat as index
        if (rel.empty()) rel = "index.html";

        // ensure path is normalized and exists
        fs::path p = fs::u8path(rel);
        if (!fs::exists(p) || fs::is_directory(p)) {
            // Not a static file - might be an API endpoint; return 404 here
            res.status = 404;
            addCorsHeaders(res);
            res.set_content("Not found", "text/plain");
            return;
        }

        string data = readFile(p.string());
        if (data.empty()) {
            res.status = 500;
            addCorsHeaders(res);
            res.set_content("Failed to read file", "text/plain");
            return;
        }

        string ext = p.extension().u8string();
        string ctype = contentTypeForExt(ext);
        addCorsHeaders(res);
        res.set_content(data, ctype);
    });

    // API endpoint for booking/cancel/get_bookings
    svr.Post("/api", [](const Request& req, Response& res) {
        addCorsHeaders(res);
        try {
            auto requestJson = nlohmann::json::parse(req.body);
            auto responseJson = manager.processJsonRequest(requestJson, rooms, booked);
            res.set_content(responseJson.dump(), "application/json");
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["success"] = false;
            error["error"] = e.what();
            res.set_content(error.dump(), "application/json");
        }
    });

    cout << "Server started at http://localhost:8081\n";
    svr.listen("localhost", 8081);

    return 0;
}
