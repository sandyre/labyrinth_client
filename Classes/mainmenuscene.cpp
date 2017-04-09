    //
    //  mainmenuscene.cpp
    //  labyrinth
    //
    //  Created by Aleksandr Borzikh on 20.01.17.
    //
    //

#include "mainmenuscene.hpp"

#include "pregamescene.hpp"

#include "accountinfo.hpp"
#include "netsystem.hpp"
#include "msnet_generated.h"
#include <fstream>
#include <string>
#include <Poco/Net/DatagramSocket.h>

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
    
    m_pUI = new UIMainMenuScene();
    this->addChild(m_pUI);
    
        // listener to get into login page
    m_pUI->m_pStartPage->m_pStartButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                               {
                                                                   if(type == ui::Widget::TouchEventType::ENDED)
                                                                   {
                                                                       m_pUI->m_pPageView->scrollToPage(1);
                                                                   }
                                                               });
    
        // listener to get into main page
    m_pUI->m_pLoginPage->m_pLogInButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
       {
           if(type == ui::Widget::TouchEventType::ENDED)
           {
               auto& log_form = m_pUI->m_pLoginPage;
               
               flatbuffers::FlatBufferBuilder builder;
               
               auto email_of = builder.CreateString(log_form->m_pMailField->getString());
               auto pass_of = builder.CreateString(log_form->m_pPasswordField->getString());
               auto log_msg = MSNet::CreateCLLogin(builder,
                                                   email_of,
                                                   pass_of);
               auto msg = MSNet::CreateMSEvent(builder,
                                               MSNet::MSEvents_CLLogin,
                                               log_msg.Union());
               builder.Finish(msg);
               
               NetSystem::Instance().GetChannel(0).SendBytes(builder.GetBufferPointer(),
                                                             builder.GetSize());
               
               NetSystem::Instance().GetChannel(0).ReceiveBytes();
               auto rmsg = MSNet::GetMSEvent(NetSystem::Instance().GetChannel(0).GetBuffer().data());
               if(rmsg->event_type() == MSNet::MSEvents_SVLogin)
               {
                   auto response = static_cast<const MSNet::SVLogin*>(rmsg->event());
                   
                   if(response->response() == MSNet::LoginStatus_SUCCESS)
                   {
                       m_pUI->m_pPageView->scrollToPage(2);
                   }
                   else
                   {
                       MessageBox("Error", "Wrong email or password");
                   }
               }
           }
       });
    
        // listener to get into play pregamescene
    m_pUI->m_pMainPage->m_pPlayButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                             {
                                                                 if(type == ui::Widget::TouchEventType::ENDED)
                                                                 {
                                                                     Director::getInstance()->pushScene(PreGameScene::createScene());
                                                                 }
                                                             });
    
        // listener to registration button
    m_pUI->m_pLoginPage->m_pRegButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
     {
         if(type == ui::Widget::TouchEventType::ENDED)
         {
             auto& log_form = m_pUI->m_pLoginPage;
             
             flatbuffers::FlatBufferBuilder builder;
             
             auto email_of = builder.CreateString(log_form->m_pMailField->getString());
             auto pass_of = builder.CreateString(log_form->m_pPasswordField->getString());
             auto reg_msg = MSNet::CreateCLRegister(builder,
                                                    email_of,
                                                    pass_of);
             auto msg = MSNet::CreateMSEvent(builder,
                                             MSNet::MSEvents_CLRegister,
                                             reg_msg.Union());
             builder.Finish(msg);
             
             NetSystem::Instance().GetChannel(0).SendBytes(builder.GetBufferPointer(),
                                                           builder.GetSize());
             
             NetSystem::Instance().GetChannel(0).ReceiveBytes();
             auto rmsg = MSNet::GetMSEvent(NetSystem::Instance().GetChannel(0).GetBuffer().data());
             if(rmsg->event_type() == MSNet::MSEvents_SVRegister)
             {
                 auto response = static_cast<const MSNet::SVRegister*>(rmsg->event());
                 
                 if(response->response() == MSNet::RegistrationStatus_SUCCESS)
                 {
                     MessageBox("Success", "Registration done, check email");
                 }
                 else
                 {
                     MessageBox("Error", "Email has been taken, please take another one");
                 }
             }
         }
     });
    
    return true;
}
