#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "csv_db.h"

// Split string by comma (used for ADD command)
static std::vector<std::string> splitComma(const std::string& s) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ',')) parts.push_back(item);
    return parts;
}

static void printHelp() {
    std::cout <<
        "Commands:\n"
        "  LOAD <file.csv>            - load data from CSV file\n"
        "  SHOW                       - display all records\n"
        "  FIND <column> <value>      - find record by column and value\n"
        "  ADD <v1,v2,...>            - add new record\n"
        "  UPDATE <row> <col> <value> - update value (0-indexed row)\n"
        "  DELETE <row>               - delete record (0-indexed row)\n"
        "  SAVE <file.csv>            - save data to CSV file\n"
        "  HELP                       - show this help message\n"
        "  EXIT / QUIT                - exit program\n";
}

int main() {
    std::cout << "==========================================\n";
    std::cout << "  CSV Mini Database & Query Engine\n";
    std::cout << "  Type 'HELP' for commands, 'EXIT' to stop.\n";
    std::cout << "==========================================\n";

    CsvDatabase db;
    std::string line;

    while (true) {
        std::cout << "db> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        for (auto& c : cmd) c = static_cast<char>(toupper(c));

        try {
            if (cmd == "EXIT" || cmd == "QUIT") {
                std::cout << "Exiting database. Goodbye!\n";
                break;
            } else if (cmd == "HELP") {
                printHelp();
            } else if (cmd == "LOAD") {
                std::string file; ss >> file;
                db.load(file);
                std::cout << "Loaded " << db.rowCount() << " record(s) from " << file << "\n";
            } else if (cmd == "SHOW") {
                db.display();
            } else if (cmd == "FIND") {
                std::string col, val; ss >> col >> val;
                auto matches = db.search(col, val);
                if (matches.empty()) {
                    std::cout << "No record found.\n";
                } else {
                    for (size_t idx : matches) {
                        std::cout << "Row " << idx << ": ";
                        for (const auto& f : db.getRow(idx)) std::cout << f << " ";
                        std::cout << "\n";
                    }
                }
            } else if (cmd == "ADD") {
                std::string rest;
                std::getline(ss, rest);
                if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
                db.addRecord(splitComma(rest));
                std::cout << "Record added. Total: " << db.rowCount() << "\n";
            } else if (cmd == "UPDATE") {
                size_t row; std::string col, val;
                ss >> row >> col >> val;
                db.updateRecord(row, col, val);
                std::cout << "Row " << row << " updated.\n";
            } else if (cmd == "DELETE") {
                size_t row; ss >> row;
                db.deleteRecord(row);
                std::cout << "Row " << row << " deleted. Total: " << db.rowCount() << "\n";
            } else if (cmd == "SAVE") {
                std::string file; ss >> file;
                db.save(file);
                std::cout << "Saved " << db.rowCount() << " record(s) to " << file << "\n";
            } else {
                std::cout << "Unknown command. Type HELP.\n";
            }
        } catch (const std::exception& e) {
            // Error handling: prevent crash, report error clearly
            std::cout << "[Error] " << e.what() << "\n";
        }
    }

    return 0;
}