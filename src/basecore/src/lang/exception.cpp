#include "lang/Exception.h"

namespace cpp {
    namespace lang {
        Exception::Exception(const int &errId,
                             const std::string &errMsg,
                             const int &codeLine,
                             const std::string &fileName)
            : _errId(errId)
            , _codeLine(codeLine)
            , _errMsg(errMsg)
            , _fileName(fileName)
            , _type(MESSAGE)
        {

        }

        Exception::Exception(const std::string &errMsg)
            : _errId(-1)
            , _codeLine(0)
            , _errMsg(errMsg)
            , _fileName("")
            , _type(MESSAGE)
        {

        }

        Exception::Exception(const cpp::util::I18nError &i18nError)
            : _errId(0)
            , _codeLine(0)
            , _errMsg("")
            , _fileName("")
            , _i18nError(i18nError)
            , _type(I18N)
        {

        }

        Exception::~Exception()
        {

        }

        Exception::Exception(const Exception &exp)
            : _errId(exp._errId)
            , _codeLine(exp._codeLine)
            , _errMsg(exp._errMsg)
            , _fileName(exp._fileName)
            , _stackInfo(exp._stackInfo)
            , _i18nError(exp._i18nError)
            , _type(exp._type)
        {

        }

        Exception &Exception::operator= (const Exception &exp)
        {
            _errId     = exp._errId;
            _errMsg    = exp._errMsg;
            _fileName  = exp._fileName;
            _codeLine  = exp._codeLine;
            _stackInfo = exp._stackInfo;
            _i18nError = exp._i18nError;
            _type      = exp._type;

            return *this;
        }
    }
}