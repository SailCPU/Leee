#ifndef LEE_BYTEARRAY_H
#define LEE_BYTEARRAY_H

#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace leee {

class ByteArray {
private:
    std::string data_;

public:
    ByteArray() = default;
    explicit ByteArray(const char* str) : data_(str) {}
    explicit ByteArray(const std::string& str) : data_(str) {}
    ByteArray(const ByteArray& other) : data_(other.data_) {}

    ByteArray& operator=(const ByteArray& other) {
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    bool isEmpty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
    std::string str() const { return data_; }

    ByteArray left(size_t len) const { return ByteArray(data_.substr(0, len)); }
    ByteArray right(size_t len) const { return ByteArray(data_.substr(data_.size() - len)); }
    ByteArray mid(size_t pos, size_t len) const { return ByteArray(data_.substr(pos, len)); }

    bool startsWith(const std::string& str) const { return data_.rfind(str, 0) == 0; }
    bool endsWith(const std::string& str) const {
        if (str.size() > data_.size()) return false;
        return data_.compare(data_.size() - str.size(), str.size(), str) == 0;
    }
    bool contains(const std::string& str) const { return data_.find(str) != std::string::npos; }

    ByteArray toUpper() const {
        std::string upper = data_;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        return ByteArray(upper);
    }
    ByteArray toLower() const {
        std::string lower = data_;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return ByteArray(lower);
    }

    ByteArray trim() const {
        size_t start = data_.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return ByteArray();
        size_t end = data_.find_last_not_of(" \t\n\r\f\v");
        return ByteArray(data_.substr(start, end - start + 1));
    }

    ByteArray& append(const std::string& str) { data_ += str; return *this; }
    ByteArray& prepend(const std::string& str) { data_ = str + data_; return *this; }
    ByteArray& insert(size_t pos, const std::string& str) { data_.insert(pos, str); return *this; }
    ByteArray& remove(size_t pos, size_t len) { data_.erase(pos, len); return *this; }

    std::vector<ByteArray> split(char delimiter) const {
        std::vector<ByteArray> parts;
        std::stringstream ss(data_);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            parts.emplace_back(item);
        }
        return parts;
    }

    size_t indexOf(char ch) const { return data_.find(ch); }
    size_t indexOf(const std::string& str) const { return data_.find(str); }
    size_t lastIndexOf(char ch) const { return data_.rfind(ch); }

    int toInt(bool* ok = nullptr) const {
        try {
            if (ok) *ok = true;
            return std::stoi(data_);
        } catch (...) {
            if (ok) *ok = false;
            return 0;
        }
    }

    double toDouble(bool* ok = nullptr) const {
        try {
            if (ok) *ok = true;
            return std::stod(data_);
        } catch (...) {
            if (ok) *ok = false;
            return 0.0;
        }
    }

    float toFloat(bool* ok = nullptr) const {
        try {
            if (ok) *ok = true;
            return std::stof(data_);
        } catch (...) {
            if (ok) *ok = false;
            return 0.0f;
        }
    }

    static ByteArray number(int num, int base = 10) {
        if (base < 2 || base > 36) return ByteArray();
        std::ostringstream oss;
        oss << std::setbase(base) << num;
        return ByteArray(oss.str());
    }

    static ByteArray number(double num, int prec = -1) {
        std::ostringstream oss;
        if (prec >= 0) oss << std::fixed << std::setprecision(prec);
        oss << num;
        return ByteArray(oss.str());
    }

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file) return false;
        file.write(data_.c_str(), data_.size());
        return true;
    }

    static ByteArray loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file) return ByteArray();
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string buffer(size, 0);
        file.read(&buffer[0], size);
        return ByteArray(buffer);
    }

    ByteArray toHexString() const {
        std::ostringstream oss;
        for (char c : data_) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
        }
        return ByteArray(oss.str());
    }

    static ByteArray fromHexString(const std::string& hex) {
        if (hex.size() % 2 != 0) return ByteArray();
        std::string result(hex.size() / 2, 0);
        for (size_t i = 0; i < hex.size() / 2; ++i) {
            result[i] = static_cast<char>(std::stoi(hex.substr(i * 2, 2), nullptr, 16));
        }
        return ByteArray(result);
    }

    int compare(const ByteArray& other, bool caseSensitive = true) const {
        if (caseSensitive) {
            return data_.compare(other.data_);
        } else {
            std::string a = toLower().data_;
            std::string b = other.toLower().data_;
            return a.compare(b);
        }
    }

    ByteArray& fill(char ch, size_t size = -1) {
        if (size == size_t(-1)) size = data_.size();
        data_.assign(size, ch);
        return *this;
    }

    ByteArray repeated(int times) const {
        std::string result;
        for (int i = 0; i < times; ++i) {
            result += data_;
        }
        return ByteArray(result);
    }

    static long long getCurrentTimeStampTicks() {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    }

    static double getCurrentTimeStampTicksMs() {
        return static_cast<double>(getCurrentTimeStampTicks());
    }

    static ByteArray getCurrentTimeString() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::tm* ptm = std::localtime(&time_t);
        std::ostringstream oss;
        oss << std::put_time(ptm, "%Y-%m-%d %H:%M:%S");
        return ByteArray(oss.str());
    }

    ByteArray operator+(const ByteArray& other) const {
        return ByteArray(data_ + other.data_);
    }

    ByteArray& operator+=(const ByteArray& other) {
        data_ += other.data_;
        return *this;
    }

    bool operator==(const ByteArray& other) const { return data_ == other.data_; }
    bool operator!=(const ByteArray& other) const { return data_ != other.data_; }
    bool operator<(const ByteArray& other) const { return data_ < other.data_; }
    bool operator>(const ByteArray& other) const { return data_ > other.data_; }

    template <typename T>
    static ByteArray pack(const T& value) {
        ByteArray ba;
        ba.data_.resize(sizeof(T));
        std::memcpy(&ba.data_[0], &value, sizeof(T));
        return ba;
    }

    template <typename T>
    T unpack(bool* ok = nullptr) const {
        if (data_.size() != sizeof(T)) {
            if (ok) *ok = false;
            return T();
        }
        T value;
        std::memcpy(&value, data_.c_str(), sizeof(T));
        if (ok) *ok = true;
        return value;
    }

    static ByteArray join(const std::vector<ByteArray>& parts, const std::string& separator) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) result += separator;
            result += parts[i].data_;
        }
        return ByteArray(result);
    }
};

} // namespace leee

#endif // LEE_BYTEARRAY_H