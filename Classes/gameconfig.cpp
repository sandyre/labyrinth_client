//
//  gameconfig.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 10.04.17.
//
//

#include "gameconfig.hpp"

#include <cocos2d.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/XML/XMLWriter.h>

#include <fstream>
#include <iostream>
#include <string>

GameConfiguraton&
GameConfiguraton::Instance()
{
    static GameConfiguraton gc;
    return gc;
}

GameConfiguraton::GameConfiguraton() :
m_bPlayerAutologin(false)
{
    m_sConfigFilepath = cocos2d::FileUtils::getInstance()->fullPathForFilename("res/gameconfig.xml");
}

void
GameConfiguraton::Load()
{
    using namespace Poco::XML;
    using Poco::AutoPtr;
    
    std::ifstream conf_file(m_sConfigFilepath);
    InputSource source(conf_file);
    
    DOMParser parser;
    m_pConfDoc = parser.parse(&source);
    
    NodeIterator it(m_pConfDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
        // graphics parsing
    {
        auto graph_node = pRootNode->getNodeByPath("graphics");
        auto screensize_node = graph_node->getNodeByPath("screensize");
        
        auto screensize_elem = static_cast<Element*>(screensize_node);
        m_stScreenSize.width = std::atoi(screensize_elem->getAttribute("width").c_str());
        m_stScreenSize.height = std::atoi(screensize_elem->getAttribute("height").c_str());
    }
    
        // network parsing
    {
        auto network_node = pRootNode->getNodeByPath("network");
        auto masterserver_node = network_node->getNodeByPath("masterserver");
        
        auto masterserver_elem = static_cast<Element*>(masterserver_node);
        m_sServerAddress += masterserver_elem->getAttribute("host");
        m_sServerAddress += ":";
        m_sServerAddress += masterserver_elem->getAttribute("port");
    }
    
        // player profile parsing
    {
        auto player_node = pRootNode->getNodeByPath("player");
        auto player_elem = static_cast<Element*>(player_node);
        
        m_sPlayerName = player_elem->getAttribute("nickname");
        m_sPlayerPassword = player_elem->getAttribute("password");
        m_sPlayerEmail = player_elem->getAttribute("email");
        m_bPlayerAutologin = player_elem->getAttribute("autologin") == "yes" ? true : false;
        
        m_nUID = cocos2d::RandomHelper::random_int(0, 666666);
    }
}

void
GameConfiguraton::SetPlayerAutologin(bool value)
{
    using namespace Poco::XML;
    
    NodeIterator it(m_pConfDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("autologin",
                              value ? "yes" : "no");
}

void
GameConfiguraton::SetPlayerEmail(const std::string& email)
{
    using namespace Poco::XML;
    
        // save to internal data
    m_sPlayerEmail = email;
    
        // save to DOM
    NodeIterator it(m_pConfDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("email",
                              email);
}

void
GameConfiguraton::SetPlayerPassword(const std::string& password)
{
    using namespace Poco::XML;
    
        // save to internal data
    m_sPlayerPassword = password;
    
        // save to DOM
    NodeIterator it(m_pConfDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("password",
                              password);
}

void
GameConfiguraton::SetPlayerName(const std::string& nickname)
{
    using namespace Poco::XML;
    
        // save to internal data
    m_sPlayerName = nickname;
    
        // save to DOM
    NodeIterator it(m_pConfDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("nickname",
                              nickname);
}

void
GameConfiguraton::Save()
{
    using namespace Poco::XML;
    
    std::ofstream conf_file(m_sConfigFilepath);
    Poco::XML::DOMWriter writer;
    writer.setNewLine("\n");
    writer.setOptions(XMLWriter::PRETTY_PRINT);
    writer.writeNode(conf_file, m_pConfDoc);
}
