#ifndef __NC_INTERFACE_APP_CONFIG_H__
#define __NC_INTERFACE_APP_CONFIG_H__
#include <iostream>
#include <map>

class ncIAppConfig
{
public:
    ncIAppConfig();
    virtual ~ncIAppConfig();

public:
    virtual void Load() = 0;
    virtual void Load(const std::string &filePath) = 0;
    virtual void Save() = 0;
    virtual void Save(const std::string &newKey, const std::string &newValue) = 0;
    virtual bool Exists(void) const = 0;

public:
    virtual bool HasValue(const std::string &key) const;

    virtual int GetIntValue(const std::string &key);
    virtual long GetInt64Value(const std::string &key);
    virtual unsigned long GetUInt64Value(const std::string &key);
    virtual bool GetBoolValue(const std::string &key);
    virtual std::string GetStringValue(const std::string &key);
   
    virtual void SetStringValue(const std::string &key, const std::string &value);
    virtual void SetIntValue(const std::string &key, const int &value);
    virtual void SetUIntValue(const std::string &key, const unsigned int &value);
    virtual void SetInt64Value(const std::string &key, const long &value);
    virtual void SetUInt64Value(const std::string &key, const unsigned long &value);
    virtual void SetBoolValue(const std::string &key, const bool &value);

protected:
    virtual void SetRawValue(const std::string &key, const std::string &value) = 0;
    virtual bool GetRawValue(const std::string &key, std::string &value) const = 0;

};

#endif // !__NC_INTERFACE_APP_CONFIG_H__