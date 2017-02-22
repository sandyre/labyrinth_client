//
//  playerinfo.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.02.17.
//
//

#include "playerinfo.hpp"

uint32_t
PlayerInfo::GetUID() const
{
    return m_nUID;
}

void
PlayerInfo::SetUID(uint32_t uid)
{
    m_nUID = uid;
}

std::string
PlayerInfo::GetNickname() const
{
    return m_sNickname;
}

void
PlayerInfo::SetNickname(const std::string& name)
{
    m_sNickname = name;
}
