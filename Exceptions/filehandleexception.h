#ifndef FILEHANDLEEXCEPTION_H
#define FILEHANDLEEXCEPTION_H

#include <QObject>
#include <stdexcept>

namespace Exceptions
{
    class FileHandleException : public std::runtime_error
    {
    public:
        FileHandleException(const std::string exText);
    };
}

#endif // FILEHANDLEEXCEPTION_H
