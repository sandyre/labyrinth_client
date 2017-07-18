//
//  gameconfig.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 10.04.17.
//
//

#ifndef gameconfig_hpp
#define gameconfig_hpp

#include <cocos2d.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>

#include <string>

class GameConfiguration
{
public:
    static GameConfiguration& Instance()
    {
        static GameConfiguration gc;
        return gc;
    }
    
    void Load();
    void Save();
    
    const std::string& GetServerAddress()
    { return _serverAddr; }
    
    void SetPlayerAutologin(bool);

    bool GetPlayerAutologin() const
    { return _autologin; }
    
    void SetPlayerEmail(const std::string&);

    const std::string& GetPlayerEmail() const
    { return _email; }
    
    void SetPlayerPassword(const std::string&);

    const std::string& GetPlayerPassword() const
    { return _password; }
    
    void SetPlayerName(const std::string&);

    const std::string& GetPlayerName() const
    { return _name; }

    void SetUUID(const std::string&);

    const std::string& GetUUID() const
    { return _uuid; }

private:
    GameConfiguration();
    
    std::string                         _serverAddr;
    std::string                         _configPath;
    Poco::AutoPtr<Poco::XML::Document>  _confDoc;
    
    bool                                _autologin;
    std::string                         _email;
    std::string                         _password;
    std::string                         _name;
    std::string                         _uuid;
};

#endif /* gameconfig_hpp */
