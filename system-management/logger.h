//
// Leee Logger Module - Header-only logging implementation
// Based on plog library with GLOG-style formatting
//

#pragma once

// Disable plog's default LOG_* macros to avoid conflicts
#define PLOG_OMIT_LOG_DEFINES 1

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <filesystem>
#include <thread>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <chrono>

// Custom formatter similar to GLOG format
namespace plog
{
    class GlogFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            tm t;
            util::localtime_s(&t, &record.getTime().time);

            util::nostringstream ss;

            // Severity character (I/W/E/F)
            char severityChar = 'I';
            switch (record.getSeverity())
            {
            case fatal: severityChar = 'F';
                break;
            case error: severityChar = 'E';
                break;
            case warning: severityChar = 'W';
                break;
            case info:
            case debug:
            case verbose:
            default: severityChar = 'I';
                break;
            }
            ss << severityChar;

            // Date (MMDD)
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << (t.tm_mon + 1)
                << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");

            // Time (HH:MM:SS.microseconds)
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":")
                << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":")
                << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".")
                << std::setfill(PLOG_NSTR('0')) << std::setw(6) << (record.getTime().millitm * 1000) << PLOG_NSTR(" ");

            // Thread ID
            ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << record.getTid() << PLOG_NSTR(" ");

            // Filename:line number
            const char* fullFileName = record.getFile();
            const char* fileName = fullFileName;
            const char* lastSlash = strrchr(fullFileName, '/');
            if (!lastSlash)
            {
                lastSlash = strrchr(fullFileName, '\\');
            }
            if (lastSlash)
            {
                fileName = lastSlash + 1;
            }
            ss << fileName << PLOG_NSTR(":") << record.getLine() << PLOG_NSTR("] ");

            // Message
            ss << record.getMessage() << PLOG_NSTR("\n");

            return ss.str();
        }
    };
}

namespace leee {

// Forward declaration
void init_logger();

// Custom log stream class for supporting LOG_INFO << syntax
class LogStream
{
private:
    std::ostringstream oss_;
    plog::Severity level_;
    const char* file_;
    int line_;
    const char* func_;

public:
    explicit LogStream(plog::Severity level, const char* file = nullptr, int line = 0, const char* func = nullptr)
        : level_(level), file_(file), line_(line), func_(func)
    {
    }

    // Disable copy constructor and assignment operator
    LogStream(const LogStream&) = delete;
    LogStream& operator=(const LogStream&) = delete;

    template <typename T>
    LogStream& operator<<(const T& value)
    {
        oss_ << value;
        return *this;
    }

    // Support for stream manipulators like std::endl
    LogStream& operator<<(std::ostream& (*manip)(std::ostream&))
    {
        manip(oss_);
        return *this;
    }

    ~LogStream()
    {
        // Get the logger (use default logger if available, otherwise create one)
        auto* logger = plog::get();
        if (!logger)
        {
            // Initialize plog if not already done
            init_logger();
            logger = plog::get();
        }

        if (logger)
        {
            (*logger) += plog::Record(level_,
                                      func_ ? func_ : __FUNCTION__,
                                      line_ ? line_ : __LINE__,
                                      file_ ? file_ : __FILE__,
                                      nullptr, 0).ref() << oss_.str();
        }
    }
};

// plog logger macros (compatible with existing interface)
// Support streaming syntax: LOG_INFO << "message" << value
#define LOG_STREAM_ERROR leee::LogStream(plog::error, __FILE__, __LINE__, __FUNCTION__)
#define LOG_STREAM_WARN leee::LogStream(plog::warning, __FILE__, __LINE__, __FUNCTION__)
#define LOG_STREAM_INFO leee::LogStream(plog::info, __FILE__, __LINE__, __FUNCTION__)

// Define plog's LOG_* macros that were disabled
#define LOG_VERBOSE                     PLOG_VERBOSE
#define LOG_DEBUG                       PLOG_DEBUG
#define LOG_INFO                        LOG_STREAM_INFO
#define LOG_WARNING                     PLOG_WARNING
#define LOG_ERROR                       LOG_STREAM_ERROR
#define LOG_FATAL                       PLOG_FATAL
#define LOG_NONE                        PLOG_NONE

// Keep original names for backward compatibility
#define LOG_WARN LOG_STREAM_WARN

// Logger initialization function
inline void init_logger()
{
    static bool initialized = false;
    if (!initialized)
    {
        // Create console appender with GLOG-style formatter
        static plog::ConsoleAppender<plog::GlogFormatter> consoleAppender;

        // Create file appender with GLOG-style formatter
        static plog::RollingFileAppender<plog::GlogFormatter> fileAppender("logs/leee.log", 1000000, 5); // 1MB, 5 files

        // Initialize logger
        plog::init(plog::verbose, &consoleAppender).addAppender(&fileAppender);

        initialized = true;
    }
}

// Logger initialization with custom log file path
inline void init_logger(const std::string& log_file_path, size_t max_file_size = 1000000, int max_files = 5)
{
    static bool initialized = false;
    if (!initialized)
    {
        // Create console appender with GLOG-style formatter
        static plog::ConsoleAppender<plog::GlogFormatter> consoleAppender;

        // Create file appender with GLOG-style formatter
        static plog::RollingFileAppender<plog::GlogFormatter> fileAppender(log_file_path.c_str(), max_file_size, max_files);

        // Initialize logger
        plog::init(plog::verbose, &consoleAppender).addAppender(&fileAppender);

        initialized = true;
    }
}

// Set log level
inline void set_log_level(plog::Severity level)
{
    plog::get()->setMaxSeverity(level);
}

// Get current log level
inline plog::Severity get_log_level()
{
    return plog::get() ? plog::get()->getMaxSeverity() : plog::none;
}

// Check if logger is initialized
inline bool is_initialized()
{
    return plog::get() != nullptr;
}

} // namespace leee
