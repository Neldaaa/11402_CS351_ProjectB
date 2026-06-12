#include <iostream>
#include <string>

// Sau này bạn sẽ include các header files của riêng bạn vào đây, ví dụ:
// #include "CSVParser.h"
// #include "Database.h"
// #include "QueryEngine.h"

int main() {
    std::cout << "==========================================\n";
    std::cout << "  CSV Mini Database & Query Engine\n";
    std::cout << "  Type 'exit' or 'quit' to stop.\n";
    std::cout << "==========================================\n";

    // 1. Khởi tạo Database (In-memory storage)
    // Database db;

    std::string input;
    while (true) {
        std::cout << "db> ";
        std::getline(std::cin, input);

        // Xử lý lệnh thoát
        if (input == "exit" || input == "quit") {
            std::cout << "Exiting database. Goodbye!\n";
            break;
        }

        // Bỏ qua nếu người dùng chỉ nhấn Enter
        if (input.empty()) {
            continue;
        }

        // 2. Phân tích lệnh của người dùng
        // - Nếu là lệnh LOAD: gọi hàm đọc CSV
        //   Ví dụ: LOAD 'data.csv'
        // - Nếu là câu lệnh SELECT/JOIN...: đưa vào Query Engine xử lý
        
        /* Pseudocode:
        if (input.starts_with("LOAD")) {
            db.loadCSV(extractPath(input));
        } else {
            QueryEngine::execute(db, input);
        }
        */

        // Tạm thời in ra câu lệnh để test REPL
        std::cout << "[Executed]: " << input << "\n";
    }

    return 0;
}