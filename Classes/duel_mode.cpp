//
//  duel_mode.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 19.02.17.
//  Changed by Sergey
//
//

#include "duel_mode.hpp"

DuelMode::DuelMode() :
	m_bComboDone(false),
	m_aCurrentSymbol(0)
{
	auto size = cocos2d::Director::getInstance()->getVisibleSize();
	int pos_x = size.width / 2;
	int pos_y = size.height / 2;
	for (auto& symbol : m_aAttackCombo)
	{
		symbol = (InputMove)cocos2d::RandomHelper::random_int((int)InputMove::_MIN,
			(int)InputMove::_MAX);
		auto sprite = cocos2d::Sprite::create(SpritesPath[(int)symbol]);
		sprite->setPosition(pos_x, pos_y);
		pos_x += sprite->getContentSize().width + 8;
		this->addChild(sprite);
		m_aSymbols.push_back(sprite);
	}

	m_pCount = cocos2d::Label::createWithTTF("0/0",
		"fonts/arial.ttf",
		20);
	m_pCount->setPosition(size.width * 0.5,
		size.height * 0.4);
	this->addChild(m_pCount);

	m_pEnemyInfo = cocos2d::Label::createWithTTF("Enemy: ",
		"fonts/arial.ttf",
		20);
	m_pEnemyInfo->setPosition(size.width * 0.7,
		size.height * 0.8);
	this->addChild(m_pEnemyInfo);
}

void
DuelMode::TakeInput(InputMove move)
{
	if (m_bComboDone)
		return;

	if (move == m_aAttackCombo[m_aCurrentSymbol])
	{
		m_aCurrentSymbol++;

		m_pCount->setString(cocos2d::StringUtils::format("%d/%zu",
			m_aCurrentSymbol,
			m_aAttackCombo.size()));

		if (m_aCurrentSymbol == m_aAttackCombo.size())
		{
			m_bComboDone = true;
		}
		else
		{
			auto size = cocos2d::Director::getInstance()->getVisibleSize();
			int pos_x = size.width / 2;
			int pos_y = size.height / 2;
			for (int i = 0; i < 5; i++)
			{
				m_aSymbols[i]->setPosition(pos_x + (i - m_aCurrentSymbol)*(m_aSymbols[i]->getContentSize().width + 8), pos_y);
				if (i < m_aCurrentSymbol) {
					m_aSymbols[i]->setOpacity(25);
				}
			}
		}
	}
	else
	{
		Reset();
	}
}

void
DuelMode::SetEnemyInfo(std::string name, uint32_t hp)
{
	m_pEnemyInfo->setString(cocos2d::StringUtils::format("Enemy: %s\nHP: %d",
		name.c_str(), hp));
}

void
DuelMode::Reset()
{
	m_aCurrentSymbol = 0;
	m_bComboDone = false;


	auto size = cocos2d::Director::getInstance()->getVisibleSize();
	int pos_x = size.width / 2;
	int pos_y = size.height / 2;
	for (int i = 0; i < m_aAttackCombo.size(); i++)
	{
		m_aAttackCombo[i] = (InputMove)cocos2d::RandomHelper::random_int((int)InputMove::_MIN,
			(int)InputMove::_MAX);
		m_aSymbols[i]->setTexture(SpritesPath[(int)m_aAttackCombo[i]]);
		m_aSymbols[i]->setPosition(pos_x, pos_y);
		m_aSymbols[i]->setOpacity(255);
		pos_x += m_aSymbols[i]->getContentSize().width + 8;
	}

	// FIXME: hardcoded!
	m_pCount->setString(cocos2d::StringUtils::format("0/%zu", m_aAttackCombo.size()));
}

bool
DuelMode::ComboDone() const
{
	return m_bComboDone;
}

void
DuelMode::Show(bool val)
{
	if (val)
	{
		this->setVisible(true);
	}
	else
	{
		this->setVisible(false);
	}
}
