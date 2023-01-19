#pragma once

#define LOGGING_ENABLED

#ifdef LOGGING_ENABLED
#define logPath logger.setLogFile
#define logInfo logger.setLogDetails(__LINE__, __func__, __FILE__, "INFO", 0); logger.log
#define logWarn logger.setLogDetails(__LINE__, __func__, __FILE__, "WARN", 1); logger.log
#define logError logger.setLogDetails(__LINE__, __func__, __FILE__, "ERROR", 2); logger.log
#define logLevel logger.setLogLevel
#else
#define logPath(path)
#define logInfo
#define logWarn
#define logError
#define logLevel
#endif

#ifdef LOGGING_ENABLED
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstring>

#ifdef WIN32
#include <windows.h>
#endif

class Logger {
public:
    Logger(){
#ifdef WIN32
        system("@echo off\nreg add HKEY_CURRENT_USER\\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f");
#endif
        logColors[0] = '2';
        logColors[1] = '3';
        logColors[2] = '1';
    }
    template<typename T>
    void log(T t) {
        content << t;
        printLog();
        logToFile();
        content.str("");
    }
    template<typename T, typename... Args>
    void log(T t, Args... args) {
        content << t;
        log(args...) ;
    }
    void setLogDetails(unsigned int line, const char* name, const char* file, const char* LEVEL, int level){
        this->level = level;
        auto now = std::chrono::system_clock::now();
        currentTime = std::chrono::system_clock::to_time_t(now);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", gmtime(&currentTime));
        sprintf(prefix, "[%s][%5s %24s %34s][%5s]: ", timeBuffer, std::to_string(line).data(), name, getFilename((char*)file), LEVEL);
    }
    void setLogLevel(int loggingLevel){
        this->loggingLevel = loggingLevel;
    }
    void printLog(){
        if(level > loggingLevel)
            std::cout << "\033[3" << logColors[level]  << "m" << prefix << content.str() << "\033[0m" << std::endl;
    }
    void setLogFile(const char* path){
        auto now = std::chrono::system_clock::now();
        currentTime = std::chrono::system_clock::to_time_t(now);
        std::ostringstream stream;
        stream << path << "/" << std::put_time(std::localtime(&currentTime), "%Y-%m-%d_%H-%M-%S") << ".log";
        file = fopen(stream.str().c_str(), "w");
        if(!file)
            std::cout << "\033[31mFailed to create log\n" << stream.str() << "\033[0m" << std::endl;
    }
    ~Logger() {
        if(file)
            fclose(file);
    }
private:
    void logToFile(){
        if(file) {
            fprintf(file, "%s%s%s", prefix, content.str().data(), "\n");
            fflush(file);
        }
    }
    static char* getFilename(char* filename) {
        auto len = strlen(filename);
        char *lastSlash = filename;
        for (auto i = 0; i < len; i++) {
            if (filename[i] == '/' || filename[i] == '\\') {
                lastSlash = filename + i + 1;
            }
        }
        return lastSlash;
    }
    FILE* file;
    std::ostringstream content;
    std::time_t currentTime;
    char prefix[100];
    char timeBuffer[20];
    char logColors[3];
    int level = 0, loggingLevel = 0;
};

static Logger logger;
#endif