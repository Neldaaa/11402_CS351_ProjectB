// ==========================================================
//  Unit tests for CSV Mini Database (Project B)
//  Output format: one line per test case -> easy to screenshot.
//  Details are printed ONLY when a check fails.
// ==========================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../src/csv_db.h"

static int g_testsPassed = 0;
static int g_testsFailed = 0;
static std::vector<std::string> g_failures; // details of failed checks in current test

// CHECK: silent when OK, records detail when it fails.
#define CHECK(cond, detail)                                        \
    do {                                                           \
        if (!(cond)) {                                             \
            g_failures.push_back(std::string(detail) +             \
                " (line " + std::to_string(__LINE__) + ")");       \
        }                                                          \
    } while (0)

// Print one aligned result line: [TC-001] Load CSV file .......... PASS
static void report(const std::string& id, const std::string& name) {
    std::string label = "[" + id + "] " + name + " ";
    std::cout << label;
    for (size_t i = label.size(); i < 46; ++i) std::cout << '.';
    if (g_failures.empty()) {
        std::cout << " PASS\n";
        ++g_testsPassed;
    } else {
        std::cout << " FAIL\n";
        for (const auto& f : g_failures) std::cout << "    -> " << f << "\n";
        ++g_testsFailed;
    }
    g_failures.clear();
}

static void createMockCsv(const std::string& filename, const std::string& content) {
    std::ofstream f(filename);
    f << content;
}

// ---------- TC-001: Load CSV ----------
static void test_load_csv() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n3,Carol,28\n");
    CsvDatabase db;
    db.load("test_data.csv");
    CHECK(db.rowCount() == 3, "expected 3 records, got " + std::to_string(db.rowCount()));
    CHECK(db.getHeaders().size() == 3, "expected 3 header columns");
    CHECK(db.getField(0, "name") == "Alice", "row 0 'name' should be 'Alice'");
    report("TC-001", "Load CSV file");
}

// ---------- TC-002: Quoted fields ----------
static void test_quoted_fields() {
    createMockCsv("test_quotes.csv", "id,desc\n1,\"hello, world\"\n");
    CsvDatabase db;
    db.load("test_quotes.csv");
    CHECK(db.getField(0, "desc") == "hello, world",
          "quoted field parsed as '" + db.getField(0, "desc") + "'");
    report("TC-002", "Parse quoted fields");
}

// ---------- TC-003: Display ----------
static void test_display() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n");
    CsvDatabase db;
    db.load("test_data.csv");
    std::ostringstream out;
    db.display(out);
    CHECK(out.str().find("Alice") != std::string::npos, "output should contain 'Alice'");
    CHECK(out.str().find("2 record(s)") != std::string::npos, "output should report record count");
    report("TC-003", "Display records");
}

// ---------- TC-004: Search ----------
static void test_search() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n3,Bob,40\n");
    CsvDatabase db;
    db.load("test_data.csv");
    CHECK(db.search("name", "Bob").size() == 2, "search name=Bob should return 2 rows");
    CHECK(db.search("name", "Nobody").empty(), "search for missing value should return 0 rows");
    report("TC-004", "Search records");
}

// ---------- TC-005: Add ----------
static void test_add() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n");
    CsvDatabase db;
    db.load("test_data.csv");
    db.addRecord({"2", "David", "22"});
    CHECK(db.rowCount() == 2, "record count should grow to 2");
    CHECK(db.getField(1, "name") == "David", "new record should be 'David'");
    report("TC-005", "Add record");
}

// ---------- TC-006: Update ----------
static void test_update() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n");
    CsvDatabase db;
    db.load("test_data.csv");
    db.updateRecord(0, "age", "26");
    CHECK(db.getField(0, "age") == "26", "age should change 25 -> 26");
    report("TC-006", "Update record");
}

// ---------- TC-007: Delete ----------
static void test_delete() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n");
    CsvDatabase db;
    db.load("test_data.csv");
    db.deleteRecord(0);
    CHECK(db.rowCount() == 1, "record count should drop to 1");
    CHECK(db.getField(0, "name") == "Bob", "remaining record should be 'Bob'");
    report("TC-007", "Delete record");
}

// ---------- TC-008: Save ----------
static void test_save() {
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n");
    CsvDatabase db;
    db.load("test_data.csv");
    db.addRecord({"2", "Eve", "31"});
    db.save("test_out.csv");
    CsvDatabase db2;
    db2.load("test_out.csv");
    CHECK(db2.rowCount() == 2, "saved file should contain 2 records");
    CHECK(db2.getField(1, "name") == "Eve", "saved data should include 'Eve'");
    report("TC-008", "Save changes to CSV");
}

// ---------- TC-009: Error handling ----------
static void test_error_handling() {
    CsvDatabase db;
    bool threw = false;
    try { db.load("no_such_file.csv"); } catch (const std::exception&) { threw = true; }
    CHECK(threw, "loading a missing file should throw a clear error");

    createMockCsv("test_data.csv", "id,name\n1,Alice\n");
    db.load("test_data.csv");
    threw = false;
    try { db.search("bad_column", "x"); } catch (const std::exception&) { threw = true; }
    CHECK(threw, "searching an unknown column should throw");
    report("TC-009", "Error handling");
}

// ---------- TC-010: Missing fields ----------
static void test_missing_fields() {
    createMockCsv("test_missing.csv", "id,name,age\n1,Alice\n");
    CsvDatabase db;
    db.load("test_missing.csv");
    CHECK(db.getField(0, "age") == "", "missing field should become empty string");
    report("TC-010", "Handle missing fields");
}

int main() {
    std::cout << "=================================================\n";
    std::cout << "  CSV Mini Database & Query Engine - Test Suite\n";
    std::cout << "  Goal: load, query and edit CSV data safely\n";
    std::cout << "=================================================\n";

    test_load_csv();
    test_quoted_fields();
    test_display();
    test_search();
    test_add();
    test_update();
    test_delete();
    test_save();
    test_error_handling();
    test_missing_fields();

    std::remove("test_data.csv");
    std::remove("test_quotes.csv");
    std::remove("test_missing.csv");
    std::remove("test_out.csv");

    std::cout << "-------------------------------------------------\n";
    std::cout << "  Result: " << g_testsPassed << "/" << (g_testsPassed + g_testsFailed)
              << " test cases passed\n";
    std::cout << "=================================================\n";

    return g_testsFailed == 0 ? 0 : 1;
}
