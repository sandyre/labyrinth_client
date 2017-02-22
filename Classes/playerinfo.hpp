//
//  playerinfo.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.02.17.
//
//

#ifndef playerinfo_hpp
#define playerinfo_hpp

#include <string>
#include <cocos2d.h>

class PlayerInfo
{
public:
    static PlayerInfo& Instance()
    {
        static PlayerInfo pi;
        return pi;
    }
    
    uint32_t    GetUID() const;
    void        SetUID(uint32_t);
    
    std::string GetNickname() const;
    void        SetNickname(const std::string&);
private:
    PlayerInfo()
    {
        m_nUID = cocos2d::RandomHelper::random_int(0, 666666);
        m_sNickname = "sandyre";
    }
    ~PlayerInfo() {}
    
    PlayerInfo(PlayerInfo const&) = delete;
    PlayerInfo& operator=(PlayerInfo const&) = delete;
    
private:
    std::string m_sNickname;
    uint32_t    m_nUID;
};

#endif /* playerinfo_hpp */
