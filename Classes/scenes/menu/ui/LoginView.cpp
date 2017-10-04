//
//  LoginView.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#include "scenes/menu/ui/LoginView.hpp"

#include "resources.hpp"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Button;
using cocos2d::ui::Layout;
using cocos2d::ui::Margin;
using cocos2d::ui::RelativeAlign;
using cocos2d::ui::RelativeLayoutParameter;
using cocos2d::ui::Text;
using cocos2d::ui::TextField;
using cocos2d::ui::Widget;


namespace labyrinth {
namespace ui {
namespace impl
{

    bool LoginView::init()
    {
        if (!Layout::init())
            return false;

        auto visibleSize = Director::getInstance()->getVisibleSize();

        this->setCascadeOpacityEnabled(true);
        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);
        this->setPosition(Vec2::ZERO);

        auto viewTitlePos = RelativeLayoutParameter::create();
        viewTitlePos->setAlign(RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
        viewTitlePos->setMargin(Margin(0, visibleSize.height * 0.1, 0, 0));

        _loginViewTitle = Text::create("Authorization", TitleFont, 70);
        _loginViewTitle->setLayoutParameter(viewTitlePos);
        this->addChild(_loginViewTitle);

        auto loginFormSize = cocos2d::Size(visibleSize.width * 0.777, visibleSize.height * 0.304);
        auto fieldSize = cocos2d::Size(loginFormSize.width * 0.8928, loginFormSize.height * 0.1282);

        auto loginFormPos = RelativeLayoutParameter::create();
        loginFormPos->setAlign(RelativeAlign::CENTER_IN_PARENT);

        _mainFormLayout = Layout::create();
        _mainFormLayout->setLayoutType(Layout::Type::RELATIVE);
        _mainFormLayout->setLayoutParameter(loginFormPos);
        _mainFormLayout->setContentSize(loginFormSize);
        _mainFormLayout->setBackGroundImage("res/ui/ui_plates/p_login.png");
        _mainFormLayout->setCascadeOpacityEnabled(true);
        this->addChild(_mainFormLayout);

        
        auto emailLabelPos = RelativeLayoutParameter::create();
        emailLabelPos->setAlign(RelativeAlign::PARENT_TOP_LEFT);
        emailLabelPos->setMargin(Margin(loginFormSize.width * 0.05357, loginFormSize.height * 0.1107, 0, 0));

        _emailFieldLabel = Text::create("Enter mail", TitleFont, 30);
        _emailFieldLabel->setLayoutParameter(emailLabelPos);
        _mainFormLayout->addChild(_emailFieldLabel);


        auto emailFieldPos = RelativeLayoutParameter::create();
        emailFieldPos->setAlign(RelativeAlign::PARENT_TOP_LEFT);
        emailFieldPos->setMargin(Margin(loginFormSize.width * 0.05357, loginFormSize.height * 0.2307, 0, 0));

        _emailField = TextField::create("example@gmail.com", TitleFont, 30);
        _emailField->setMaxLengthEnabled(true);
        _emailField->setMaxLength(30);
        _emailField->setCursorChar('_');
        _emailField->setCursorEnabled(true);
        _emailField->setLayoutParameter(emailFieldPos);
        _emailField->setAnchorPoint(Vec2::ZERO);
        _emailField->setTextAreaSize(fieldSize);
        _mainFormLayout->addChild(_emailField);


        auto passwordLabelPos = RelativeLayoutParameter::create();
        passwordLabelPos->setAlign(RelativeAlign::PARENT_TOP_LEFT);
        passwordLabelPos->setMargin(Margin(loginFormSize.width * 0.05357, loginFormSize.height * 0.4341, 0, 0));

        _passwordFieldLabel = Text::create("Enter password", TitleFont, 30);
        _passwordFieldLabel->setLayoutParameter(passwordLabelPos);
        _mainFormLayout->addChild(_passwordFieldLabel);


        auto passwordFieldPos = RelativeLayoutParameter::create();
        passwordFieldPos->setAlign(RelativeAlign::PARENT_TOP_LEFT);
        passwordFieldPos->setMargin(Margin(loginFormSize.width * 0.05357, loginFormSize.height * 0.5641, 0, 0));

        _passwordField = TextField::create("somepassword", TitleFont, 30);
        _passwordField->setMaxLengthEnabled(true);
        _passwordField->setMaxLength(12);
        _passwordField->setCursorChar('_');
        _passwordField->setCursorEnabled(true);
        _passwordField->setPasswordEnabled(true);
        _passwordField->setPasswordStyleText("*");
        _passwordField->setAnchorPoint(Vec2::ZERO);
        _passwordField->setTextAreaSize(fieldSize);
        _passwordField->setLayoutParameter(passwordFieldPos);
        _mainFormLayout->addChild(_passwordField);


        auto loginButtonPos = RelativeLayoutParameter::create();
        loginButtonPos->setAlign(RelativeAlign::PARENT_RIGHT_BOTTOM);
        loginButtonPos->setMargin(Margin(0, 0, loginFormSize.width * 0.0357, loginFormSize.height * 0.0769));

        _loginButton = Button::create("res/ui/ui_buttons/b_small_1.png", "res/ui/ui_buttons/b_small_1_press.png");
        _loginButton->setLayoutParameter(loginButtonPos);
        _loginButton->setTitleText("Log me in");
        _loginButton->setTitleFontName(TitleFont);
        _loginButton->setTitleFontSize(32);

        _loginButton->addTouchEventListener([this](Ref * pSender, Widget::TouchEventType type)
                                            {
                                                if (type == Widget::TouchEventType::ENDED)
                                                    _onLoginClick();
                                            });

        _mainFormLayout->addChild(_loginButton);


        auto registerButtonPos = RelativeLayoutParameter::create();
        registerButtonPos->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
        registerButtonPos->setMargin(Margin(loginFormSize.width * 0.0357, 0,0, loginFormSize.height * 0.0769));

        _registerButton = Button::create("res/ui/ui_buttons/b_small_2.png", "res/ui/ui_buttons/b_small_2.png");
        _registerButton->setLayoutParameter(registerButtonPos);
        _registerButton->setTitleText("Register");
        _registerButton->setTitleFontName(TitleFont);
        _registerButton->setTitleFontSize(32);

        _registerButton->addTouchEventListener([this](Ref * pSender, Widget::TouchEventType type)
                                               {
                                                   if (type == Widget::TouchEventType::ENDED)
                                                       _onRegisterClick();
                                               });

        _mainFormLayout->addChild(_registerButton);

        return true;
    }

}}}
