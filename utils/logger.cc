#include "logger.h"

INITIALIZE_EASYLOGGINGPP

//void Logger::Initialize(int argc, char *argv[]) {
    //START_EASYLOGGINGPP(argc, argv);

#include <signal.h>
#include <execinfo.h>
#include <unistd.h>

void dump(int signo)
{
        char buf[1024];
        char cmd[1024];
        //FILE *fh;

        //snprintf(buf, sizeof(buf), "/proc/%d/cmdline", getpid());
        //if(!(fh = fopen(buf, "r")))
                //exit(0);
        //if(!fgets(buf, sizeof(buf), fh))
                //exit(0);
        //fclose(fh);
        sprintf(buf, "/Users/jwsu/sandbox/geek/libofd/build/bin/ofdviewer"); 
        if(buf[strlen(buf) - 1] == '\n')
                buf[strlen(buf) - 1] = '\0';
        snprintf(cmd, sizeof(cmd), "gdb %s %d -ex=bt > ./a.txt", buf, getpid());
        system(cmd);

        exit(0);
}

void _backtrace(int eid){
    void *pTrace[256];
    char **ppszMsg = NULL;
    size_t uTraceSize = 0;
    static const char szSigMsg[][256] = {
        "Received SIGSEGV",
        "Received SIGPIPE",
        "Received SIGFPE",
        "Received SIGABRT"};

    do {
        if (0 == (uTraceSize = backtrace(pTrace, sizeof(pTrace) / sizeof(void *)))) {
            break;
        }
        if (NULL == (ppszMsg = backtrace_symbols(pTrace, uTraceSize))) {
            break;
        }

        printf("%s. call stack:\n", szSigMsg[eid]);
        for (size_t i = 0; i < uTraceSize; ++i) {
              printf("%s\n", ppszMsg[i]);
        }
    } while (0);

    if (NULL != ppszMsg) {
        free(ppszMsg);
        ppszMsg = NULL;
    }

    //dump(eid);
}

void _signal_handler(int signo){
    _backtrace(signo);
    exit(0);
}

void InitSignals(){
    // 管道破裂
    signal(SIGPIPE, _signal_handler);
    // 非法内存访问
    signal(SIGSEGV, _signal_handler);
    // 数学相关异常，如除0、浮点溢出等
    signal(SIGFPE, _signal_handler);
    // 进程非正常退出
    signal(SIGABRT, _signal_handler);
}

void Logger::Initialize(int loggerLevel) {

    InitSignals();

    //el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, 
    el::Loggers::reconfigureLogger("performance", el::ConfigurationType::Format, 
            "%datetime %logger %level : %msg");

    el::Configurations defaultConf;
    defaultConf.setToDefault();

    std::string enableDebug = "false";

    //if ( VLOG_IS_ON(1) ){
    if ( loggerLevel >= 1 ){
        enableDebug = "true";
    }
    defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, enableDebug);

    //if ( VLOG_IS_ON(5) ) {
    if ( loggerLevel >= 5 ) {
        defaultConf.set(el::Level::Global, el::ConfigurationType::Format, 
                "%datetime %logger %level [%fbase:%line %func]: %msg");
    } else if ( VLOG_IS_ON(1) ) {
        defaultConf.set(el::Level::Global, el::ConfigurationType::Format, 
                "%datetime %logger %level [%fbase:%line %func]: %msg");
    } else {
        defaultConf.set(el::Level::Global, el::ConfigurationType::Format, 
                "%datetime %logger %level [%fbase:%line]: %msg");
    }

    el::Loggers::reconfigureLogger("default", defaultConf);

    el::Loggers::addFlag(el::LoggingFlag::NewLineForContainer);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
    //el::Loggers::setVerboseLevel(5);
}


    //TIMED_FUNC(timerOpenOFD);
    //TIMED_SCOPE(timerOpenPage, "Open OFD Page.");

