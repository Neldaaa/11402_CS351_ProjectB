#ifndef CSV_DB_H
#define CSV_DB_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// ==========================================================
//  CsvDatabase - mot CSDL mini don gian luu tru du lieu CSV
//  Chuc nang: Load / Display / Search / Add / Update / Delete / Save
// ==========================================================
class CsvDatabase {
public:
    // ---------- Load CSV ----------
    // Doc file CSV: dong dau la header, cac dong sau la du lieu.
    // Nem loi (throw) neu file khong ton tai -> Error handling.
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        headers_.clear();
        rows_.clear();

        std::string line;
        bool isHeader = true;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::vector<std::string> fields = parseLine(line);
            if (isHeader) {
                headers_ = fields;
                isHeader = false;
            } else {
                // Dong thieu cot -> dien chuoi rong; dong thua cot -> bo bot
                fields.resize(headers_.size(), "");
                rows_.push_back(fields);
            }
        }

        if (headers_.empty()) {
            throw std::runtime_error("File is empty or has no header: " + filename);
        }
    }

    // ---------- Display ----------
    // In toan bo bang ra man hinh (dang bang don gian).
    void display(std::ostream& out = std::cout) const {
        printRow(out, headers_);
        out << std::string(headers_.size() * 15, '-') << "\n";
        for (const auto& row : rows_) {
            printRow(out, row);
        }
        out << "(" << rows_.size() << " record(s))\n";
    }

    // ---------- Search ----------
    // Tim cac dong co gia tri cot `column` bang `value`.
    // Tra ve danh sach chi so (index) cua cac dong khop.
    std::vector<size_t> search(const std::string& column, const std::string& value) const {
        int col = columnIndex(column);
        if (col < 0) {
            throw std::runtime_error("Column not found: " + column);
        }
        std::vector<size_t> result;
        for (size_t i = 0; i < rows_.size(); ++i) {
            if (rows_[i][col] == value) result.push_back(i);
        }
        return result;
    }

    // ---------- Add ----------
    // Them 1 record moi. Nem loi neu so cot khong khop header.
    void addRecord(const std::vector<std::string>& record) {
        if (record.size() != headers_.size()) {
            throw std::runtime_error("Record has wrong number of fields");
        }
        rows_.push_back(record);
    }

    // ---------- Update ----------
    // Sua gia tri 1 o tai dong rowIndex, cot column.
    void updateRecord(size_t rowIndex, const std::string& column, const std::string& newValue) {
        if (rowIndex >= rows_.size()) {
            throw std::runtime_error("Row index out of range");
        }
        int col = columnIndex(column);
        if (col < 0) {
            throw std::runtime_error("Column not found: " + column);
        }
        rows_[rowIndex][col] = newValue;
    }

    // ---------- Delete ----------
    // Xoa dong tai vi tri rowIndex.
    void deleteRecord(size_t rowIndex) {
        if (rowIndex >= rows_.size()) {
            throw std::runtime_error("Row index out of range");
        }
        rows_.erase(rows_.begin() + rowIndex);
    }

    // ---------- Save ----------
    // Ghi du lieu hien tai nguoc lai ra file CSV.
    void save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot write file: " + filename);
        }
        file << joinCsv(headers_) << "\n";
        for (const auto& row : rows_) {
            file << joinCsv(row) << "\n";
        }
    }

    // ---------- Getters (dung cho test) ----------
    size_t rowCount() const { return rows_.size(); }
    const std::vector<std::string>& getHeaders() const { return headers_; }
    const std::vector<std::string>& getRow(size_t i) const { return rows_.at(i); }

    std::string getField(size_t rowIndex, const std::string& column) const {
        int col = columnIndex(column);
        if (col < 0) {
            throw std::runtime_error("Column not found: " + column);
        }
        return rows_.at(rowIndex)[col];
    }

private:
    std::vector<std::string> headers_;
    std::vector<std::vector<std::string>> rows_;

    // Tim chi so cot theo ten, tra ve -1 neu khong co.
    int columnIndex(const std::string& name) const {
        for (size_t i = 0; i < headers_.size(); ++i) {
            if (headers_[i] == name) return static_cast<int>(i);
        }
        return -1;
    }

    // Tach 1 dong CSV thanh cac field, ho tro dau ngoac kep:
    //   1,"a, b" -> [1] [a, b]
    static std::vector<std::string> parseLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string current;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                    current += '"'; // "" ben trong ngoac kep -> 1 dau "
                    ++i;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                fields.push_back(current);
                current.clear();
            } else if (c == '\r') {
                continue; // bo ky tu xuong dong kieu Windows
            } else {
                current += c;
            }
        }
        fields.push_back(current);
        return fields;
    }

    // Noi cac field thanh 1 dong CSV, tu dong them ngoac kep neu can.
    static std::string joinCsv(const std::vector<std::string>& fields) {
        std::string line;
        for (size_t i = 0; i < fields.size(); ++i) {
            if (i > 0) line += ",";
            const std::string& f = fields[i];
            if (f.find(',') != std::string::npos || f.find('"') != std::string::npos) {
                std::string escaped = f;
                size_t pos = 0;
                while ((pos = escaped.find('"', pos)) != std::string::npos) {
                    escaped.insert(pos, "\"");
                    pos += 2;
                }
                line += "\"" + escaped + "\"";
            } else {
                line += f;
            }
        }
        return line;
    }

    static void printRow(std::ostream& out, const std::vector<std::string>& row) {
        for (const auto& f : row) {
            out << f;
            // can le don gian bang khoang trang
            if (f.size() < 14) out << std::string(14 - f.size(), ' ');
            out << " ";
        }
        out << "\n";
    }
};

#endif // CSV_DB_H