/**
 * @file bytearray.h
 * @brief Leee字符串工具模块
 * @details 增强的字符串类，继承自std::string，提供Qt QByteArray风格的接口
 *
 * 该模块提供了丰富的字符串操作功能，包括：
 * - Qt风格的基本操作（isEmpty, constData等）
 * - 字符串分割和连接
 * - 查找和替换
 * - 类型转换
 * - 文件操作
 * - JSON处理
 *
 * @author Sail <sailcpu@gmail.com>
 * @version 1.0.0
 * @date 2024
 *
 * @copyright MIT License
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

// For convenience
using json = nlohmann::json;

#ifndef SLASH
#if defined(_WIN32) || defined(WIN32)
#define SLASH '\\'
#else
#define SLASH '/'
#endif
#endif

namespace leee {

/**
 * @brief 增强的字符串类，采用Qt QByteArray风格设计
 *
 * ByteArray类继承自std::string，提供丰富的字符串操作功能。
 * 该类完全兼容std::string的所有操作，同时添加了Qt风格的便捷方法。
 *
 * @section usage 使用示例
 * @code
 * #include <bytearray.h>
 *
 * leee::ByteArray str("Hello World");
 * str.toUpper();  // 转换为大写
 * auto parts = str.split(' ');  // 分割字符串
 * @endcode
 *
 * @section features 主要特性
 * - 完全兼容std::string
 * - Qt风格的API设计
 * - 丰富的字符串操作方法
 * - 类型安全转换
 * - JSON处理支持
 * - 文件操作支持
 *
 * @note 该类是header-only设计，无需编译即可使用
 * @warning 继承自std::string，注意对象切片问题
 *
 * @sa std::string
 */
class ByteArray : public std::string
{
public:
    // Inherit all std::string constructors
    using std::string::string;

    // Additional constructors for convenience
    ByteArray() : std::string() {}
    ByteArray(const std::string& str) : std::string(str) {}
    ByteArray(const char* str) : std::string(str ? str : "") {}
    ByteArray(const char* str, size_type n) : std::string(str, n) {}

    // Basic operations (Qt style)
    /**
     * @brief 检查字符串是否为空
     * @return true如果字符串长度为0，false否则
     * @note 相当于empty()
     */
    bool isEmpty() const { return empty(); }

    /**
     * @brief 获取常量C风格字符串指针
     * @return 指向字符串数据的常量指针
     * @note 相当于c_str()
     */
    const char* constData() const { return c_str(); }

    /**
     * @brief 获取可修改的字符串数据指针
     * @return 指向字符串数据的可修改指针
     * @warning 使用后需要确保字符串仍然有效
     */
    char* data() { return &(*this)[0]; }

    /**
     * @brief 获取常量字符串数据指针
     * @return 指向字符串数据的常量指针
     */
    const char* data() const { return c_str(); }

    /**
     * @brief 获取std::string引用
     * @return 当前对象的std::string引用
     */
    const std::string& str() const { return *this; }

    // Capacity operations
    void resize(size_type n) { std::string::resize(n); }
    void resize(size_type n, char c) { std::string::resize(n, c); }
    void reserve(size_type n = 0) { std::string::reserve(n); }
    void clear() { std::string::clear(); }
    void squeeze() { shrink_to_fit(); }

    // Substring operations
    /**
     * @brief 获取左边的子字符串
     * @param len 要获取的字符数
     * @return 包含前len个字符的新字符串
     * @note 如果len超过字符串长度，返回整个字符串
     */
    ByteArray left(size_t len) const;

    /**
     * @brief 获取右边的子字符串
     * @param len 要获取的字符数
     * @return 包含后len个字符的新字符串
     * @note 如果len超过字符串长度，返回整个字符串
     */
    ByteArray right(size_t len) const;

    /**
     * @brief 获取中间的子字符串
     * @param pos 开始位置
     * @param len 要获取的字符数，默认为npos表示到结尾
     * @return 指定位置和长度的子字符串
     * @note 相当于substr(pos, len)
     */
    ByteArray mid(size_t pos, size_t len = std::string::npos) const;

    // String checking
    /**
     * @brief 检查字符串是否以指定前缀开头
     * @param prefix 要检查的前缀
     * @return true如果以指定前缀开头，false否则
     */
    bool startsWith(const std::string& prefix) const;

    /**
     * @brief 检查字符串是否以指定后缀结尾
     * @param suffix 要检查的后缀
     * @return true如果以指定后缀结尾，false否则
     */
    bool endsWith(const std::string& suffix) const;

    /**
     * @brief 检查字符串是否包含指定子串
     * @param substr 要查找的子串
     * @return true如果包含指定子串，false否则
     * @note 相当于find() != npos
     */
    bool contains(const std::string& substr) const;

    // String modification
    /**
     * @brief 在字符串末尾追加字符串
     * @param str 要追加的字符串
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& append(const std::string& str) { *this += str; return *this; }

    /**
     * @brief 在字符串末尾追加字符
     * @param ch 要追加的字符
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& append(char ch) { *this += ch; return *this; }

    /**
     * @brief 在字符串开头插入字符串
     * @param str 要插入的字符串
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& prepend(const std::string& str);

    /**
     * @brief 在字符串开头插入字符
     * @param ch 要插入的字符
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& prepend(char ch);

    /**
     * @brief 将字符串转换为小写
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& toLower();

    /**
     * @brief 将字符串转换为大写
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& toUpper();

    /**
     * @brief 移除字符串两端的空白字符
     * @return 当前对象的引用，支持链式调用
     * @note 空白字符包括空格、制表符、换行符等
     */
    ByteArray& trim();

    /**
     * @brief 移除字符串左端的空白字符
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& trimLeft();

    /**
     * @brief 移除字符串右端的空白字符
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& trimRight();

    /**
     * @brief 替换字符串中的子串
     * @param oldStr 要替换的旧字符串
     * @param newStr 新的字符串
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& replace(const std::string& oldStr, const std::string& newStr);

    /**
     * @brief 从字符串中移除指定位置和长度的子串
     * @param pos 开始位置
     * @param len 要移除的字符数，默认为npos表示到结尾
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& remove(size_t pos, size_t len = std::string::npos);

    // Qt-style insert/remove
    ByteArray& insert(size_type pos, const std::string& str);

    // Splitting and joining
    /**
     * @brief 按单个字符分割字符串
     * @param delimiter 分隔符字符
     * @return 分割后的字符串数组
     * @note 如果字符串为空，返回包含一个空字符串的数组
     */
    std::vector<ByteArray> split(char delimiter) const;

    /**
     * @brief 按字符串分割字符串
     * @param delimiter 分隔符字符串
     * @return 分割后的字符串数组
     * @note 如果分隔符为空，返回包含原字符串的数组
     */
    std::vector<ByteArray> split(const std::string& delimiter) const;

    /**
     * @brief 将字符串数组连接为单个字符串
     * @param parts 要连接的字符串数组
     * @param delimiter 连接符
     * @return 连接后的字符串
     */
    static ByteArray join(const std::vector<ByteArray>& parts, const std::string& delimiter);

    // Qt-style search operations
    /**
     * @brief 从指定位置开始查找字符
     * @param ch 要查找的字符
     * @param from 开始查找的位置，默认为0
     * @return 找到的位置，如果没找到返回npos
     */
    size_t indexOf(char ch, size_t from = 0) const;

    /**
     * @brief 从指定位置开始查找字符串
     * @param str 要查找的字符串
     * @param from 开始查找的位置，默认为0
     * @return 找到的位置，如果没找到返回npos
     */
    size_t indexOf(const std::string& str, size_t from = 0) const;

    /**
     * @brief 从指定位置开始反向查找字符
     * @param ch 要查找的字符
     * @param from 开始查找的位置，默认为npos（从末尾开始）
     * @return 找到的位置，如果没找到返回npos
     */
    size_t lastIndexOf(char ch, size_t from = std::string::npos) const;

    /**
     * @brief 从指定位置开始反向查找字符串
     * @param str 要查找的字符串
     * @param from 开始查找的位置，默认为npos（从末尾开始）
     * @return 找到的位置，如果没找到返回npos
     */
    size_t lastIndexOf(const std::string& str, size_t from = std::string::npos) const;

    // Type conversion
    /**
     * @brief 将字符串转换为整数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的整数值，转换失败时返回0
     * @note 支持十进制、八进制（0开头）、十六进制（0x开头）
     */
    int toInt(bool* ok = nullptr) const;

    /**
     * @brief 将字符串转换为长整数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的长整数值，转换失败时返回0
     */
    long toLong(bool* ok = nullptr) const;

    /**
     * @brief 将字符串转换为长长整数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的长长整数值，转换失败时返回0
     */
    long long toLongLong(bool* ok = nullptr) const;
    /**
     * @brief 将字符串转换为无符号整数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的无符号整数值，转换失败时返回0
     */
    unsigned int toUInt(bool* ok = nullptr) const;

    /**
     * @brief 将字符串转换为无符号长整数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的无符号长整数值，转换失败时返回0
     */
    unsigned long toULong(bool* ok = nullptr) const;

    /**
     * @brief 将字符串转换为无符号长长整数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的无符号长长整数值，转换失败时返回0
     */
    unsigned long long toULongLong(bool* ok = nullptr) const;

    /**
     * @brief 将字符串转换为浮点数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的浮点数值，转换失败时返回0.0f
     */
    float toFloat(bool* ok = nullptr) const;

    /**
     * @brief 将字符串转换为双精度浮点数
     * @param ok 可选的输出参数，转换成功时设为true，失败时设为false
     * @return 转换后的双精度浮点数值，转换失败时返回0.0
     */
    double toDouble(bool* ok = nullptr) const;

    // Number to string
    /**
     * @brief 将整数转换为字符串
     * @param n 要转换的整数
     * @param base 进制，默认为10，支持2-36进制
     * @return 转换后的字符串
     */
    static ByteArray number(int n, int base = 10);

    /**
     * @brief 将无符号整数转换为字符串
     * @param n 要转换的无符号整数
     * @param base 进制，默认为10，支持2-36进制
     * @return 转换后的字符串
     */
    static ByteArray number(unsigned int n, int base = 10);

    /**
     * @brief 将长整数转换为字符串
     * @param n 要转换的长整数
     * @param base 进制，默认为10，支持2-36进制
     * @return 转换后的字符串
     */
    static ByteArray number(long n, int base = 10);

    /**
     * @brief 将无符号长整数转换为字符串
     * @param n 要转换的无符号长整数
     * @param base 进制，默认为10，支持2-36进制
     * @return 转换后的字符串
     */
    static ByteArray number(unsigned long n, int base = 10);

    /**
     * @brief 将长长整数转换为字符串
     * @param n 要转换的长长整数
     * @param base 进制，默认为10，支持2-36进制
     * @return 转换后的字符串
     */
    static ByteArray number(long long n, int base = 10);

    /**
     * @brief 将无符号长长整数转换为字符串
     * @param n 要转换的无符号长长整数
     * @param base 进制，默认为10，支持2-36进制
     * @return 转换后的字符串
     */
    static ByteArray number(unsigned long long n, int base = 10);

    /**
     * @brief 将浮点数转换为字符串
     * @param n 要转换的浮点数
     * @param format 格式字符：'e'/'E'(科学计数法), 'f'/'F'(定点表示), 'g'/'G'(自动选择)
     * @param precision 精度，小数点后的位数
     * @return 转换后的字符串
     */
    static ByteArray number(float n, char format = 'g', int precision = 6);

    /**
     * @brief 将双精度浮点数转换为字符串
     * @param n 要转换的双精度浮点数
     * @param format 格式字符：'e'/'E'(科学计数法), 'f'/'F'(定点表示), 'g'/'G'(自动选择)
     * @param precision 精度，小数点后的位数
     * @return 转换后的字符串
     */
    static ByteArray number(double n, char format = 'g', int precision = 6);

    // File operations
    /**
     * @brief 将字符串内容保存到文件
     * @param filePath 文件路径
     * @return 保存成功返回true，失败返回false
     * @note 如果文件不存在会创建新文件，如果存在会覆盖原有内容
     */
    bool saveToFile(const std::string& filePath) const;

    /**
     * @brief 从文件加载字符串内容
     * @param filePath 文件路径
     * @return 文件内容，如果加载失败返回空字符串
     */
    static ByteArray loadFromFile(const std::string& filePath);

    /**
     * @brief 将字符串内容追加到文件末尾
     * @param filePath 文件路径
     * @return 追加成功返回true，失败返回false
     * @note 如果文件不存在会创建新文件
     */
    bool appendToFile(const std::string& filePath) const;

    // Path operations
    /**
     * @brief 从文件路径中提取文件名
     * @param filePath 文件路径
     * @return 文件名（不包含路径）
     * @example getFileName("/home/user/file.txt") 返回 "file.txt"
     */
    static ByteArray getFileName(const std::string& filePath);

    /**
     * @brief 从文件路径中提取目录部分
     * @param filePath 文件路径
     * @return 目录路径（不包含文件名）
     * @example getDirectory("/home/user/file.txt") 返回 "/home/user/"
     */
    static ByteArray getDirectory(const std::string& filePath);

    /**
     * @brief 从文件路径中提取文件扩展名
     * @param filePath 文件路径
     * @return 文件扩展名（不包含点号）
     * @example getFileExtension("/home/user/file.txt") 返回 "txt"
     */
    static ByteArray getFileExtension(const std::string& filePath);
    /**
     * @brief 获取当前字符串作为文件路径时的文件名
     * @return 文件名（不包含路径）
     */
    ByteArray getFileName() const;

    /**
     * @brief 获取当前字符串作为文件路径时的目录部分
     * @return 目录路径（不包含文件名）
     */
    ByteArray getDirectory() const;

    /**
     * @brief 获取当前字符串作为文件路径时的文件扩展名
     * @return 文件扩展名（不包含点号）
     */
    ByteArray getFileExtension() const;

    // Encoding operations
    /**
     * @brief 将二进制数据转换为十六进制字符串
     * @return 十六进制表示的字符串，每个字节用两个十六进制字符表示
     * @example "ABC".toHexString() 返回 "414243"
     */
    ByteArray toHexString() const;

    /**
     * @brief 从十六进制字符串转换为二进制数据
     * @param hexStr 十六进制字符串
     * @return 二进制数据，如果转换失败返回空字符串
     * @note 字符串长度必须是偶数，每两个字符表示一个字节
     */
    static ByteArray fromHexString(const std::string& hexStr);

    // Qt-style comparison
    /**
     * @brief 与另一个字符串进行比较
     * @param other 要比较的字符串
     * @param caseSensitive 是否区分大小写，默认为true
     * @return 比较结果：<0(当前字符串小), 0(相等), >0(当前字符串大)
     */
    int compare(const ByteArray& other, bool caseSensitive = true) const;

    // Qt-style utility functions
    /**
     * @brief 用指定字符填充字符串
     * @param ch 填充字符
     * @param size 填充后的字符串长度，-1表示保持当前长度
     * @return 当前对象的引用，支持链式调用
     */
    ByteArray& fill(char ch, int size = -1);

    /**
     * @brief 重复字符串指定次数
     * @param times 重复次数
     * @return 重复后的新字符串
     */
    ByteArray repeated(int times) const;

    // Utility functions
    /**
     * @brief 获取环境变量的值
     * @param name 环境变量名
     * @return 环境变量的值，如果不存在返回空字符串
     */
    static ByteArray getEnvironmentVariable(const std::string& name);

    /**
     * @brief 获取当前时间戳（以ticks为单位）
     * @return 当前时间戳
     */
    static long long getCurrentTimeStampTicks();

    /**
     * @brief 获取当前时间戳（以毫秒为单位）
     * @return 当前时间戳（毫秒）
     */
    static double getCurrentTimeStampTicksMs();
    /**
     * @brief 获取当前时间的字符串表示
     * @return 当前时间的字符串，格式为"YYYY-MM-DD HH:MM:SS"
     */
    static ByteArray getCurrentTimeString();

    // Template operations
    /**
     * @brief 将任意类型的数据打包为二进制数据
     * @tparam T 要打包的数据类型
     * @param data 要打包的数据
     * @return 包含二进制数据的字符串
     * @warning 使用memcpy进行二进制复制，不适用于包含指针的复杂类型
     */
    template <typename T>
    static ByteArray pack(const T& data) {
        ByteArray result;
        result.resize(sizeof(T));
        memcpy(result.data(), &data, sizeof(T));
        return result;
    }

    /**
     * @brief 从二进制缓冲区解包数据
     * @tparam T 要解包的数据类型
     * @param buffer 二进制数据缓冲区
     * @param ok 可选的输出参数，表示解包是否成功
     * @return 解包后的数据
     * @warning 使用memcpy进行二进制复制，不适用于包含指针的复杂类型
     */
    template <typename T>
    static T unpack(const char* buffer, bool* ok = nullptr) {
        T result;
        memcpy(&result, buffer, sizeof(T));
        if (ok) *ok = true;
        return result;
    }

    /**
     * @brief 从当前字符串解包数据
     * @tparam T 要解包的数据类型
     * @param ok 可选的输出参数，表示解包是否成功
     * @return 解包后的数据
     * @warning 使用memcpy进行二进制复制，不适用于包含指针的复杂类型
     */
    template <typename T>
    T unpack(bool* ok = nullptr) const {
        return unpack<T>(data(), ok);
    }

    // Operators
    /**
     * @brief +=操作符，支持链式调用
     * @param str 要追加的字符串
     * @return 当前对象的引用
     */
    ByteArray& operator+=(const std::string& str) {
        std::string::operator+=(str);
        return *this;
    }

    /**
     * @brief +=操作符，支持C风格字符串
     * @param str 要追加的C风格字符串
     * @return 当前对象的引用
     */
    ByteArray& operator+=(const char* str) {
        if (str) std::string::operator+=(str);
        return *this;
    }

    /**
     * @brief +=操作符，支持另一个ByteArray对象
     * @param other 要追加的ByteArray对象
     * @return 当前对象的引用
     */
    ByteArray& operator+=(const ByteArray& other) {
        std::string::operator+=(other);
        return *this;
    }

    /**
     * @brief +=操作符，支持单个字符
     * @param ch 要追加的字符
     * @return 当前对象的引用
     */
    ByteArray& operator+=(char ch) {
        std::string::operator+=(ch);
        return *this;
    }

    /**
     * @brief +操作符，连接字符串
     * @param str 要连接的字符串
     * @return 连接后的新字符串
     */
    ByteArray operator+(const std::string& str) const {
        return ByteArray(*this) += str;
    }

    /**
     * @brief +操作符，连接C风格字符串
     * @param str 要连接的C风格字符串
     * @return 连接后的新字符串
     */
    ByteArray operator+(const char* str) const {
        return ByteArray(*this) += str;
    }

    /**
     * @brief +操作符，连接另一个ByteArray对象
     * @param other 要连接的ByteArray对象
     * @return 连接后的新字符串
     */
    ByteArray operator+(const ByteArray& other) const {
        return ByteArray(*this) += other;
    }

    /**
     * @brief +操作符，连接单个字符
     * @param ch 要连接的字符
     * @return 连接后的新字符串
     */
    ByteArray operator+(char ch) const {
        return ByteArray(*this) += ch;
    }

    /**
     * @brief ==操作符，比较两个ByteArray对象
     * @param other 要比较的ByteArray对象
     * @return true如果相等，false否则
     */
    bool operator==(const ByteArray& other) const {
        return static_cast<const std::string&>(*this) == static_cast<const std::string&>(other);
    }

    /**
     * @brief !=操作符，比较两个ByteArray对象
     * @param other 要比较的ByteArray对象
     * @return true如果不相等，false否则
     */
    bool operator!=(const ByteArray& other) const {
        return static_cast<const std::string&>(*this) != static_cast<const std::string&>(other);
    }

    /**
     * @brief <操作符，比较两个ByteArray对象
     * @param other 要比较的ByteArray对象
     * @return true如果当前对象小于other，false否则
     */
    bool operator<(const ByteArray& other) const {
        return static_cast<const std::string&>(*this) < static_cast<const std::string&>(other);
    }

    // Iterator support (inherited from std::string)
    using std::string::begin;
    using std::string::end;
    using std::string::cbegin;
    using std::string::cend;
    using std::string::rbegin;
    using std::string::rend;
    using std::string::crbegin;
    using std::string::crend;
};

/**
 * @brief Performance analysis utility class
 */
class PerformanceUtil
{
public:
    PerformanceUtil() = default;

    void reset() {
        timePoints_.clear();
        recordTimePoint();
    }

    void recordTimePoint() {
        timePoints_.push_back(ByteArray::getCurrentTimeStampTicksMs());
    }

    double getDuration() const {
        if (timePoints_.size() < 2) return 0.0;
        return timePoints_.back() - timePoints_.front();
    }

    std::vector<double> getIntervals() const {
        std::vector<double> intervals;
        for (size_t i = 1; i < timePoints_.size(); ++i) {
            intervals.push_back(timePoints_[i] - timePoints_[i - 1]);
        }
        return intervals;
    }

    ByteArray getReport() const {
        double duration = getDuration();
        auto intervals = getIntervals();

        ByteArray report = ByteArray::number(duration, 'f', 3) + ByteArray(" ms total");
        if (!intervals.empty()) {
            report += ByteArray(" |");
            for (double interval : intervals) {
                report += ByteArray(" ") + ByteArray::number(interval, 'f', 3);
            }
        }
        return report;
    }

private:
    std::vector<double> timePoints_;
};

// Global helper functions (similar to log module style)
inline ByteArray toString(int value) {
    return ByteArray::number(value);
}

inline ByteArray toString(double value) {
    return ByteArray::number(value);
}

inline ByteArray toString(float value) {
    return ByteArray::number(value);
}

// Convenience functions
inline ByteArray formatString(const std::string& format) {
    return ByteArray(format);
}

template<typename... Args>
inline ByteArray formatString(const std::string& format, Args&&... args) {
    return ByteArray(fmt::format(format, std::forward<Args>(args)...));
}

// Implementation of ByteArray methods

// Substring operations
ByteArray ByteArray::left(size_t len) const {
    if (len >= size()) return *this;
    return ByteArray(substr(0, len));
}

ByteArray ByteArray::right(size_t len) const {
    if (len >= size()) return *this;
    return ByteArray(substr(size() - len, len));
}

ByteArray ByteArray::mid(size_t pos, size_t len) const {
    if (pos >= size()) return ByteArray();
    return ByteArray(substr(pos, len));
}

// String checking
bool ByteArray::startsWith(const std::string& prefix) const {
    return size() >= prefix.size() &&
           substr(0, prefix.size()) == prefix;
}

bool ByteArray::endsWith(const std::string& suffix) const {
    return size() >= suffix.size() &&
           substr(size() - suffix.size()) == suffix;
}

bool ByteArray::contains(const std::string& substr) const {
    return find(substr) != std::string::npos;
}

// String modification
ByteArray& ByteArray::prepend(const std::string& str) {
    *this = str + *this;
    return *this;
}

ByteArray& ByteArray::prepend(char ch) {
    *this = ch + *this;
    return *this;
}

ByteArray& ByteArray::toLower() {
    std::transform(begin(), end(), begin(), ::tolower);
    return *this;
}

ByteArray& ByteArray::toUpper() {
    std::transform(begin(), end(), begin(), ::toupper);
    return *this;
}

ByteArray& ByteArray::trim() {
    return trimLeft().trimRight();
}

ByteArray& ByteArray::trimLeft() {
    size_t start = find_first_not_of(" \t\n\r\f\v");
    if (start != std::string::npos) {
        *this = substr(start);
    } else {
        clear();
    }
    return *this;
}

ByteArray& ByteArray::trimRight() {
    size_t end = find_last_not_of(" \t\n\r\f\v");
    if (end != std::string::npos) {
        *this = substr(0, end + 1);
    } else {
        clear();
    }
    return *this;
}

ByteArray& ByteArray::replace(const std::string& oldStr, const std::string& newStr) {
    size_t pos = 0;
    while ((pos = find(oldStr, pos)) != std::string::npos) {
        std::string::replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return *this;
}

ByteArray& ByteArray::remove(size_t pos, size_t len) {
    if (pos < size()) {
        erase(pos, len);
    }
    return *this;
}

ByteArray& ByteArray::insert(size_type pos, const std::string& str) {
    std::string::insert(pos, str);
    return *this;
}

// Splitting and joining
std::vector<ByteArray> ByteArray::split(char delimiter) const {
    std::vector<ByteArray> result;
    std::stringstream ss(*this);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        result.emplace_back(token);
    }
    return result;
}

std::vector<ByteArray> ByteArray::split(const std::string& delimiter) const {
    std::vector<ByteArray> result;
    size_t start = 0;
    size_t end = find(delimiter);
    while (end != std::string::npos) {
        result.emplace_back(substr(start, end - start));
        start = end + delimiter.length();
        end = find(delimiter, start);
    }
    result.emplace_back(substr(start));
    return result;
}

ByteArray ByteArray::join(const std::vector<ByteArray>& parts, const std::string& delimiter) {
    if (parts.empty()) return ByteArray();
    ByteArray result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += delimiter + parts[i];
    }
    return result;
}

// Finding
size_t ByteArray::indexOf(char ch, size_t from) const {
    size_t pos = find(ch, from);
    return pos != std::string::npos ? pos : std::string::npos;
}

size_t ByteArray::indexOf(const std::string& str, size_t from) const {
    return find(str, from);
}

size_t ByteArray::lastIndexOf(char ch, size_t from) const {
    return rfind(ch, from);
}

size_t ByteArray::lastIndexOf(const std::string& str, size_t from) const {
    return rfind(str, from);
}

// Comparison
int ByteArray::compare(const ByteArray& other, bool caseSensitive) const {
    if (caseSensitive) {
        return std::string::compare(other);
    } else {
        ByteArray lower1 = *this;
        ByteArray lower2 = other;
        lower1.toLower();
        lower2.toLower();
        return lower1.std::string::compare(lower2);
    }
}

// Utility functions
ByteArray& ByteArray::fill(char ch, int size) {
    if (size < 0) {
        assign(this->size(), ch);
    } else {
        assign(static_cast<size_type>(size), ch);
    }
    return *this;
}

ByteArray ByteArray::repeated(int times) const {
    if (times <= 0) return ByteArray();
    if (times == 1) return *this;

    ByteArray result;
    result.reserve(size() * times);
    for (int i = 0; i < times; ++i) {
        result += *this;
    }
    return result;
}

// Type conversion
int ByteArray::toInt(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stoi(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0;
    }
}

long ByteArray::toLong(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stol(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0L;
    }
}

long long ByteArray::toLongLong(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stoll(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0LL;
    }
}

unsigned int ByteArray::toUInt(bool* ok) const {
    try {
        if (ok) *ok = true;
        return static_cast<unsigned int>(std::stoul(*this));
    } catch (...) {
        if (ok) *ok = false;
        return 0U;
    }
}

unsigned long ByteArray::toULong(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stoul(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0UL;
    }
}

unsigned long long ByteArray::toULongLong(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stoull(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0ULL;
    }
}

float ByteArray::toFloat(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stof(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0.0f;
    }
}

double ByteArray::toDouble(bool* ok) const {
    try {
        if (ok) *ok = true;
        return std::stod(*this);
    } catch (...) {
        if (ok) *ok = false;
        return 0.0;
    }
}

// Number to string
ByteArray ByteArray::number(int n, int base) {
    if (base == 10) {
        return ByteArray(std::to_string(n));
    } else if (base == 16) {
        std::stringstream ss;
        ss << "0x" << std::hex << n;
        return ByteArray(ss.str());
    } else if (base == 8) {
        std::stringstream ss;
        ss << "0" << std::oct << n;
        return ByteArray(ss.str());
    }
    return ByteArray(std::to_string(n));
}

ByteArray ByteArray::number(unsigned int n, int base) {
    if (base == 10) {
        return ByteArray(std::to_string(n));
    } else if (base == 16) {
        std::stringstream ss;
        ss << "0x" << std::hex << n;
        return ByteArray(ss.str());
    } else if (base == 8) {
        std::stringstream ss;
        ss << "0" << std::oct << n;
        return ByteArray(ss.str());
    }
    return ByteArray(std::to_string(n));
}

ByteArray ByteArray::number(long n, int base) {
    if (base == 10) {
        return ByteArray(std::to_string(n));
    } else if (base == 16) {
        std::stringstream ss;
        ss << "0x" << std::hex << n;
        return ByteArray(ss.str());
    } else if (base == 8) {
        std::stringstream ss;
        ss << "0" << std::oct << n;
        return ByteArray(ss.str());
    }
    return ByteArray(std::to_string(n));
}

ByteArray ByteArray::number(unsigned long n, int base) {
    if (base == 10) {
        return ByteArray(std::to_string(n));
    } else if (base == 16) {
        std::stringstream ss;
        ss << "0x" << std::hex << n;
        return ByteArray(ss.str());
    } else if (base == 8) {
        std::stringstream ss;
        ss << "0" << std::oct << n;
        return ByteArray(ss.str());
    }
    return ByteArray(std::to_string(n));
}

ByteArray ByteArray::number(long long n, int base) {
    if (base == 10) {
        return ByteArray(std::to_string(n));
    } else if (base == 16) {
        std::stringstream ss;
        ss << "0x" << std::hex << n;
        return ByteArray(ss.str());
    } else if (base == 8) {
        std::stringstream ss;
        ss << "0" << std::oct << n;
        return ByteArray(ss.str());
    }
    return ByteArray(std::to_string(n));
}

ByteArray ByteArray::number(unsigned long long n, int base) {
    if (base == 10) {
        return ByteArray(std::to_string(n));
    } else if (base == 16) {
        std::stringstream ss;
        ss << "0x" << std::hex << n;
        return ByteArray(ss.str());
    } else if (base == 8) {
        std::stringstream ss;
        ss << "0" << std::oct << n;
        return ByteArray(ss.str());
    }
    return ByteArray(std::to_string(n));
}

ByteArray ByteArray::number(float n, char format, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << n;
    return ByteArray(ss.str());
}

ByteArray ByteArray::number(double n, char format, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << n;
    return ByteArray(ss.str());
}

// File operations
bool ByteArray::saveToFile(const std::string& filePath) const {
    std::ofstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file.write(data(), size());
        file.close();
        return true;
    }
    return false;
}

ByteArray ByteArray::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string buffer(size, '\0');
        file.read(&buffer[0], size);
        file.close();
        return ByteArray(buffer);
    }
    return ByteArray();
}

bool ByteArray::appendToFile(const std::string& filePath) const {
    std::ofstream file(filePath, std::ios::binary | std::ios::app);
    if (file.is_open()) {
        file.write(data(), size());
        file.close();
        return true;
    }
    return false;
}

// Path operations
ByteArray ByteArray::getFileName(const std::string& filePath) {
    size_t pos = filePath.rfind(SLASH);
    if (pos != std::string::npos) {
        return ByteArray(filePath.substr(pos + 1));
    }
    return ByteArray(filePath);
}

ByteArray ByteArray::getDirectory(const std::string& filePath) {
    size_t pos = filePath.rfind(SLASH);
    if (pos != std::string::npos) {
        return ByteArray(filePath.substr(0, pos + 1));
    }
    return ByteArray();
}

ByteArray ByteArray::getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.rfind('.');
    size_t slashPos = filePath.rfind(SLASH);

    if (dotPos != std::string::npos && (slashPos == std::string::npos || dotPos > slashPos)) {
        return ByteArray(filePath.substr(dotPos));
    }
    return ByteArray();
}

ByteArray ByteArray::getFileName() const {
    return getFileName(*this);
}

ByteArray ByteArray::getDirectory() const {
    return getDirectory(*this);
}

ByteArray ByteArray::getFileExtension() const {
    return getFileExtension(*this);
}

// Encoding operations
ByteArray ByteArray::toHexString() const {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : *this) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ByteArray(ss.str());
}

ByteArray ByteArray::fromHexString(const std::string& hexStr) {
    if (hexStr.size() % 2 != 0) return ByteArray();

    std::string result;
    for (size_t i = 0; i < hexStr.size(); i += 2) {
        std::string byteStr = hexStr.substr(i, 2);
        try {
            char byte = static_cast<char>(std::stoi(byteStr, nullptr, 16));
            result += byte;
        } catch (...) {
            return ByteArray();
        }
    }
    return ByteArray(result);
}

// Utility functions
ByteArray ByteArray::getEnvironmentVariable(const std::string& name) {
    const char* env = std::getenv(name.c_str());
    return env ? ByteArray(env) : ByteArray();
}

long long ByteArray::getCurrentTimeStampTicks() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

double ByteArray::getCurrentTimeStampTicksMs() {
    return getCurrentTimeStampTicks() * 1.0e-6;
}

ByteArray ByteArray::getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ByteArray(ss.str());
}

} // namespace leee