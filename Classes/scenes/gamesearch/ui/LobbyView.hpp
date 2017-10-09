//
//  LobbyView.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#ifndef LobbyView_hpp
#define LobbyView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <boost/signals2.hpp>


namespace labyrinth {
namespace ui {
namespace impl
{

    namespace impl
    {

        class HeroCarousel
            : public cocos2d::ui::Layout
        {
            using OnSelectedHeroChangedSignal = boost::signals2::signal<void(int)>;

        public:
            virtual bool init() override;

            boost::signals2::scoped_connection OnSelectedHeroChangedConnector(const OnSelectedHeroChangedSignal::slot_type& slot)
            { return _onSelectedHeroChanged.connect(slot); }

        private:
            cocos2d::ui::PageView *     _heros;
            cocos2d::ui::Button *       _rightChangeButton;
            cocos2d::ui::Button *       _leftChangeButton;

            OnSelectedHeroChangedSignal _onSelectedHeroChanged;
        };


        class PlayerRow
            : public cocos2d::ui::Layout
        {
            using OnReadyClickedSignal = boost::signals2::signal<void(void)>;

        public:
            PlayerRow(const std::string& name, uint32_t uuid)
            : _rawName(name),
              _uuid(uuid)
            { }

            virtual bool init() override;

            void SetIcon();

            std::string GetName() const
            { return _rawName; }
            
            uint32_t GetUuid() const
            { return _uuid; }

            boost::signals2::scoped_connection OnReadyClickedConnector(const OnReadyClickedSignal::slot_type& slot)
            {
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]()
                {
                    _status->setEnabled(true);
                    _status->setTouchEnabled(true);
                });
                return _onReadyClicked.connect(slot);
            }

        private:
            cocos2d::ui::ImageView *    _icon;
            cocos2d::ui::Text *         _name;
            cocos2d::ui::CheckBox *     _status;

            const std::string           _rawName;
            uint32_t                    _uuid;

            OnReadyClickedSignal        _onReadyClicked;
        };


        class PlayersList
            : public cocos2d::ui::Layout
        {
        public:
            virtual bool init() override;

            PlayerRow * GetPlayerRow(uint32_t uuid) const;

            PlayerRow * InsertPlayer(const std::string& name, uint32_t uuid);
            void RemovePlayer(uint32_t uuid);

        private:
            cocos2d::ui::Text *         _playersLabel;
            cocos2d::ui::ListView *     _playersList;
            std::vector<PlayerRow*>     _playersListInternal;
        };

    }


    class LobbyView
        : public cocos2d::ui::Layout
    {
    public:
        virtual bool init() override;

        void SetStatus(const std::string& status)
        { }

        impl::HeroCarousel * GetHeroCarousel()
        { return _heroCarousel; }

        impl::PlayersList * GetPlayersList()
        { return _playersList; }

    private:
        cocos2d::ui::Text *     _statusText;
        impl::HeroCarousel *    _heroCarousel;
        impl::PlayersList *     _playersList;
    };

}}}

#endif /* LobbyView_hpp */
