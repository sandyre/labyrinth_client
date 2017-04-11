//
//  mainmenuscene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 20.01.17.
//
//

#include "mainmenuscene.hpp"

#include "pregamescene.hpp"
#include "settingsscene.hpp"

#include "gameconfig.hpp"
#include "netsystem.hpp"
#include "msnet_generated.h"
#include <regex>
#include <fstream>
#include <string>
#include <Poco/Net/DatagramSocket.h>

using namespace MasterEvent;
USING_NS_CC;

Scene *
MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    
    return scene;
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
            auto log_msg = CreateCLLogin(builder,
                                                email_of,
                                                pass_of);
            auto msg = CreateMessage(builder,
                                            Messages_CLLogin,
                                            log_msg.Union());
            builder.Finish(msg);
            
            NetSystem::Instance().GetChannel(0).SendBytes(builder.GetBufferPointer(),
                                                          builder.GetSize());
            
            NetSystem::Instance().GetChannel(0).ReceiveBytes();
            auto rmsg = GetMessage(NetSystem::Instance().GetChannel(0).GetBuffer().data());
            if(rmsg->message_type() == Messages_SVLogin)
            {
                auto response = static_cast<const SVLogin*>(rmsg->message());
                
                if(response->response() == LoginStatus_SUCCESS)
                {
                    m_eSceneState = SceneState::MAIN_PAGE;
                    m_pUI->m_pPageView->scrollToPage(2);
                }
                else
                {
                    MessageBox("Error", "Wrong email or password");
                }
            }
        }
    };
    m_pUI->m_pLoginPage->m_pLogInButton->addTouchEventListener(login_button_callback);
    
        // listener to get into play pregamescene
    m_pUI->m_pMainPage->m_pPlayButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                             {
                                                                 if(type == ui::Widget::TouchEventType::ENDED)
                                                                 {
                                                                     Director::getInstance()->pushScene(PreGameScene::createScene());
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
            auto pass_of = builder.CreateString(log_form->m_pPasswordField->getString());
            auto reg_msg = CreateCLRegister(builder,
                                                   email_of,
                                                   pass_of);
            auto msg = CreateMessage(builder,
                                            Messages_CLRegister,
                                            reg_msg.Union());
            builder.Finish(msg);
            
            NetSystem::Instance().GetChannel(0).SendBytes(builder.GetBufferPointer(),
                                                          builder.GetSize());
            
            NetSystem::Instance().GetChannel(0).ReceiveBytes();
            auto rmsg = GetMessage(NetSystem::Instance().GetChannel(0).GetBuffer().data());
            if(rmsg->message_type() == Messages_SVRegister)
            {
                auto response = static_cast<const SVRegister*>(rmsg->message());
                
                if(response->response() == RegistrationStatus_SUCCESS)
                {
                    MessageBox("Success", "Registration done, check email");
                }
                else
                {
                    MessageBox("Error", "Email has been taken, please take another one");
                }
            }
        }
    };
    m_pUI->m_pLoginPage->m_pRegButton->addTouchEventListener(reg_button_callback);
    
        // if autologin is set, fill login page with data from config file
    auto& config = GameConfiguraton::Instance();
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
    
    this->scheduleUpdate();
    
    return true;
}

void
MainMenuScene::update(float delta)
{
    if(m_eSceneState == SceneState::START_PAGE)
    {
        m_fTimer += delta;
        
        if(m_fTimer >= 5.0f)
        {
            m_fTimer = 0.0f;
            
            flatbuffers::FlatBufferBuilder builder;
            auto ping = CreateCLPing(builder);
            auto msg = CreateMessage(builder,
                                     Messages_CLPing,
                                     ping.Union());
            builder.Finish(msg);
            NetSystem::Instance().GetChannel(0).SendBytes(builder.GetBufferPointer(),
                                                          builder.GetSize());
            NetSystem::Instance().GetChannel(0).ReceiveBytes();
            if(NetSystem::Instance().GetChannel(0).GetState() == NetSystem::ChannelState::DR_DONE)
            {
                m_pUI->m_pStartPage->m_pStartButton->setEnabled(true);
                m_pUI->m_pStartPage->m_pStartButton->setTitleText("Begin");
            }
        }
    }
}
