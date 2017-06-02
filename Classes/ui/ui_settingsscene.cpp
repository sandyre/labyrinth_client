    //
    //  ui_settingsscene.cpp
    //  labyrinth
    //
    //  Created by Aleksandr Borzikh on 11.04.17.
    //
    //

#include "ui_settingsscene.hpp"

#include "resources.hpp"

USING_NS_CC;

UISettingsScene::UISettingsScene()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
        // init layout
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visibleSize);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(100, 68, 53));
    
        // init 'settings' label
    auto settings_label_pos = ui::RelativeLayoutParameter::create();
    settings_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pSettingsLabel = ui::Text::create("Settings",
                                        TitleFont,
                                        50);
    m_pSettingsLabel->setLayoutParameter(settings_label_pos);
    this->addChild(m_pSettingsLabel);
    
        // init 'settings' layout
    auto settings_layout_size = visibleSize;
    settings_layout_size.height *= 0.4;
    settings_layout_size.width *= 0.5;
    
    auto field_size = settings_layout_size;
    field_size.height *= 0.2;
    
    auto settings_layout_pos  = ui::RelativeLayoutParameter::create();
    settings_layout_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pSettingsLayout = ui::Layout::create();
    m_pSettingsLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    m_pSettingsLayout->setContentSize(settings_layout_size);
    m_pSettingsLayout->setLayoutParameter(settings_layout_pos);
    m_pSettingsLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    m_pSettingsLayout->setBackGroundColor(Color3B(50, 50, 50));
    this->addChild(m_pSettingsLayout);
    
        // init 'nickname' label
    auto name_label_pos = ui::LinearLayoutParameter::create();
    name_label_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    
    m_pNicknameLabel = ui::Text::create("Change nickname",
                                        TitleFont,
                                        30);
    m_pNicknameLabel->setLayoutParameter(name_label_pos);
    m_pSettingsLayout->addChild(m_pNicknameLabel);
    
        // init 'nickname' field
    auto name_field_pos = ui::LinearLayoutParameter::create();
    name_field_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    
    m_pNicknameField = ui::TextField::create(" ",
                                             TitleFont,
                                             30);
    m_pNicknameField->setMaxLengthEnabled(true);
    m_pNicknameField->setMaxLength(30);
    m_pNicknameField->setCursorChar('_');
    m_pNicknameField->setCursorEnabled(true);
    m_pNicknameField->setLayoutParameter(name_field_pos);
    m_pNicknameField->setAnchorPoint(Vec2::ZERO);
    m_pNicknameField->setTextAreaSize(field_size);
    m_pSettingsLayout->addChild(m_pNicknameField);
    
        // init back button
    auto back_butt_pos = ui::RelativeLayoutParameter::create();
    back_butt_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    
    m_pBackButton = ui::Button::create("res/ui/buttons/b_big_1.png",
                                       "res/ui/buttons/b_big_1_pressed.png");
    m_pBackButton->setLayoutParameter(back_butt_pos);
    m_pBackButton->setTitleText("Back");
    m_pBackButton->setTitleFontName(TitleFont);
    m_pBackButton->setTitleFontSize(32);
    this->addChild(m_pBackButton);
}
