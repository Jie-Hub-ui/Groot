#include "app/ncIAppConfig.h"
#include "lang/Exception.h"

using namespace cpp::lang;

static bool parseBoolValue(const std::string &value)
{
    try {
        long n = std::stol(value);
        return n != 0;
    }
    catch (...) {

    }

    if ((value.compare("true") == 0) || (value.compare("True") == 0) || (value.compare("TRUE") == 0)) {
        return true;
    } else if ((value.compare("false") == 0) || (value.compare("False") == 0) || (value.compare("FALSE") == 0)) {
        return false;
    } else if ((value.compare("yes") == 0) || (value.compare("Yes") == 0) || (value.compare("YES") == 0)) {
        return true;
    } else if ((value.compare("no") == 0) || (value.compare("No") == 0) || (value.compare("NO") == 0)) {
        return false;
    } else if ((value.compare("on") == 0) || (value.compare("On") == 0) || (value.compare("ON") == 0)) {
        return true;
    } else if ((value.compare("off") == 0) || (value.compare("Off") == 0) || (value.compare("OFF") == 0)) {
        return false;
    } else {
        throw Exception("Cannot convert to boolean");
    }
}

ncIAppConfig::ncIAppConfig()
{

}

ncIAppConfig::~ncIAppConfig()
{

}

bool ncIAppConfig::HasValue(const std::string &key) const
{
    std::string value;
    bool existed = GetRawValue(key, value);

    return existed;
}

int ncIAppConfig::GetIntValue(const std::string &key)
{
    std::string value;
    bool existed = GetRawValue(key, value);
    if (existed == false) {
        throw Exception("Config file key [" + key + "] not found.");
    }

    return std::stoi(value);
}

long ncIAppConfig::GetInt64Value(const std::string &key)
{
    std::string value;
    bool existed = GetRawValue(key, value);
    if (existed == false) {
        throw Exception("Config file key [" + key + "] not found.");
    }

    return std::stol(value);
}

unsigned long ncIAppConfig::GetUInt64Value(const std::string &key)
{
    std::string value;
    bool existed = GetRawValue(key, value);
    if (existed == false) {
        throw Exception("Config file key [" + key + "] not found.");
    }

    return std::stoul(value);
}

bool ncIAppConfig::GetBoolValue(const std::string &key)
{
    std::string value;
    bool existed = GetRawValue(key, value);
    if (existed == false) {
        throw Exception("Config file key [" + key + "] not found.");
    }
    
    return parseBoolValue(value);
}

std::string ncIAppConfig::GetStringValue(const std::string &key)
{
    std::string value;
    bool existed = GetRawValue (key, value);
    if (existed == false) {
        throw Exception("Config file key [" + key + "] not found.");
    }

    return value;
}

void ncIAppConfig::SetStringValue (const std::string &key, const std::string &value)
{
    SetRawValue(key, value);
}

void ncIAppConfig::SetIntValue (const std::string &key, const int &value)
{
    SetRawValue(key, std::to_string(value));
}

void ncIAppConfig::SetUIntValue (const std::string &key, const unsigned int &value)
{
    SetRawValue(key, std::to_string(value));
}

void ncIAppConfig::SetInt64Value (const std::string &key, const long &value)
{
    SetRawValue(key, std::to_string(value));
}

void ncIAppConfig::SetUInt64Value (const std::string &key, const unsigned long &value)
{
    SetRawValue(key, std::to_string(value));
}

void ncIAppConfig::SetBoolValue (const std::string &key, const bool &value)
{
    if (value) {
        SetRawValue(key, "true");
    } 
    else {
        SetRawValue(key, "false");
    }
}
