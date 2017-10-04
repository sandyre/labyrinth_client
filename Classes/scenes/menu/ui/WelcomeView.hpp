//
//  WelcomeView.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#ifndef WelcomeView_hpp
#define WelcomeView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <boost/signals2.hpp>


namespace labyrinth {
namespace ui {
namespace impl
{

    class WelcomeView
        : public cocos2d::ui::Layout
    {
        using OnClickSignal = boost::signals2::signal<void()>;
        using SignalConnection = boost::signals2::scoped_connection;

    public:
        virtual bool init() override;

        void SetConnectionEstablished(bool online)
        { _startButton->setEnabled(online); }

        SignalConnection OnStartClickConnector(const OnClickSignal::slot_type& slot)
        { return _onStartClick.connect(slot); }

    private:
        cocos2d::ui::Text *     _labyrinthLabel;
        cocos2d::ui::Button *   _startButton;

        OnClickSignal           _onStartClick;
    };

}}}


#endif /* WelcomeView_hpp */
