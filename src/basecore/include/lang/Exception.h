/***************************************************************************************************
exception.h:	
    C++ Foundation Library header files
    Copyright (c) Eisoo Software, Inc.(2004)

Purpose:
    Exception is the base class of all CFL exception

Author:
    Lei jie

Creating Time:
    2024-8-14
    
***************************************************************************************************/
#ifndef __CPP_LANG_EXCEPTION_H__
#define __CPP_LANG_EXCEPTION_H__
// #include <string>
#include <vector>
#include "util/i18n_error.h"

namespace cpp {
    namespace lang {
        /*
        * The class #Exception is the base class of all exceptions in CFL, Only objects 
        * that are instances of this class (or one of its subclasses) are thrown by 
        * the CFL. Similarly, only this class or one of its subclasses can be the argument 
        * type in a catch clause.
        */
        class Exception
        {
        public:
            enum ExceptionType 
            {
                MESSAGE,
                I18N,
            };
            /*
             * Constructs a new exception with the specified detail message.  
             *
             * @param errId 	The error identifier that cause this exception. The 
             *					default value is NO_MAPPED_ERROR_ID.
             * @param errMsg 	The detail message. The detail message is saved for 
             *					later retrieval by the getMessage() method.
             * @param codeLine	The error file line number. 
             * @param fileName	The error file name. 
             */
            explicit Exception(const int &errId = 0,
                               const std::string &errMsg = "",
                               const int &codeLine = 0,
                               const std::string &fileName = "");

            /*
             * Constructs a new exception with the specified I18nError.  
             *
             * @param i18nError internationalization error definition
             */
            Exception(const std::string &errMsg);

            Exception(const cpp::util::I18nError &i18nError);

            /*
             * Virtual destructor to release the resource
             */
            virtual ~Exception (void);

            /*
             * Intializes this exception with another exception instance.
             */
            Exception(const Exception &exp);

            /*
             * Assigns this exception with another exception instance.
             */
            Exception &operator= (const Exception &exp);

            // Virtual methods
        public:
            /*
             * Returns the message of this exception.
             *
             * @return  the message of this exception.
             */
            virtual std::string GetMessage() const
            {
                return _errMsg;
            }

            /*
             * Retruns the error identifier for this exception.
             * @return the error identifier for this exception.
             */
            virtual int GetErrorId() const
            {
                return _errId;
            }

            virtual void SetI18nError(cpp::util::I18nError &i18nError)
            {
                _i18nError = i18nError;
            }

            virtual const cpp::util::I18nError *GetI18nError () const
            {
                if (_i18nError.code ().empty ()) {
                    return nullptr;
                }
                return &_i18nError;
            }

            /*
             *  get execption file name
             */
            virtual std::string GetFileName (void) const
            {
                return _fileName;
            }

            /*
             *  get execption code line
             */
            virtual int GetCodeLine (void) const
            {
                return _codeLine;
            }

            /*
            * get execption stack info log
            */
            virtual std::vector<std::string> GetStackTrace() const
            {
                return _stackInfo;
            }

            /*
            * set execption stack info log
            */
            virtual void SetStackTrace(const std::vector<std::string> &stackInfo)
            {
                _stackInfo = stackInfo;
            }

            /*
             * exception type
             */
            virtual ExceptionType Type()
            {
                return _type;
            }

        protected:
            void setMessage(const std::string &errMsg)
            {
                _errMsg = errMsg;
            }

            void setErrorId(const int &errId)
            {
                _errId = errId;
            }

        protected:
            int                         _errId;     // A error identifier mapped with this exception.
            int                         _codeLine;  // Number of lines of code error location
            std::string                 _errMsg;    // std::string to represented the exception message
            std::string                 _fileName;  // Code error file
            std::vector<std::string>    _stackInfo; // stack info
            cpp::util::I18nError        _i18nError; // The i18n error info
            ExceptionType               _type;
        }; // End Exception
    } // namespace lang
} // namespace cpp

#endif //__CPP_LANG_EXCEPTION_H__
