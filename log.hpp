#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <fstream>
#include <string>

#include "error.hpp"

class TLogger {
public:
    static void OpenLog(const std::string &path, const unsigned int mode);
    static void CloseLog();
    static std::basic_ostream<char> &Log();
    static void Log(const std::string &action);
    static void LogAction(const std::string &action, bool error = false, int errcode = 0);
    static void LogRequest(const std::string &message);
    static void LogResponse(const std::string &message);

    static void LogError(const TError &e, const std::string &s) {
        if (!e)
            return;

        Log() << " Error(" << e.GetErrorName() << "): " << s << ": " << e.GetMsg() << std::endl;
    }
};

#endif /* __LOG_HPP__ */
