//
//  ui_gamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 09.04.17.
//
//

#include "ui_gamescene.hpp"

#include "../resources.hpp"
#include "../gamelogic/units/hero.hpp"
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
    
    m_pStatsLayout = ui::Layout::create();
    m_pStatsLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    m_pStatsLayout->setContentSize(stats_size);
    this->addChild(m_pStatsLayout);
    
        // init hp
    auto hp_bar_b_pos = ui::RelativeLayoutParameter::create();
    hp_bar_b_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    
    m_pHPBarBack = ui::ImageView::create("res/ui/bars/bar_hp_back.png");
    m_pHPBarBack->setLayoutParameter(hp_bar_b_pos);
    m_pStatsLayout->addChild(m_pHPBarBack);
    
    auto hp_bar_pos = ui::RelativeLayoutParameter::create();
    hp_bar_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    hp_bar_pos->setMargin(ui::Margin(visible_size.width * 0.098,
                                     visible_size.height * 0.0083,
                                     0,
                                     0));
    
    m_pHPBar = ui::LoadingBar::create("res/ui/bars/bar_hp.png");
    m_pHPBar->setLayoutParameter(hp_bar_pos);
    m_pHPBar->setPercent(100.0);
    m_pStatsLayout->addChild(m_pHPBar);
    
    auto hp_text_pos = ui::RelativeLayoutParameter::create();
    hp_text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    hp_text_pos->setMargin(ui::Margin(visible_size.width * 0.43,
                                      visible_size.height * 0.0083,
                                      0,
                                      0));
    
    m_pHPText = ui::Text::create(" ",
                             TitleFont,
                             30);
    m_pHPText->setAnchorPoint(Vec2::ZERO);
    m_pHPText->setLayoutParameter(hp_text_pos);
    
    m_pStatsLayout->addChild(m_pHPText);
    
        // init dmg
    auto arm_pos = ui::RelativeLayoutParameter::create();
    arm_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    arm_pos->setMargin(ui::Margin(10,
                                  40,
                                  0,
                                  0)); // FIXME: calculate the margin
    
    m_pArmor = ui::Text::create("Armor: ",
                                TitleFont,
                                30);
    m_pArmor->setAnchorPoint(Vec2::ZERO);
    m_pArmor->setLayoutParameter(arm_pos);
    m_pStatsLayout->addChild(m_pArmor);
    
        // init selected item
    auto frame_pos = ui::RelativeLayoutParameter::create();
    frame_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_pItemFrame = ui::ImageView::create("res/graphics/frame.png");
    m_pItemFrame->setLayoutParameter(frame_pos);
    this->addChild(m_pItemFrame);
    
    m_pSelectedItem = ui::ImageView::create();
    m_pSelectedItem->setLayoutParameter(frame_pos);
    this->addChild(m_pSelectedItem);
    
        // init take item button
    auto take_item_pos = ui::RelativeLayoutParameter::create();
    take_item_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    
    m_pTakeItemButton = ui::Button::create("res/graphics/take_item.png");
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
    skills_panel_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    
    m_poSkillsPanel = new UISpellsPanel();
    m_poSkillsPanel->setLayoutParameter(skills_panel_pos);
    this->addChild(m_poSkillsPanel);
    
        // init 'battleview'
    auto battle_view_pos = ui::RelativeLayoutParameter::create();
    battle_view_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_poBattleView = new UIBattleView();
    m_poBattleView->setLayoutParameter(battle_view_pos);
    this->addChild(m_poBattleView);
}

void
UIGameScene::ConfigureForHero(Hero * player)
{
        // init warrior UI
    if(player->GetHero() == Hero::Type::WARRIOR)
    {
            // init skills panel
            // init dash
        m_poSkillsPanel->CreateSpell("res/graphics/warrior_spell_1.png");
        
            // init battleview
        auto atk_action = m_poBattleView->m_poActionsView->CreateAction();
        atk_action->SetIcon("res/graphics/warrior_spell_2.png");
        
        auto dash_action = m_poBattleView->m_poActionsView->CreateAction();
        dash_action->SetIcon("res/graphics/warrior_spell_3.png");
    }
    else if(player->GetHero() == Hero::Type::MAGE)
    {
            // init skills panel
            // init teleport
        m_poSkillsPanel->CreateSpell("res/graphics/mage_spell_1.png");
        
            // init battleview
        auto atk_action = m_poBattleView->m_poActionsView->CreateAction();
        atk_action->SetIcon("res/graphics/mage_spell_2.png");
        
        auto freeze_act = m_poBattleView->m_poActionsView->CreateAction();
        freeze_act->SetIcon("res/graphics/mage_spell_3.png");
    }
    else if(player->GetHero() == Hero::Type::ROGUE)
    {
            // init skills panel
            // init invisibility
        m_poSkillsPanel->CreateSpell("res/graphics/rogue_spell_1.png");
        
            // init battleview
        auto atk_action = m_poBattleView->m_poActionsView->CreateAction();
        atk_action->SetIcon("res/graphics/rogue_spell_2.png");
        
        auto poisondagg_act = m_poBattleView->m_poActionsView->CreateAction();
        poisondagg_act->SetIcon("res/graphics/rogue_spell_3.png");
    }
    else if(player->GetHero() == Hero::Type::PRIEST)
    {
            // init skills panel
            // init blinding light
        m_poSkillsPanel->CreateSpell("res/units/priest/spell_1_icon.png");
        
            // init battleview
        auto atk_action = m_poBattleView->m_poActionsView->CreateAction();
        atk_action->SetIcon("res/units/rogue/atk_icon.png");
    }
}

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
    layout_size.width *= 0.5;
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
                                     14);
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
    act_view_pos->setMargin(ui::Margin(0,
                                       visible_size.height * 0.120,
                                       0,
                                       0));
    
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
    layout_size.height *= 0.4;
    
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
    
        // init tape
    auto tape_img_pos = ui::RelativeLayoutParameter::create();
    tape_img_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pTapeImage = ui::ImageView::create("res/ui/battle/btl_mainTape.png");
    m_pTapeImage->setLayoutParameter(tape_img_pos);
    this->addChild(m_pTapeImage);
    
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
    auto icon_pos = ui::RelativeLayoutParameter::create();
    icon_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_pIcon = ui::ImageView::create(filename);
    m_pIcon->setLayoutParameter(icon_pos);
    this->addChild(m_pIcon);
}

void
UIAction::SetHighlighted(bool val)
{
    if(val)
    {
        this->setOpacity(255);
    }
    else
    {
        this->setOpacity(50);
    }
}

void
UIAction::ShiftLeft()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    for(auto symb : m_pSequenceSymbols)
    {
        auto move_left = cocos2d::MoveBy::create(0.2,
                                                 cocos2d::Vec2(-visibleSize.width * 0.2,
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
        
        current_symbol_pos.width += (visibleSize.width * 0.2);
    }
}
