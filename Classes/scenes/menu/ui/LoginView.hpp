//
//  LoginView.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#ifndef LoginView_hpp
#define LoginView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <boost/signals2.hpp>

namespace labyrinth {
namespace ui {
namespace impl
{

    class LoginView
        : public cocos2d::ui::Layout
    {
        using OnClickSignal = boost::signals2::signal<void()>;
        using SignalConnection = boost::signals2::scoped_connection;

    public:
        virtual bool init() override;

        std::string GetEmail() const
        { return _emailField->getString(); }

        std::string GetPassword() const
        { return _passwordField->getString(); }

        SignalConnection OnLoginClickConnector(const OnClickSignal::slot_type& slot)
        { return _onLoginClick.connect(slot); }

        SignalConnection OnRegisterClickConnector(const OnClickSignal::slot_type& slot)
        { return _onRegisterClick.connect(slot); }

    private:
        cocos2d::ui::Text *         _loginViewTitle;

        cocos2d::ui::Layout *       _mainFormLayout;
        cocos2d::ui::Text *         _emailFieldLabel;
        cocos2d::ui::TextField *    _emailField;
        cocos2d::ui::Text *         _passwordFieldLabel;
        cocos2d::ui::TextField *    _passwordField;

        cocos2d::ui::Button *       _registerButton;
        cocos2d::ui::Button *       _loginButton;

        OnClickSignal               _onLoginClick;
        OnClickSignal               _onRegisterClick;
    };

}}}

#endif /* LoginView_hpp */
