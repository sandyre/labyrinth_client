//
//  make_autorelease.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#ifndef make_autorelease_h
#define make_autorelease_h

/*
	Create autorelease cocos object

	@note T must be derived from cocos2d::Ref
	@param args [optional] arguments that forwards to T constructor
	@return autorelease object
 */
template<typename T, typename... Args>
inline T* make_autorelease( Args&&... args )noexcept {
    static_assert( std::is_base_of<cocos2d::Ref, T>::value,
                  "T must be derived from cocos2d::Ref" );

    auto pRet = new( std::nothrow ) T{ std::forward<Args>( args )... };
    if ( pRet && pRet->init( ) ) {
        pRet->autorelease( );
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

#endif /* make_autorelease_h */
