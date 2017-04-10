//
//  gameconfig.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 10.04.17.
//
//

#include "gameconfig.hpp"

#include <fstream>
#include <iostream>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/SAX/SAXParser.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/SAX/InputSource.h>

GameConfiguraton&
GameConfiguraton::Instance()
{
    static GameConfiguraton gc;
    return gc;
}

GameConfiguraton::GameConfiguraton() :
m_sConfigFilepath("gameconfig.xml"),
m_bPlayerAutologin(false)
{
    
}

void
GameConfiguraton::Load()
{
    using namespace Poco::XML;
    using Poco::AutoPtr;
    
    std::ifstream conf_file(m_sConfigFilepath);
    InputSource source(conf_file);
    
    DOMParser parser;
    AutoPtr<Document> pDoc = parser.parse(&source);
    
    NodeIterator it(pDoc, NodeFilter::SHOW_ELEMENT);
    Node * pNode = it.nextNode();
    
    while(pNode)
    {
        if(pNode->nodeName() == "masterserver")
        {
            auto attrs = pNode->attributes();
            
            auto s = attrs->getNamedItem("host");
            int a = 0;
        }
        std::cout<<pNode->nodeName()<<":"<< pNode->getNodeValue()<<std::endl;
        auto kik = pNode->attributes();
        pNode = it.nextNode();
    }
}

void
GameConfiguraton::Save()
{
    using namespace Poco::XML;
    
    std::ofstream conf_file(m_sConfigFilepath);
    XMLWriter conf_file_writer(conf_file,
                               XMLWriter::WRITE_XML_DECLARATION |
                               XMLWriter::PRETTY_PRINT);
    
    conf_file_writer.setNewLine("\n");
    conf_file_writer.startDocument();
    
        // start root
    conf_file_writer.startElement("", "settings", "");
    
        // start network
    conf_file_writer.startElement("", "network", "");
    
        // start masterserver addr
    AttributesImpl ms_attr;
    ms_attr.addAttribute("", "", "host", "", m_sServerAddress);
    ms_attr.addAttribute("", "", "port", "", "1930");
    conf_file_writer.startElement("", "masterserver", "", ms_attr);
    
        // end masterserver addr
    conf_file_writer.endElement("", "masterserver", "");
    
        // end network
    conf_file_writer.endElement("", "network", "");
    
        // start player info
    AttributesImpl player_attr;
    player_attr.addAttribute("", "", "email", "", m_sPlayerEmail);
    player_attr.addAttribute("", "", "password", "", m_sPlayerPassword);
    player_attr.addAttribute("", "", "autologin", "", m_bPlayerAutologin ? "yes" : "no");
    player_attr.addAttribute("", "", "nickname", "", m_sPlayerName);
    conf_file_writer.startElement("", "player", "", player_attr);
    
        // end player info
    conf_file_writer.endElement("", "player", "");
    
        // end root
    conf_file_writer.endElement("", "settings", "");
    
    conf_file_writer.endDocument();
}
