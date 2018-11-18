#ifndef CONFIGMGR_H
#define CONFIGMGR_H

// There are 3 conditions for this configuration manager to work:
//
// the getConfig/setConfig callbacks need to parse/serialize need to work with whatever is served
// from /cp/ path. That path on SPIFFS should contain a web application that asks for the configuration
// from GET /api/config and then stores it back using a POST /api/config

class ConfigMgr {
  public:
    void start(String hostname, IPAddress ip, String(*getConfig)(), bool(*setConfig)(String));
};

extern ConfigMgr ConfigManager;

#endif
