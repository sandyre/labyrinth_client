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
#include <fstream>
#include <cocos2d.h>

class AccountInfo
{
public:
    static AccountInfo& Instance()
    {
        static AccountInfo pi;
        return pi;
    }
    
    uint32_t    GetUID() const;
    void        SetUID(uint32_t);
    
    std::string GetNickname() const;
    void        SetNickname(const std::string&);
private:
    AccountInfo()
    {
        std::ifstream   file("res/player_info.txt");
        m_nUID = cocos2d::RandomHelper::random_int(0, 666666);
        file >> m_sNickname;
    }
    ~AccountInfo() {}
    
    AccountInfo(AccountInfo const&) = delete;
    AccountInfo& operator=(AccountInfo const&) = delete;
    
private:
    std::string m_sNickname;
    uint32_t    m_nUID;
};

#endif /* playerinfo_hpp */
