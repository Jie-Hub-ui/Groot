#ifndef __NC_INI_APP_CONFIG_H__
#define __NC_INI_APP_CONFIG_H__
#include "app/ncIAppConfig.h"
#include <map>

class ncIniAppConfig : public ncIAppConfig
{
public:
    ncIniAppConfig();
    ncIniAppConfig(const std::string &fileName);
    virtual ~ncIniAppConfig();

public:
    virtual void Load();
    virtual void Load(const std::string &filePath);
    virtual void Save();
    virtual void Save(const std::string &newKey, const std::string &newValue);
    virtual bool Exists(void) const;

protected:
    virtual void SetRawValue(const std::string &key, const std::string &value);
    virtual bool GetRawValue (const std::string &key, std::string &value) const;

public:
    bool                                        _hasLoaded;
    std::string                                 _configFile;
    typedef std::map<std::string, std::string>  ncKeyValueMap;
    ncKeyValueMap                               _valueMap;

};

#endif // !__NC_INI_APP_CONFIG_H__