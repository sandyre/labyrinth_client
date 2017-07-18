//
//  gameconfig.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 10.04.17.
//
//

#include "gameconfig.hpp"

#include <cocos2d.h>
#include <Poco/UUIDGenerator.h>
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

GameConfiguration::GameConfiguration()
: _configPath(cocos2d::FileUtils::getInstance()->fullPathForFilename("res/gameconfig.xml"))
{ }

void
GameConfiguration::Load()
{
    using namespace Poco::XML;
    using Poco::AutoPtr;
    
    std::ifstream conf_file(_configPath);
    InputSource source(conf_file);
    
    DOMParser parser;
    _confDoc = parser.parse(&source);
    
    NodeIterator it(_confDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
        // network parsing
    {
        auto network_node = pRootNode->getNodeByPath("network");
        auto masterserver_node = network_node->getNodeByPath("masterserver");
        
        auto masterserver_elem = static_cast<Element*>(masterserver_node);
        _serverAddr += masterserver_elem->getAttribute("host");
        _serverAddr += ":";
        _serverAddr += masterserver_elem->getAttribute("port");
    }
    
        // player profile parsing
    {
        auto player_node = pRootNode->getNodeByPath("player");
        auto player_elem = static_cast<Element*>(player_node);
        
        _name = player_elem->getAttribute("nickname");
        _password = player_elem->getAttribute("password");
        _email = player_elem->getAttribute("email");
        _autologin = player_elem->getAttribute("autologin") == "yes" ? true : false;
        _uuid = player_elem->getAttribute("uuid");

        if(_uuid.size() != 32)
        {
            auto uuid = Poco::UUIDGenerator().createOne();
            SetUUID(uuid.toString());
        }
    }
}

void
GameConfiguration::SetPlayerAutologin(bool value)
{
    using namespace Poco::XML;
    
    NodeIterator it(_confDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("autologin",
                              value ? "yes" : "no");
    Save();
}

void
GameConfiguration::SetPlayerEmail(const std::string& email)
{
    using namespace Poco::XML;
    
        // save to internal data
    _email = email;
    
        // save to DOM
    NodeIterator it(_confDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("email",
                              email);
    Save();
}

void
GameConfiguration::SetPlayerPassword(const std::string& password)
{
    using namespace Poco::XML;
    
        // save to internal data
    _password = password;
    
        // save to DOM
    NodeIterator it(_confDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("password",
                              password);
    Save();
}

void
GameConfiguration::SetPlayerName(const std::string& nickname)
{
    using namespace Poco::XML;
    
        // save to internal data
    _name = nickname;
    
        // save to DOM
    NodeIterator it(_confDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();
    
    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);
    
    player_elem->setAttribute("nickname",
                              nickname);
    Save();
}

void
GameConfiguration::SetUUID(const std::string& uuid)
{
    using namespace Poco::XML;

        // save to internal data
    _uuid = uuid;

        // save to DOM
    NodeIterator it(_confDoc, NodeFilter::SHOW_ELEMENT);
    Node * pRootNode = it.nextNode();

    auto player_node = pRootNode->getNodeByPath("player");
    auto player_elem = static_cast<Element*>(player_node);

    player_elem->setAttribute("uuid",
                              uuid);
    Save();
}

void
GameConfiguration::Save()
{
    using namespace Poco::XML;
    
    std::ofstream conf_file(_configPath);
    Poco::XML::DOMWriter writer;
    writer.setNewLine("\n");
    writer.setOptions(XMLWriter::PRETTY_PRINT);
    writer.writeNode(conf_file, _confDoc);
}
