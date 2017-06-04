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

class GameConfiguraton
{
public:
    static GameConfiguraton& Instance();
    
    void    Load();
    void    Save();
    
    const std::string& GetServerAddress()
    {
        return m_sServerAddress;
    }
    
    void    SetPlayerAutologin(bool);
    bool    GetPlayerAutologin() const
    {
        return m_bPlayerAutologin;
    }
    
    void    SetPlayerEmail(const std::string&);
    const std::string& GetPlayerEmail() const
    {
        return m_sPlayerEmail;
    }
    
    void    SetPlayerPassword(const std::string&);
    const std::string& GetPlayerPassword() const
    {
        return m_sPlayerPassword;
    }
    
    void    SetPlayerName(const std::string&);
    const std::string& GetPlayerName() const
    {
        return m_sPlayerName;
    }
    
    const uint32_t GetUID() const
    {
        return m_nUID;
    }
    
    const cocos2d::Size GetScreenSize() const
    {
        return m_stScreenSize;
    }
    
private:
    GameConfiguraton();
    
    std::string m_sServerAddress;
    
    bool        m_bPlayerAutologin;
    std::string m_sPlayerEmail;
    std::string m_sPlayerPassword;
    std::string m_sPlayerName;
    uint32_t    m_nUID;
    
    cocos2d::Size m_stScreenSize;
    
    std::string m_sConfigFilepath;
    
    Poco::AutoPtr<Poco::XML::Document> m_pConfDoc;
};

#endif /* gameconfig_hpp */
