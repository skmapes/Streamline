
#include <crow.h>       // server library from crowcpp.org
#include <fstream>      // input output for HTML pages and JSON files
#include <iostream>     // logging

#include <windows.h>    // run in background

// log takes a string and writes it to the console if debugging is enabled
void log(std::string str) {
#ifdef DEBUG
    std::cout << str << "\n";
#endif // DEBUG
    return;
}

crow::SimpleApp app;

int main() {
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    /////////////////////
    // start endpoints //
    /////////////////////

    // / is the basic status page, shared with /status/ for clarity
    CROW_ROUTE(app, "/")([]() {
        std::string     result = "";
        std::string     input_line;
        std::ifstream   input_file;
        input_file.open("./assets/index.html");
        if (input_file.is_open()) {
            while (input_file) {
                std::getline(input_file, input_line);
                result = result + input_line;
            }
        }
        return result;
        });
    // duplicate of / endpoint
    CROW_ROUTE(app, "/status/")([]() {
        std::string     result = "";
        std::string     input_line;
        std::ifstream   input_file;
        input_file.open("./assets/index.html");
        if (input_file.is_open()) {
            while (input_file) {
                std::getline(input_file, input_line);
                result = result + input_line;
            }
        }
        return result;
        });
    // /tool/ sends the form to update tournament info for the overlay
    CROW_ROUTE(app, "/tool/")([]() {
        std::string     result = "";
        std::string     input_line;
        std::ifstream   input_file;
        input_file.open("./assets/tool/index.html");
        if (input_file.is_open()) {
            while (input_file) {
                std::getline(input_file, input_line);
                result = result + input_line;
            }
        }
        return result;
        });
    CROW_ROUTE(app, "/quit/")([]() {
        std::string     result = "server stopped";
        app.stop();     // for some reason this makes it so that app needs to be global??? i wish i could figure out how to make it work without that but for now it stays
        return result;
        });
    // /server_status sends a small text blurb with information about how the server is doing
    CROW_ROUTE(app, "/server_status/")([]() {
        std::string     result = "hello :)";
        return result;
        });

    // /get/tournament goes to start.gg and retrieves tournament info
    CROW_ROUTE(app, "/get/tournament").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        crow::multipart::message msg(req);
        crow::json::wvalue result;              // this is what crow uses to generate a JSON object
        result["tournament_slug"] = msg.parts[0].body;
        return result;
    });
    // /update/tournament updates the tournament info (name, date, etc)
    CROW_ROUTE(app, "/update/tournament").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        crow::multipart::message msg(req);
        crow::json::wvalue result;              // this is what crow uses to generate a JSON object
        result["tournament_name"] = msg.parts[0].body;
        result["tournament_date"] = msg.parts[1].body;
        result["event_name"] = msg.parts[2].body;
        std::ofstream   output_file;
        output_file.open("./assets/tournament.json");
        output_file << result.dump();           // dump is a crow method that stringifys the JSON
        log(result.dump());
        output_file.close();
        return result;
    });
    // /update/set updates the current set info (player names, set name, game, etc)
    CROW_ROUTE(app, "/update/set").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        crow::multipart::message msg(req);
        crow::json::wvalue result;              // this is what crow uses to generate a JSON object
        std::string name, temp;                 // used to define team names
        result["set_name"]              = msg.parts[0].body;
        result["set_goal"]              = msg.parts[1].body;

        // add a [W] or [L] to player 1 if they are in grands
        if (msg.parts[0].body == "Grand Finals" || msg.parts[0].body == "Grand Finals Reset") {
            result["player1"]["players"][0] = msg.parts[3].body + msg.parts[8].body;
        }
        else {
            result["player1"]["players"][0] = msg.parts[3].body;
        }
        // only add extra names to the list of players if they exist
        if (!msg.parts[4].body.empty())     { result["player1"]["players"][1] = msg.parts[4].body; }
        if (!msg.parts[5].body.empty())     { result["player1"]["players"][2] = msg.parts[5].body; }
        if (!msg.parts[6].body.empty())     { result["player1"]["players"][3] = msg.parts[6].body; }
        if (!msg.parts[7].body.empty())     { result["player1"]["players"][4] = msg.parts[7].body; }
        // set the name of the player or team
        if (msg.parts[2].body.empty())  {
            temp = msg.parts[3].body;
            name.append(temp);
            for (int i = 1; i < result["player1"]["players"].size(); i++) {
                name.append(" / ");
                temp = result["player1"]["players"][i].dump();
                temp.erase(temp.begin());
                temp.erase(temp.end() - 1);
                name.append(temp);
            }
            if (msg.parts[0].body == "Grand Finals" || msg.parts[0].body == "Grand Finals Reset") {
                name.append(msg.parts[8].body);
            }
            result["player1"]["name"] = name;
        }
        else {
            if (msg.parts[0].body == "Grand Finals" || msg.parts[0].body == "Grand Finals Reset") {
                result["player1"]["name"] = msg.parts[2].body + msg.parts[8].body;
            }
            else {
                result["player1"]["name"] = msg.parts[2].body;
            }
        }
        // set the score
        result["player1"]["score"]      = msg.parts[9].body;

        // add a [W] or [L] to player 2 if they are in grands
        if (msg.parts[0].body == "Grand Finals" || msg.parts[0].body == "Grand Finals Reset") {
            result["player2"]["players"][0] = msg.parts[17].body + msg.parts[11].body;
        }
        else {
            result["player2"]["players"][0] = msg.parts[11].body;
        }
        // only add extra names to the list of players if they exist
        if (!msg.parts[12].body.empty())    { result["player2"]["players"][1] = msg.parts[12].body; }
        if (!msg.parts[13].body.empty())    { result["player2"]["players"][2] = msg.parts[13].body; }
        if (!msg.parts[14].body.empty())    { result["player2"]["players"][3] = msg.parts[14].body; }
        if (!msg.parts[15].body.empty())    { result["player2"]["players"][4] = msg.parts[15].body; }
        // set the name of the player or team
        if (msg.parts[10].body.empty()) {
            name = "";
            if (msg.parts[0].body == "Grand Finals" || msg.parts[0].body == "Grand Finals Reset") {
                name.append(msg.parts[17].body);
            }
            temp = msg.parts[11].body;
            name.append(temp);
            for (int i = 1; i < result["player2"]["players"].size(); i++) {
                name.append(" / ");
                temp = result["player2"]["players"][i].dump();
                temp.erase(temp.begin());
                temp.erase(temp.end() - 1);
                name.append(temp);
            }
            result["player2"]["name"] = name;
        }
        else {
            if (msg.parts[0].body == "Grand Finals" || msg.parts[0].body == "Grand Finals Reset") {
                result["player2"]["name"] = msg.parts[17].body + msg.parts[10].body;
            }
            else {
                result["player2"]["name"] = msg.parts[10].body;
            }
        }
        // set the score
        result["player2"]["score"]      = msg.parts[16].body;

        // output the json
        std::ofstream   output_file;
        output_file.open("./assets/set.json");
        output_file << result.dump();           // dump is a crow method that stringifys the JSON
        log(result.dump());
        output_file.close();
        return result;
    });

    //////////////////////
    // finish endpoints //
    //////////////////////

#ifdef DEBUG
    app.loglevel(crow::LogLevel::Warning);
#endif
#ifndef DEBUG
    system("start http://localhost:8000/");
#endif

    // run the server
    app.port(8000)
        .multithreaded()
        .run();
}
