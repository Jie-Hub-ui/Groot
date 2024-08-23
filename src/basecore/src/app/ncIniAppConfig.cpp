#include "app/ncIniAppConfig.h"
#include "lang/Exception.h"
#include <fstream>

using namespace cpp::lang;

static void RemoveTrailLine(std::string &buffer)
{
    if (buffer.size() == 0) { return; }

    std::string tmpBuffer = buffer;
    buffer.clear();
    for (size_t i = 0; i < tmpBuffer.size(); ++i) {
        if ((tmpBuffer[i] == '\n') || (tmpBuffer[i] == '\r')) {
            continue;
        }
        buffer += tmpBuffer[i];
    }
}

static void TrimHeader(std::string &buffer)
{
    if (buffer.size() == 0) { return; }

    std::string tmpBuffer = buffer;
    buffer.clear();
    size_t i = 0;
    for (; i < tmpBuffer.size(); ++i) {
        if (tmpBuffer[i] != ' ') {
            break;
        }
    }
    for (; i < tmpBuffer.size(); ++i) {
        buffer += tmpBuffer[i];
    }
}

static void TrimTrail(std::string &buffer)
{
    if (buffer.size() == 0) { return; }

    std::string tmpBuffer = buffer;
    buffer.clear();
    size_t index = tmpBuffer.size() - 1;
    do {
        if (tmpBuffer[index] != ' ') {
            break;
        }
        if (index == 0) {
            return;
        }
        --index;
    } while(true);

    for (int i = 0; i <= index; ++i) {
        buffer += tmpBuffer[i];
    }
}

static void TrimAll(std::string &buffer)
{
    if (buffer.size() == 0) { return; }

    std::string tmpBuffer = buffer;
    buffer.clear();
    for (size_t i = 0; i < tmpBuffer.size(); ++i) {
        if (tmpBuffer[i] != ' ') {
            buffer += tmpBuffer[i];
        }
    }
}

static bool parseBoolValue(const std::string &value)
{
    try {
        long n = std::stol(value);
        return n != 0;
    }
    catch(...) {

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

ncIniAppConfig::ncIniAppConfig()
    : _hasLoaded(false)
    , _configFile("")
{

}

ncIniAppConfig::ncIniAppConfig(const std::string &fileName)
    : _hasLoaded(false)
    , _configFile(fileName)
{

}

ncIniAppConfig::~ncIniAppConfig()
{

}

void ncIniAppConfig::Load(const std::string &filePath)
{
    _hasLoaded = false;
    _configFile = filePath;
    _valueMap.clear();
    Load();
}

void ncIniAppConfig::Load(void)
{
    if (_hasLoaded) { return; }
    std::cout << "Begin Load config: " << _configFile << "..." << std::endl;
    std::ifstream file(_configFile.c_str(), std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        throw Exception("Cannot open config file failed.");
    }

    std::string buffer;
    std::string sectionKey;
    while (std::getline(file, buffer)) {
        RemoveTrailLine(buffer);

        TrimHeader(buffer);

        TrimTrail(buffer);

        if (buffer.size() == 0) {
            continue;
        }
        else if (buffer[0] == '#') {
            continue;
        }
        else if ((buffer[0] == '[') && (buffer[buffer.size() - 1] == ']')) {
            sectionKey = buffer.substr(buffer.find("[") + 1, buffer.find("]") - 1);
            continue;
        }
        else if (!sectionKey.empty()) {
            TrimAll(buffer);
            std::string::size_type pos = buffer.find("=");
            if (pos == std::string::npos) {
                continue;
            }

            std::string key = buffer.substr(0, pos);
            std::string value = buffer.substr(pos + 1);
            _valueMap[sectionKey + "." + key] = value;
            continue;
        }
    }
}

void ncIniAppConfig::Save()
{

}

void ncIniAppConfig::Save(const std::string &newKey, const std::string &newValue)
{
    _valueMap[newKey] = newValue;
}

bool ncIniAppConfig::Exists(void) const
{
    if (_configFile.empty()) {
        return false;
    }

    return std::ifstream(_configFile).good();
}

void ncIniAppConfig::SetRawValue(const std::string &key, const std::string &value)
{
    _valueMap[key] = value;
}

bool ncIniAppConfig::GetRawValue (const std::string &key, std::string &value) const
{
    ncKeyValueMap::const_iterator iter = _valueMap.find(key);
    if (iter != _valueMap.end ()) {
        value = iter->second;
        return true;
    }

    return false;
}