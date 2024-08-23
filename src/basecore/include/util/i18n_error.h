#ifndef __NC_I18N_ERROR_H__
#define __NC_I18N_ERROR_H__
#include <string>
#include <map>

namespace cpp {
    namespace util {
        // internationalization error definition
        class I18nError
        {
        public:
            I18nError()
            {

            }

            I18nError(const std::string &code)
                : _code(code)
            {

            }

            I18nError(const std::string &code, const std::map<std::string, std::string> &para)
                : _code(code)
                , _para(para)
            {

            }

            I18nError(const I18nError &o)
                : _code(o._code)
                , _para(o._para)
            {

            }

            void push_para(const std::string &key, const std::string &val)
            {
                _para[key] = val;
            }

            std::string code() const
            {
                return _code;
            }

            std::map<std::string, std::string> para() const
            {
                return _para;
            }

        private:
            std::string                         _code;
            std::map<std::string, std::string>  _para;
        };

    } // namespace util
} // namespace cpp

#endif // !__NC_I18N_ERROR_H__