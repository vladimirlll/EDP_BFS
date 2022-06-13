#include "filehandleexception.h"

Exceptions::FileHandleException::FileHandleException(const std::string exText) : std::runtime_error(exText)
{

}
