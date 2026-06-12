// ==========================================================
//  Unit tests cho CSV Mini Database (Project B)
//  Moi test in ro rang: ten test, ket qua, va chi tiet kiem tra.
//  Chay bang: ctest --test-dir build --output-on-failure
// ==========================================================
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../src/csv_db.h"

// ---------- Bo dem ket qua ----------
static int g_passed = 0;
static int g_failed = 0;

// CHECK: kiem tra 1 dieu kien, in chi tiet expected/actual khi sai.
#define CHECK(cond, detail)                                              \
    do {                                                                 \
        if (cond) {                                                      \
            std::cout << "      [OK] " << detail << "\n";                \
        } else {                                                         \
            std::cout << "      [FAIL] " << detail                       \
                      << "  (at line " << __LINE__ << ")\n";             \
            ++g_failed;                                                  \
        }                                                                \
    } while (0)

static void beginTest(const std::string& id, const std::string& name) {
    std::cout << "\n[" << id << "] " << name << "\n";
}

static void endTest() {
    // Neu khong co FAIL nao moi duoc tinh la passed o muc test case
    ++g_passed;
}

// Helper: tao file CSV gia de test
static void createMockCsv(const std::string& filename, const std::string& content) {
    std::ofstream f(filename);
    f << content;
}

// ==========================================================
//  TC-001: Load CSV
// ==========================================================
static void test_load_csv() {
    beginTest("TC-001", "Load CSV - doc dung header va so dong");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n3,Carol,28\n");

    CsvDatabase db;
    db.load("test_data.csv");

    CHECK(db.rowCount() == 3, "Doc duoc 3 records (actual: " + std::to_string(db.rowCount()) + ")");
    CHECK(db.getHeaders().size() == 3, "Header co 3 cot");
    CHECK(db.getHeaders()[0] == "id", "Cot dau tien la 'id'");
    CHECK(db.getField(0, "name") == "Alice", "Row 0, cot 'name' = 'Alice'");
    endTest();
}

// ==========================================================
//  TC-002: Load CSV co field trong ngoac kep
// ==========================================================
static void test_quoted_fields() {
    beginTest("TC-002", "Parse field co ngoac kep va dau phay");
    createMockCsv("test_quotes.csv", "id,desc\n1,\"hello, world\"\n");

    CsvDatabase db;
    db.load("test_quotes.csv");

    CHECK(db.getField(0, "desc") == "hello, world",
          "Field '\"hello, world\"' duoc parse thanh 'hello, world' (actual: '"
          + db.getField(0, "desc") + "')");
    endTest();
}

// ==========================================================
//  TC-003: Display records
// ==========================================================
static void test_display() {
    beginTest("TC-003", "Display - hien thi du lieu ra man hinh");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n");

    CsvDatabase db;
    db.load("test_data.csv");

    std::ostringstream out;
    db.display(out);
    std::string s = out.str();

    CHECK(s.find("name") != std::string::npos, "Output co chua header 'name'");
    CHECK(s.find("Alice") != std::string::npos, "Output co chua du lieu 'Alice'");
    CHECK(s.find("2 record(s)") != std::string::npos, "Output bao cao dung so record");
    endTest();
}

// ==========================================================
//  TC-004: Search / query
// ==========================================================
static void test_search() {
    beginTest("TC-004", "Search - tim record theo cot va gia tri");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n3,Bob,40\n");

    CsvDatabase db;
    db.load("test_data.csv");

    auto r1 = db.search("name", "Bob");
    CHECK(r1.size() == 2, "Tim 'name=Bob' ra 2 ket qua (actual: " + std::to_string(r1.size()) + ")");

    auto r2 = db.search("id", "1");
    CHECK(r2.size() == 1 && db.getField(r2[0], "name") == "Alice",
          "Tim 'id=1' ra dung record cua Alice");

    auto r3 = db.search("name", "Nobody");
    CHECK(r3.empty(), "Tim gia tri khong ton tai -> 0 ket qua");
    endTest();
}

// ==========================================================
//  TC-005: Add record
// ==========================================================
static void test_add() {
    beginTest("TC-005", "Add - them record moi");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n");

    CsvDatabase db;
    db.load("test_data.csv");
    db.addRecord({"2", "David", "22"});

    CHECK(db.rowCount() == 2, "So record tang tu 1 len 2");
    CHECK(db.getField(1, "name") == "David", "Record moi co name = 'David'");

    bool threw = false;
    try { db.addRecord({"chi-co-1-cot"}); } catch (const std::exception&) { threw = true; }
    CHECK(threw, "Them record sai so cot -> bao loi (khong crash)");
    endTest();
}

// ==========================================================
//  TC-006: Update record
// ==========================================================
static void test_update() {
    beginTest("TC-006", "Update - sua record co san");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n");

    CsvDatabase db;
    db.load("test_data.csv");
    db.updateRecord(0, "age", "26");

    CHECK(db.getField(0, "age") == "26", "age cua row 0 doi tu 25 -> 26");

    bool threw = false;
    try { db.updateRecord(99, "age", "1"); } catch (const std::exception&) { threw = true; }
    CHECK(threw, "Update row khong ton tai -> bao loi");
    endTest();
}

// ==========================================================
//  TC-007: Delete record
// ==========================================================
static void test_delete() {
    beginTest("TC-007", "Delete - xoa record");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n2,Bob,30\n");

    CsvDatabase db;
    db.load("test_data.csv");
    db.deleteRecord(0);

    CHECK(db.rowCount() == 1, "So record giam tu 2 xuong 1");
    CHECK(db.getField(0, "name") == "Bob", "Record con lai la 'Bob'");

    bool threw = false;
    try { db.deleteRecord(99); } catch (const std::exception&) { threw = true; }
    CHECK(threw, "Delete row khong ton tai -> bao loi");
    endTest();
}

// ==========================================================
//  TC-008: Save CSV
// ==========================================================
static void test_save() {
    beginTest("TC-008", "Save - ghi du lieu nguoc ra file CSV");
    createMockCsv("test_data.csv", "id,name,age\n1,Alice,25\n");

    CsvDatabase db;
    db.load("test_data.csv");
    db.addRecord({"2", "Eve", "31"});
    db.save("test_out.csv");

    // Doc lai file vua ghi de kiem tra
    CsvDatabase db2;
    db2.load("test_out.csv");
    CHECK(db2.rowCount() == 2, "File da ghi co du 2 records");
    CHECK(db2.getField(1, "name") == "Eve", "Du lieu moi (Eve) duoc luu dung");
    endTest();
}

// ==========================================================
//  TC-009: Error handling
// ==========================================================
static void test_error_handling() {
    beginTest("TC-009", "Error handling - file khong ton tai, cot sai");

    CsvDatabase db;
    bool threw = false;
    try { db.load("khong_ton_tai.csv"); } catch (const std::exception& e) {
        threw = true;
        std::cout << "      (thong bao loi: " << e.what() << ")\n";
    }
    CHECK(threw, "Load file khong ton tai -> nem exception ro rang");

    createMockCsv("test_data.csv", "id,name\n1,Alice\n");
    db.load("test_data.csv");
    threw = false;
    try { db.search("cot_khong_co", "x"); } catch (const std::exception&) { threw = true; }
    CHECK(threw, "Search cot khong ton tai -> bao loi");
    endTest();
}

// ==========================================================
//  TC-010: Dong thieu cot
// ==========================================================
static void test_missing_fields() {
    beginTest("TC-010", "Dong CSV thieu cot -> dien gia tri rong");
    createMockCsv("test_missing.csv", "id,name,age\n1,Alice\n");

    CsvDatabase db;
    db.load("test_missing.csv");
    CHECK(db.getField(0, "age") == "", "Cot 'age' bi thieu duoc dien chuoi rong");
    endTest();
}

// ==========================================================
//  MAIN - chay toan bo test va in tong ket
// ==========================================================
int main() {
    std::cout << "==========================================\n";
    std::cout << "  CSV Mini Database - Unit Test Suite\n";
    std::cout << "==========================================\n";

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

    // Don dep file tam
    std::remove("test_data.csv");
    std::remove("test_quotes.csv");
    std::remove("test_missing.csv");
    std::remove("test_out.csv");

    std::cout << "\n==========================================\n";
    if (g_failed == 0) {
        std::cout << "  RESULT: ALL " << g_passed << " TEST CASES PASSED\n";
    } else {
        std::cout << "  RESULT: " << g_failed << " CHECK(S) FAILED\n";
    }
    std::cout << "==========================================\n";

    // Tra ve khac 0 neu co loi -> ctest se bao FAIL dung chuan
    return g_failed == 0 ? 0 : 1;
}