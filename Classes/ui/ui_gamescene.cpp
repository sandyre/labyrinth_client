//
//  ui_gamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 09.04.17.
//
//

#include "ui_gamescene.hpp"

#include "../resources.hpp"
#include "game/client/units/hero.hpp"
USING_NS_CC;


UIGameScene::UIGameScene()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visible_size);
    
        // stats layout init
    auto stats_size = visible_size;
    stats_size.height *= 0.25;
    stats_size.width *= 0.5;
    
    auto stats_pos = ui::RelativeLayoutParameter::create();
    stats_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);

    _heroStats = new UIHeroStats();
    _heroStats->setLayoutParameter(stats_pos);

    this->addChild(_heroStats);

        // init take item button
    auto take_item_pos = ui::RelativeLayoutParameter::create();
    take_item_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_pTakeItemButton = ui::Button::create("res/ui/ui_icons/ui_icon_ability_takeItem.png");
    m_pTakeItemButton->setLayoutParameter(take_item_pos);
    this->addChild(m_pTakeItemButton);
    
        // init battle logs
    auto battle_logs_pos = ui::RelativeLayoutParameter::create();
    battle_logs_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    
    m_pBattleLogs = new UIBattleLogs();
    m_pBattleLogs->setLayoutParameter(battle_logs_pos);
    this->addChild(m_pBattleLogs);
    
        // init 'skillspanel'
    auto skills_panel_pos = ui::RelativeLayoutParameter::create();
    skills_panel_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_poSkillsPanel = new UISpellsPanel();
    m_poSkillsPanel->setLayoutParameter(skills_panel_pos);
    this->addChild(m_poSkillsPanel);
    
        // init 'battleview'
    auto battle_view_pos = ui::RelativeLayoutParameter::create();
    battle_view_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    battle_view_pos->setMargin(ui::Margin(0,
                                          visible_size.height * 0.14,
                                          0,
                                          0));
    
    m_poBattleView = new UIBattleView();
    m_poBattleView->setLayoutParameter(battle_view_pos);
    this->addChild(m_poBattleView);
}

void
UIGameScene::ConfigureForHero(Hero * player)
{
        // init warrior UI
    if(player->GetType() == Hero::Type::WARRIOR)
    {
            // init skills panel
            // init dash
        m_poSkillsPanel->CreateSpell("res/ui/ui_icons/ui_icon_ability_warrior_speedUp.png");
        
            // init battleview
        auto atk_action = m_poBattleView->m_poActionsView->CreateAction();
        atk_action->SetIcon("res/ui/ui_icons/ui_icon_ability_warrior_attack.png");
        
        auto dash_action = m_poBattleView->m_poActionsView->CreateAction();
        dash_action->SetIcon("res/ui/ui_icons/ui_icon_ability_warrior_block.png");
    }
    else if(player->GetType() == Hero::Type::MAGE)
    {
            // init skills panel
            // init teleport
        m_poSkillsPanel->CreateSpell("res/ui/ui_icons/ui_icon_ability_mage_teleport.png");
        
            // init battleview
        auto atk_action = m_poBattleView->m_poActionsView->CreateAction();
        atk_action->SetIcon("res/ui/ui_icons/ui_icon_ability_mage_attack.png");
        
        auto freeze_act = m_poBattleView->m_poActionsView->CreateAction();
        freeze_act->SetIcon("res/ui/ui_icons/ui_icon_ability_mage_freeze.png");
    }
}

UIHeroStats::UIHeroStats()
{
    auto visible_size = Director::getInstance()->getVisibleSize();

    auto layout_size = visible_size;
    layout_size.width *= 0.55;
    layout_size.height *= 0.137;

    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(layout_size);

        // HP
    {
        auto back_pos = ui::RelativeLayoutParameter::create();
        back_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);

        _hpBarBack = ui::ImageView::create("res/ui/ui_bars/bar_hp_back.png");
        _hpBarBack->setLayoutParameter(back_pos);

        this->addChild(_hpBarBack);

        auto front_pos = ui::RelativeLayoutParameter::create();
        front_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
        front_pos->setMargin(ui::Margin(visible_size.width * 0.11,
                                        visible_size.height * 0.0083,
                                        0,
                                        0));

        _hpBarFront = ui::LoadingBar::create("res/ui/ui_bars/bar_hp.png");
        _hpBarFront->setLayoutParameter(front_pos);

        this->addChild(_hpBarFront);

        auto text_pos = ui::RelativeLayoutParameter::create();
        text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
        text_pos->setMargin(ui::Margin(visible_size.width * 0.29,
                                       visible_size.height * 0.01,
                                       0,
                                       0));

        _hpBarText = ui::Text::create("0/0",
                                      TempestaFive,
                                      26);
        _hpBarText->enableShadow();
        _hpBarText->setLayoutParameter(text_pos);

        this->addChild(_hpBarText);
    }

        // Phys damage
    {
        auto icon_pos = ui::RelativeLayoutParameter::create();
        icon_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        icon_pos->setMargin(ui::Margin(0,
                                       0,
                                       0,
                                       visible_size.height * 0.039));

        _physDamageIcon = ui::ImageView::create("res/ui/ui_icons/ui_stats_icon_atk.png");
        _physDamageIcon->setLayoutParameter(icon_pos);

        this->addChild(_physDamageIcon);

        auto text_pos = ui::RelativeLayoutParameter::create();
        text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        text_pos->setMargin(ui::Margin(visible_size.width * 0.069,
                                       0,
                                       0,
                                       visible_size.height * 0.039));

        _physDamageText = ui::Text::create("0",
                                           TempestaFive,
                                           22);
        _physDamageText->setLayoutParameter(text_pos);

        this->addChild(_physDamageText);
    }

        // Mag damage
    {
        auto icon_pos = ui::RelativeLayoutParameter::create();
        icon_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);

        _magDamageIcon = ui::ImageView::create("res/ui/ui_icons/ui_stats_icon_spl.png");
        _magDamageIcon->setLayoutParameter(icon_pos);

        this->addChild(_magDamageIcon);

        auto text_pos = ui::RelativeLayoutParameter::create();
        text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        text_pos->setMargin(ui::Margin(visible_size.width * 0.069,
                                       0,
                                       0,
                                       0));

        _magDamageText = ui::Text::create("0",
                                          TempestaFive,
                                          22);
        _magDamageText->setLayoutParameter(text_pos);

        this->addChild(_magDamageText);
    }

        // Armor
    {
        auto icon_pos = ui::RelativeLayoutParameter::create();
        icon_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        icon_pos->setMargin(ui::Margin(visible_size.width * 0.22,
                                       0,
                                       0,
                                       visible_size.height * 0.039));

        _armorIcon = ui::ImageView::create("res/ui/ui_icons/ui_stats_icon_def.png");
        _armorIcon->setLayoutParameter(icon_pos);

        this->addChild(_armorIcon);

        auto text_pos = ui::RelativeLayoutParameter::create();
        text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        text_pos->setMargin(ui::Margin(visible_size.width * 0.29,
                                       0,
                                       0,
                                       visible_size.height * 0.039));

        _armorText = ui::Text::create("0",
                                      TempestaFive,
                                      22);
        _armorText->setLayoutParameter(text_pos);

        this->addChild(_armorText);
    }

        // Mag resistance
    {
        auto icon_pos = ui::RelativeLayoutParameter::create();
        icon_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        icon_pos->setMargin(ui::Margin(visible_size.width * 0.22,
                                       0,
                                       0,
                                       0));

        _magResistIcon = ui::ImageView::create("res/ui/ui_icons/ui_stats_icon_rst.png");
        _magResistIcon->setLayoutParameter(icon_pos);

        this->addChild(_magResistIcon);

        auto text_pos = ui::RelativeLayoutParameter::create();
        text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        text_pos->setMargin(ui::Margin(visible_size.width * 0.29,
                                       0,
                                       0,
                                       0));

        _magResistText = ui::Text::create("0",
                                          TempestaFive,
                                          22);
        _magResistText->setLayoutParameter(text_pos);

        this->addChild(_magResistText);
    }
}

void
UIHeroStats::SetHP(int val, int max)
{
    _hpBarText->setString(StringUtils::format("%d/%d", val, max));
    _hpBarFront->setPercent((float)val/max * 100.0f);
}

void
UIHeroStats::SetArmor(int val)
{ _armorText->setString(std::to_string(val)); }

void
UIHeroStats::SetMagResistance(int val)
{ _magResistText->setString(std::to_string(val)); }

void
UIHeroStats::SetPhysicalDamage(int val)
{ _physDamageText->setString(std::to_string(val)); }

void
UIHeroStats::SetMagicalDamage(int val)
{ _magDamageText->setString(std::to_string(val)); }

UISpellsPanel::UISpellsPanel()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.width *= 0.2;
    layout_size.height *= 0.3;
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    this->setContentSize(layout_size);
}

void
UISpellsPanel::CreateSpell(const std::string& icon)
{
    auto skill_pos = ui::LinearLayoutParameter::create();
    skill_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    
    auto skill_button = ui::Button::create(icon);
    skill_button->setLayoutParameter(skill_pos);
    this->addChild(skill_button);
    m_aSkillsButtons.push_back(skill_button);
}

UIBattleLogs::UIBattleLogs()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.width *= 0.44;
    layout_size.height *= 0.4;
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(layout_size);
    this->setTouchEnabled(false);
    this->setEnabled(false);

        // init textfield
    auto textf_pos = ui::RelativeLayoutParameter::create();
    textf_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pListView = ui::ListView::create();
    m_pListView->setContentSize(layout_size);
    m_pListView->setLayoutParameter(textf_pos);
    this->addChild(m_pListView);
}

void
UIBattleLogs::AddLogMessage(const std::string& msg)
{
    auto text_msg = ui::Text::create(msg,
                                     "fonts/pw_extended.ttf",
                                     12);
    text_msg->setCameraMask((unsigned short)CameraFlag::USER1);
    text_msg->setOpacity(0);
    m_pListView->insertCustomItem(text_msg, 0);
    
    auto fadein = FadeIn::create(0.5f);
    auto delay = DelayTime::create(5.0f);
    auto fadeout = FadeOut::create(0.5f);
    auto seq = Sequence::create(fadein,
                                delay,
                                fadeout,
                                RemoveSelf::create(),
                                nullptr);
    text_msg->runAction(seq);
}

UIBattleView::UIBattleView()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visible_size);
    
        // init 'actionsview'
    auto act_view_pos = ui::RelativeLayoutParameter::create();
    act_view_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_poActionsView = new UIActionsView();
    m_poActionsView->setLayoutParameter(act_view_pos);
    this->addChild(m_poActionsView);
}

void
UIBattleView::setActive(bool val)
{
    this->setEnabled(val);
    this->setVisible(val);
}

UIActionsView::UIActionsView()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.height *= 0.18;
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    this->setContentSize(layout_size);
}

UIAction *
UIActionsView::CreateAction()
{
    auto action_pos = ui::LinearLayoutParameter::create();
    action_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::RIGHT);
    
    auto action = new UIAction();
    action->setLayoutParameter(action_pos);
    this->addChild(action);
    m_apActions.push_back(action);
    
    return action;
}

UIAction::UIAction()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.height *= 0.12;
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(layout_size);
    this->setBackGroundImage("res/ui/battle/btl_tape.png");
    this->setCascadeOpacityEnabled(true);

        // init icon back
    auto icon_back_pos = ui::RelativeLayoutParameter::create();
    icon_back_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);

    _iconBack = ui::ImageView::create("res/ui/ui_combo/ui_combo_skill.png");
    _iconBack->setLayoutParameter(icon_back_pos);

    this->addChild(_iconBack);

        // init main frame
    auto tape_img_pos = ui::RelativeLayoutParameter::create();
    tape_img_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);

    _mainFrame = ui::ImageView::create("res/ui/ui_combo/ui_combo_mainFrame.png");
    _mainFrame->setLayoutParameter(tape_img_pos);

    this->addChild(_mainFrame);

    auto rightframe_pos = ui::RelativeLayoutParameter::create();
    rightframe_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);

    _rightFrame = ui::ImageView::create("res/ui/ui_combo/ui_combo_rightFrame.png");
    _rightFrame->setLayoutParameter(rightframe_pos);

    this->addChild(_rightFrame);
    
        // init sequence
    auto seq_lay_pos = ui::RelativeLayoutParameter::create();
    tape_img_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pSequenceLayout = ui::Layout::create();
    m_pSequenceLayout->setLayoutType(ui::Layout::Type::ABSOLUTE);
    m_pSequenceLayout->setLayoutParameter(seq_lay_pos);
    m_pSequenceLayout->setContentSize(layout_size);
    this->addChild(m_pSequenceLayout);
}

void
UIAction::SetIcon(const std::string& filename)
{
    auto visible_size = Director::getInstance()->getVisibleSize();

    auto icon_pos = ui::RelativeLayoutParameter::create();
    icon_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    icon_pos->setMargin(ui::Margin(visible_size.width * 0.0139,
                                   0,
                                   0,
                                   0));
    
    m_pIcon = ui::ImageView::create(filename);
    m_pIcon->setLayoutParameter(icon_pos);
    this->addChild(m_pIcon);
}

void
UIAction::SetHighlighted(bool val)
{
    this->setOpacity(val ? 255 : 50);
}

void
UIAction::ShiftLeft()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    for(auto symb : m_pSequenceSymbols)
    {
        auto move_left = cocos2d::MoveBy::create(0.2,
                                                 cocos2d::Vec2(-visibleSize.width * 0.139,
                                                               0));
        symb->runAction(move_left);
    }
}

void
UIAction::Clear()
{
    for(auto symb : m_pSequenceSymbols)
    {
        symb->removeFromParentAndCleanup(true);
    }
    m_pSequenceSymbols.clear();
}

void
UIAction::Fill(InputSequence seq)
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto current_symbol_pos = visibleSize;
    current_symbol_pos.width *= 0.5;
    current_symbol_pos.height *= 0.058;
    
    for(auto i = 0; i < seq.sequence.size(); ++i)
    {
        auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)seq.sequence[i]]);
        symbol->setPosition(current_symbol_pos);
        symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
        m_pSequenceLayout->addChild(symbol);
        m_pSequenceSymbols.push_back(symbol);
        
        current_symbol_pos.width += (visibleSize.width * 0.139);
    }
}
