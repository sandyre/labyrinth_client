//
//  gameconfig.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 10.04.17.
//
//

#ifndef gameconfig_hpp
#define gameconfig_hpp

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
    
    bool    GetPlayerAutologin() const
    {
        return m_bPlayerAutologin;
    }
    
    const std::string& GetPlayerEmail() const
    {
        return m_sPlayerEmail;
    }
    
    const std::string& GetPlayerPassword() const
    {
        return m_sPlayerPassword;
    }
    
    const std::string& GetPlayerName() const
    {
        return m_sPlayerName;
    }
    
private:
    GameConfiguraton();
    
    std::string m_sServerAddress;
    
    bool        m_bPlayerAutologin;
    std::string m_sPlayerEmail;
    std::string m_sPlayerPassword;
    std::string m_sPlayerName;
    
    std::string m_sConfigFilepath;
};

#endif /* gameconfig_hpp */
