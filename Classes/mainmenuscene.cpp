//
//  mainmenuscene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 20.01.17.
//
//

#include "mainmenuscene.hpp"

#include "gameconfig.hpp"
#include "msnet_generated.h"
#include "netsystem.hpp"
#include "pregamescene.hpp"
#include "settingsscene.hpp"
#include "toolkit/SafePacketGetter.hpp"

#include <audio/include/AudioEngine.h>
#include <network/HttpRequest.h>
#include <network/HttpResponse.h>
#include <network/HttpClient.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/SAX/AttributesImpl.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/XML/XMLWriter.h>

#include <fstream>
#include <iostream>
#include <istream>
#include <regex>
#include <streambuf>
#include <string>

using namespace MasterMessage;
USING_NS_CC;

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

Scene *
MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    
    return scene;
}

void
MainMenuScene::UpdateNewsWidget()
{
    m_pUI->m_pMainPage->m_pNewsLayout->m_pNewsListView->removeAllChildrenWithCleanup(true);
    m_pUI->m_pMainPage->m_pNewsLayout->m_pNewsArray.clear();
    
    network::HttpRequest * news_req = new network::HttpRequest();
    news_req->setUrl("https://www.hate-red.com/newsfeed.xml");
    news_req->setRequestType(network::HttpRequest::Type::GET);
    news_req->setResponseCallback([=](network::HttpClient * pClient, network::HttpResponse * pResp)
                                  {
                                      if(pResp == nullptr)
                                          return;
                                      
                                      if(pResp->getResponseCode() == 200 && pResp->getResponseData() != nullptr)
                                      {
                                          using namespace Poco::XML;
                                          using Poco::AutoPtr;
                                          
                                          membuf data_buf(pResp->getResponseData()->data(),
                                                          pResp->getResponseData()->data() + pResp->getResponseData()->size());
                                          std::istream is(&data_buf);
                                          InputSource source(is);
                                          
                                          Poco::XML::DOMParser parser;
                                          Poco::XML::Document * xmldoc = parser.parse(&source);
                                          
                                          NodeIterator it(xmldoc, NodeFilter::SHOW_ELEMENT);
                                          Poco::XML::Node * pRootNode = it.nextNode();
                                          
                                          while(pRootNode)
                                          {
                                              if(pRootNode->nodeName() == "root")
                                              {
                                                  auto item_ptr = pRootNode->firstChild();
                                                  while(item_ptr = item_ptr->nextSibling())
                                                  {
                                                      auto news_item = static_cast<Element*>(item_ptr);
                                                      if(news_item->attributes() == nullptr)
                                                          continue;
                                                      auto date = news_item->getAttribute("date");
                                                      auto text = news_item->getAttribute("text");
                                                      auto ui_news_item = cocos2d::ui::Text::create(StringUtils::format("<%s> %s",
                                                                                                                        news_item->getAttribute("date").c_str(),
                                                                                                                        news_item->getAttribute("text").c_str()),
                                                                                                    "fonts/alagard.ttf",
                                                                                                    30);
                                                      ui_news_item->setTextHorizontalAlignment(TextHAlignment::LEFT);
                                                      ui_news_item->ignoreContentAdaptWithSize(false);
                                                      auto visibleSize = Director::getInstance()->getVisibleSize();
                                                      visibleSize.width *= 0.94;
                                                      visibleSize.height *= 0.05;
                                                      ui_news_item->setContentSize(visibleSize);
                                                      m_pUI->m_pMainPage->m_pNewsLayout->m_pNewsListView->pushBackCustomItem(ui_news_item);
                                                      m_pUI->m_pMainPage->m_pNewsLayout->m_pNewsArray.push_back(ui_news_item);
                                                  }
                                              }
                                              pRootNode = it.nextNode();
                                          }
                                      }
                                  });
    news_req->setTag("GET test1");
    network::HttpClient::getInstance()->send(news_req);
    news_req->release();
}

bool
MainMenuScene::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    m_fTimer = 5.0f;
    m_eSceneState = SceneState::START_PAGE;
    
    m_pUI = new UIMainMenuScene();
    this->addChild(m_pUI);
    
        // Get channel
    _channel = NetSystem::Instance().GetChannel("masterserver");
    
    flatbuffers::FlatBufferBuilder builder;
    auto ping = CreateCLPing(builder);
    auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
    auto msg = CreateMessage(builder,
                             uuid,
                             Messages_CLPing,
                             ping.Union());
    builder.Finish(msg);

    _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                              builder.GetBufferPointer() + builder.GetSize()));
    builder.Clear();
    
        // play background sound
//    auto audio = cocos2d::experimental::AudioEngine::play2d("res/audio/main_theme.mp3",
//                                                            true,
//                                                            0.1f);
//    
        // listener to get into login page
    m_pUI->m_pStartPage->m_pStartButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                               {
                                                                   if(type == ui::Widget::TouchEventType::ENDED)
                                                                   {
                                                                       m_eSceneState = SceneState::LOGIN_PAGE;
                                                                       m_pUI->m_pPageView->scrollToPage(1);
                                                                   }
                                                               });
    
        // listener which validates email and password on change
    m_pUI->m_pLoginPage->m_pLogInButton->setEnabled(false);
    m_pUI->m_pLoginPage->m_pRegButton->setEnabled(false);
    auto input_validation_callback = [this](Ref * pSender, ui::TextField::EventType type)
    {
        if(type == ui::TextField::EventType::INSERT_TEXT ||
           type == ui::TextField::EventType::DELETE_BACKWARD)
        {
            if(std::regex_match(m_pUI->m_pLoginPage->m_pMailField->getString(),
                                std::regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+")) &&
               m_pUI->m_pLoginPage->m_pPasswordField->getString().size() >= 4)
            {
                m_pUI->m_pLoginPage->m_pRegButton->setEnabled(true);
                m_pUI->m_pLoginPage->m_pLogInButton->setEnabled(true);
            }
            else
            {
                m_pUI->m_pLoginPage->m_pRegButton->setEnabled(false);
                m_pUI->m_pLoginPage->m_pLogInButton->setEnabled(false);
            }
        }
    };
    m_pUI->m_pLoginPage->m_pMailField->addEventListener(input_validation_callback);
    m_pUI->m_pLoginPage->m_pPasswordField->addEventListener(input_validation_callback);
    
        // listener to get into main page
    auto login_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            auto& log_form = m_pUI->m_pLoginPage;
            
            flatbuffers::FlatBufferBuilder builder;

            auto email_of = builder.CreateString(log_form->m_pMailField->getString());
            auto pass_of = builder.CreateString(log_form->m_pPasswordField->getString());

            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto log_msg = CreateCLLogin(builder,
                                         email_of,
                                         pass_of);
            auto msg = CreateMessage(builder,
                                     uuid,
                                     Messages_CLLogin,
                                     log_msg.Union());
            builder.Finish(msg);

            _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                      builder.GetBufferPointer() + builder.GetSize()));
        }
    };
    m_pUI->m_pLoginPage->m_pLogInButton->addTouchEventListener(login_button_callback);
    
        // listener to get into play pregamescene
    m_pUI->m_pMainPage->m_pPlayButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            flatbuffers::FlatBufferBuilder builder;

            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto req_lobby = CreateCLFindGame(builder,
                                              0/*GameConfiguration::Instance().GetUUID()*/,
                                              GAMEVERSION_MAJOR,
                                              GAMEVERSION_MINOR,
                                              GAMEVERSION_BUILD);
            auto ms_event = CreateMessage(builder,
                                          uuid,
                                          Messages_CLFindGame,
                                          req_lobby.Union());
            builder.Finish(ms_event);
            
            _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                      builder.GetBufferPointer() + builder.GetSize()));
        }
    });
    
        // listener to registration button
    auto reg_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            auto& log_form = m_pUI->m_pLoginPage;
            
            flatbuffers::FlatBufferBuilder builder;
            
            auto email_of = builder.CreateString(log_form->m_pMailField->getString());
            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto pass_of = builder.CreateString(log_form->m_pPasswordField->getString());
            auto reg_msg = CreateCLRegister(builder,
                                            email_of,
                                            pass_of);
            auto msg = CreateMessage(builder,
                                     uuid,
                                     Messages_CLRegister,
                                     reg_msg.Union());
            builder.Finish(msg);
            
            _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                      builder.GetBufferPointer() + builder.GetSize()));
        }
    };
    m_pUI->m_pLoginPage->m_pRegButton->addTouchEventListener(reg_button_callback);
    
        // if autologin is set, fill login page with data from config file
    auto& config = GameConfiguration::Instance();
    if(config.GetPlayerAutologin())
    {
        m_pUI->m_pLoginPage->m_pMailField->setString(config.GetPlayerEmail());
        m_pUI->m_pLoginPage->m_pPasswordField->setString(config.GetPlayerPassword());
        m_pUI->m_pLoginPage->m_pLogInButton->setEnabled(true);
    }
    
    auto settings_button_callback = [this](Ref * pSender, ui::Button::TouchEventType type)
    {
        if(type == ui::Button::TouchEventType::ENDED)
        {
            Director::getInstance()->pushScene(SettingsScene::createScene());
        }
    };
    m_pUI->m_pMainPage->m_pSettingsButton->addTouchEventListener(settings_button_callback);
    
    auto reload_news_callback = [this](Ref * pSender, ui::Button::TouchEventType type)
    {
        if(type == ui::Button::TouchEventType::ENDED)
        {
            this->UpdateNewsWidget();
        }
    };
    m_pUI->m_pMainPage->m_pNewsLayout->m_pReloadButton->addTouchEventListener(reload_news_callback);
    
        // preload newsfeed
    UpdateNewsWidget();
    
    this->scheduleUpdate();
    
    return true;
}

void
MainMenuScene::update(float delta)
{
    if(m_eSceneState == SceneState::START_PAGE)
    {
        if(_channel->Available())
        {
            m_pUI->m_pStartPage->m_pStartButton->setEnabled(true);
            m_pUI->m_pStartPage->m_pStartButton->setTitleText("Begin");
        }
    }
    else if(m_eSceneState == SceneState::LOGIN_PAGE)
    {
        if(_channel->Available())
        {
            SafePacketGetter safeGetter(_channel->native_handler());

            auto packet = safeGetter.Get<MasterMessage::Message>();

            if(!packet)
                return;

            auto message = GetMessage(packet->Data.data());
            
            switch(message->payload_type())
            {
            case MasterMessage::Messages_SVRegister:
            {
                auto response = static_cast<const SVRegister*>(message->payload());
                
                if(response->response() == RegistrationStatus_SUCCESS)
                    MessageBox("Success", "Registration done, check email");
                else
                    MessageBox("Error", "Email has been taken, please take another one");
                
                break;
            }
                
            case MasterMessage::Messages_SVLogin:
            {
                auto response = static_cast<const SVLogin*>(message->payload());
                
                if(response->response() == LoginStatus_SUCCESS)
                {
                    m_eSceneState = SceneState::MAIN_PAGE;
                    m_pUI->m_pPageView->scrollToPage(2);
                }
                else
                    MessageBox("Error", "Wrong email or password");
                
                break;
            }
                
            }
        }
    }
    else if(m_eSceneState == SceneState::MAIN_PAGE)
    {
        if(_channel->Available())
        {
            SafePacketGetter safeGetter(_channel->native_handler());

            auto packet = safeGetter.Get<MasterMessage::Message>();

            if(!packet)
                return;

            auto message = GetMessage(packet->Data.data());
            
            switch(message->payload_type())
            {
            case MasterMessage::Messages_SVGameFound:
            {
                auto gs_info = static_cast<const SVGameFound*>(message->payload());
                
                Poco::Net::SocketAddress adr(GameConfiguration::Instance().GetServerAddress());
                NetSystem::Instance().CreateChannel("gameserver",
                                                    Poco::Net::SocketAddress(adr.host(),
                                                                             gs_info->gs_port()));
                
                Director::getInstance()->pushScene(PreGameScene::createScene());
                
                break;
            }
            }
                    
            }
        }
}
