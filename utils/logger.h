#ifndef __UTILS_LOGGER_H__
#define __UTILS_LOGGER_H__

//#define ELPP_NO_LOG_TO_FILE
//#define ELPP_NO_DEFAULT_LOG_FILE
//#include "easylogging++.h"

#include <string>

namespace utils{

class Logger {
public:
    enum class Level{
        DEBUG,
        INFO,
        NOTICE,
        WARN,
        ERROR,
        FATAL
    };
    //static void Initialize(int argc, char *argv[]);
    static void Initialize(int loggerLevel = 0, const std::string &logfilename="");

    void debug(const char *file, const char *func, int line, const char *format, ...);
    void info(const char *file, const char *func, int line, const char *format, ...);
    void notice(const char *file, const char *func, int line, const char *format, ...);
    void warn(const char *file, const char *func, int line, const char *format, ...);
    void error(const char *file, const char *func, int line, const char *format, ...);
    void fatal(const char *file, const char *func, int line, const char *format, ...);

    
}; // class Logger

extern Logger g_logger;

#define LOG_DEBUG(format, ...) utils::g_logger.debug(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) utils::g_logger.info(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)
#define LOG_NOTICE(format, ...) utils::g_logger.notice(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) utils::g_logger.warn(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) utils::g_logger.error(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) utils::g_logger.fatal(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)

}; // namespace utils

#endif // __UTILS_LOGGER_H__
