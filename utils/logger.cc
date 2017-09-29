#include <iostream>

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

#include "logger.h"
using namespace utils;

namespace utils{
    Logger g_logger;
}

std::string vform(const char* format, va_list args) {
    size_t size = 1024;
    char* buffer = new char[size];

    while (1) {
        va_list args_copy;

#if defined(_MSC_VER) || defined(__BORLANDC__)
        args_copy = args;
#else
        va_copy(args_copy, args);
#endif

        int n = vsnprintf(buffer, size, format, args_copy);

        va_end(args_copy);

        // If that worked, return a string.
        if ((n > -1) && (static_cast<size_t>(n) < size)) {
            std::string s(buffer);
            delete [] buffer;
            return s;
        }

        // Else try again with more space.
        size = (n > -1) ?
            n + 1 :   // ISO/IEC 9899:1999
            size * 2; // twice the old size

        delete [] buffer;
        buffer = new char[size];
    }
}

// ==================== log4cpp ===================
void Logger::Initialize(int loggerLevel) {
    log4cpp::Appender *consoleAppender = new log4cpp::OstreamAppender("console", &std::cout);
    //consoleAppender->setLayout(new log4cpp::BasicLayout());
    log4cpp::PatternLayout *consoleLayout = new log4cpp::PatternLayout();
    consoleLayout->setConversionPattern("%d{%H:%M:%S,%l} %x [%p] %m%n");
    consoleAppender->setLayout(consoleLayout);

    log4cpp::Appender *logfileAppender = new log4cpp::FileAppender("default", "app.log");
    //logfileAppender->setLayout(new log4cpp::BasicLayout());
    log4cpp::PatternLayout *logfileLayout = new log4cpp::PatternLayout();
    logfileLayout->setConversionPattern("%d [%p] %m%n");
    logfileAppender->setLayout(logfileLayout);

    log4cpp::Category &rootCategory = log4cpp::Category::getRoot();
    rootCategory.setPriority(log4cpp::Priority::DEBUG);
    rootCategory.addAppender(consoleAppender);
    
    log4cpp::Category& logfileCategory = log4cpp::Category::getInstance("logfile");
    logfileCategory.setPriority(log4cpp::Priority::INFO);
    logfileCategory.addAppender(logfileAppender);

    // log examples
    //rootCategory << log4cpp::Priority::DEBUG << "log messges";
    //logfileCategory << log4cpp::Priority::INFO << "log messages";

    //root.errorStream() << "Another messsage.";
}

static std::string errorColor = "\033[1;31m";
static std::string warnColor = "\033[1;35m";
static std::string noticeColor = "\033[1;36m";
static std::string infoColor = "\033[1;32m";
static std::string debugColor = "\033[1;37m";
static std::string fatalColor = "\033[1;34m";
static std::string normalColor = "\033[0m";

void Logger::debug(const char *file, const char *func, int line, const char *format, ...){
    va_list va;
    va_start(va, format);
    std::string msg = debugColor + vform(format, va) + normalColor + " | " + file + ":" + std::to_string(line) + " " + func + "()";
    log4cpp::Category::getRoot().debug(msg.c_str());
    va_end(va);
}

void Logger::info(const char *file, const char *func, int line, const char *format, ...){
    va_list va;
    va_start(va, format);
    std::string msg = infoColor + vform(format, va) + normalColor + " | " + file + ":" + std::to_string(line) + " " + func + "()";
    log4cpp::Category::getRoot().info(msg.c_str());
    va_end(va);
}

void Logger::notice(const char *file, const char *func, int line, const char *format, ...){
    va_list va;
    va_start(va, format);
    std::string msg = noticeColor + vform(format, va) + normalColor + " | " + file + ":" + std::to_string(line) + " " + func + "()";
    log4cpp::Category::getRoot().notice(msg.c_str());
    va_end(va);
}

void Logger::warn(const char *file, const char *func, int line, const char *format, ...){
    va_list va;
    va_start(va, format);
    std::string msg = warnColor + vform(format, va) + normalColor + " | " + file + ":" + std::to_string(line) + " " + func + "()";
    log4cpp::Category::getRoot().warn(msg.c_str());
    va_end(va);
}

void Logger::error(const char *file, const char *func, int line, const char *format, ...){
    va_list va;
    va_start(va, format);
    std::string msg = errorColor + vform(format, va) + normalColor + " | " + file + ":" + std::to_string(line) + " " + func + "()";
    log4cpp::Category::getRoot().error(msg.c_str());
    va_end(va);
}

void Logger::fatal(const char *file, const char *func, int line, const char *format, ...){
    va_list va;
    va_start(va, format);
    std::string msg = fatalColor + vform(format, va) + normalColor + " | " + file + ":" + std::to_string(line) + " " + func + "()";
    log4cpp::Category::getRoot().fatal(msg.c_str());
    va_end(va);
}

// ==================== EasyLogging ===================

//INITIALIZE_EASYLOGGINGPP

////void Logger::Initialize(int argc, char *argv[]) {
    ////START_EASYLOGGINGPP(argc, argv);

//#include <signal.h>
//#include <execinfo.h>
//#include <unistd.h>

//void dump(int signo)
//{
        //char buf[1024];
        //char cmd[1024];
        ////FILE *fh;

        ////snprintf(buf, sizeof(buf), "/proc/%d/cmdline", getpid());
        ////if(!(fh = fopen(buf, "r")))
                ////exit(0);
        ////if(!fgets(buf, sizeof(buf), fh))
                ////exit(0);
        ////fclose(fh);
        //sprintf(buf, "/Users/jwsu/sandbox/geek/libofd/build/bin/ofdviewer"); 
        //if(buf[strlen(buf) - 1] == '\n')
                //buf[strlen(buf) - 1] = '\0';
        //snprintf(cmd, sizeof(cmd), "gdb %s %d -ex=bt > ./a.txt", buf, getpid());
        //system(cmd);

        //exit(0);
//}

//void _backtrace(int eid){
    //void *pTrace[256];
    //char **ppszMsg = NULL;
    //size_t uTraceSize = 0;
    //static const char szSigMsg[][256] = {
        //"Received SIGSEGV",
        //"Received SIGPIPE",
        //"Received SIGFPE",
        //"Received SIGABRT"};

    //do {
        //if (0 == (uTraceSize = backtrace(pTrace, sizeof(pTrace) / sizeof(void *)))) {
            //break;
        //}
        //if (NULL == (ppszMsg = backtrace_symbols(pTrace, uTraceSize))) {
            //break;
        //}

        //printf("%s. call stack:\n", szSigMsg[eid]);
        //for (size_t i = 0; i < uTraceSize; ++i) {
              //printf("%s\n", ppszMsg[i]);
        //}
    //} while (0);

    //if (NULL != ppszMsg) {
        //free(ppszMsg);
        //ppszMsg = NULL;
    //}

    ////dump(eid);
//}

//void _signal_handler(int signo){
    //_backtrace(signo);
    //exit(0);
//}

//void InitSignals(){
    //// 管道破裂
    //signal(SIGPIPE, _signal_handler);
    //// 非法内存访问
    //signal(SIGSEGV, _signal_handler);
    //// 数学相关异常，如除0、浮点溢出等
    //signal(SIGFPE, _signal_handler);
    //// 进程非正常退出
    //signal(SIGABRT, _signal_handler);
//}

//void Logger::Initialize(int loggerLevel) {

    //InitSignals();

    ////el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, 
    //el::Loggers::reconfigureLogger("performance", el::ConfigurationType::Format, 
            //"%datetime %logger %level : %msg");

    //el::Configurations defaultConf;
    //defaultConf.setToDefault();

    //std::string enableDebug = "false";

    ////if ( VLOG_IS_ON(1) ){
    //if ( loggerLevel >= 1 ){
        //enableDebug = "true";
    //}
    //defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, enableDebug);

    ////if ( VLOG_IS_ON(5) ) {
    //if ( loggerLevel >= 5 ) {
        //defaultConf.set(el::Level::Global, el::ConfigurationType::Format, 
                //"%datetime %logger %level [%fbase:%line %func]: %msg");
    //} else if ( VLOG_IS_ON(1) ) {
        //defaultConf.set(el::Level::Global, el::ConfigurationType::Format, 
                //"%datetime %logger %level [%fbase:%line %func]: %msg");
    //} else {
        //defaultConf.set(el::Level::Global, el::ConfigurationType::Format, 
                //"%datetime %logger %level [%fbase:%line]: %msg");
    //}

    //el::Loggers::reconfigureLogger("default", defaultConf);

    //el::Loggers::addFlag(el::LoggingFlag::NewLineForContainer);
    //el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    //el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
    ////el::Loggers::setVerboseLevel(5);
//}


    ////TIMED_FUNC(timerOpenOFD);
    ////TIMED_SCOPE(timerOpenPage, "Open OFD Page.");

