//
//  playerinfo.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.02.17.
//
//

#include "accountinfo.hpp"

uint32_t
AccountInfo::GetUID() const
{
    return m_nUID;
}

void
AccountInfo::SetUID(uint32_t uid)
{
    m_nUID = uid;
}

std::string
AccountInfo::GetNickname() const
{
    return m_sNickname;
}

void
AccountInfo::SetNickname(const std::string& name)
{
    m_sNickname = name;
}
