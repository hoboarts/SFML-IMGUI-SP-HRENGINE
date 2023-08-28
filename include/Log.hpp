
//flexible logging class

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <iostream>
#include <fstream>

#ifndef P_DEBUG_
#define LOG(message, type)
#else
#define LOG(message, type) Logger::Log(message, type);
#endif //P_DEBUG_

#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#endif //_MSC_VER

//Needed to access resources on OS X
#ifdef __APPLE__
#include <ResourcePath.hpp>
#endif

class Logger final
{
public:
    enum class Output
    {
        Console,
        File,
        All
    };

    enum class Type
    {
        Info,
        Warning,
        Error
    };

    //logs a message to a given destination.
    //message: message to log
    //type: whether this message gets tagged as information, a warning or an error
    //output: can be the console via cout, a log file on disk, or both
    static void Log(const std::string& message, Type type = Type::Info, Output output = Output::Console)
    {
        std::string outstring;
        switch (type)
        {
        case Type::Info:
        default:
            outstring = "INFO: " + message;
            break;
        case Type::Error:
            outstring = "ERROR: " + message;
            break;
        case Type::Warning:
            outstring = "WARNING: " + message;
            break;
        }

        if (output == Output::Console || output == Output::All)
        {
            (type == Type::Error) ?
                std::cerr << outstring << std::endl
                :
                std::cout << outstring << std::endl;
#ifdef _MSC_VER
            outstring += "\n";
            OutputDebugString(outstring.c_str());
#endif //_MSC_VER
        }
        if (output == Output::File || output == Output::All)
        {
            std::string resPath("");
            //if it's OS X, prepend the resourcePath
#ifdef __APPLE__
            resPath = resourcePath();
#endif

            //output to a log file
            std::ofstream file(resPath + "output.log", std::ios::app);
            if (file.good())
            {
                file << outstring << std::endl;
                file.close();
            }
            else
            {
                Log(message, type, Output::Console);
                Log("Above message was intended for log file. Opening file probably failed.", Type::Warning, Output::Console);
            }
        }
    }

};

#endif //LOGGER_HPP_