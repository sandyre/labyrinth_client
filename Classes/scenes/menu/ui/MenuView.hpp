//
//  MenuView.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 15/09/2017.
//
//

#ifndef MenuView_hpp
#define MenuView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <boost/signals2.hpp>


namespace labyrinth {
namespace ui {
namespace impl
{

    class MenuView
        : public cocos2d::ui::Layout
    {
        using OnClickSignal = boost::signals2::signal<void()>;
        using SignalConnection = boost::signals2::scoped_connection;

    public:
        virtual bool init() override;

        SignalConnection OnSearchClickConnector(const OnClickSignal::slot_type& slot)
        { return _onSearchClick.connect(slot); }

        SignalConnection OnCollectionClickConnector(const OnClickSignal::slot_type& slot)
        { return _onCollectionClick.connect(slot); }

        SignalConnection OnShopClickConnector(const OnClickSignal::slot_type& slot)
        { return _onShopClick.connect(slot); }

        SignalConnection OnSettingsClickConnector(const OnClickSignal::slot_type& slot)
        { return _onSettingsClick.connect(slot); }

    private:
        cocos2d::ui::Layout *   _menuFormLayout;

        cocos2d::ui::Button *   _searchButton;
        cocos2d::ui::Button *   _collectionButton;
        cocos2d::ui::Button *   _shopButton;
        cocos2d::ui::Button *   _settingsButton;

        OnClickSignal           _onSearchClick;
        OnClickSignal           _onCollectionClick;
        OnClickSignal           _onShopClick;
        OnClickSignal           _onSettingsClick;
    };

}}}

#endif /* MenuView_hpp */
