#ifndef __COMPONENT__
#define __COMPONENT__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include "Serialized.h"

class GameObject;

class Component
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public virtual Serialized
{
    RTTI_CLASS_DECLARE( Component );
    friend class GameObject;

public:
    enum Type
    {
        None = 0,
        Update = 1 << 0,
        Render = 1 << 1,
        Physics = 1 << 2,
        All = -1
    };

    typedef Component* ( *OnBuildComponentCallback )();

    Component( Type typeFlags );
    virtual ~Component();

    FORCEINLINE Type getTypeFlags() { return m_typeFlags; };
    FORCEINLINE bool isActive() { return m_active; };
    FORCEINLINE void setActive( bool mode = true ) { m_active = mode; };
    FORCEINLINE GameObject* getGameObject() { return m_gameObject; };

    void fromJson( const Json::Value& root );
    Json::Value toJson();

    XeCore::Common::Property< Type, Component > TypeFlags;
    XeCore::Common::Property< bool, Component > Active;
    XeCore::Common::Property< GameObject*, Component > Owner;

protected:
    virtual void onUpdate( float dt );
    virtual void onRender( sf::RenderTarget* target );
    virtual void onCollide( GameObject* other );

    virtual Json::Value onSerialize( const std::string& property );
    virtual void onDeserialize( const std::string& property, const Json::Value& root );

private:
    void setGameObject( GameObject* gameObject );

    Type m_typeFlags;
    bool m_active;
    GameObject* m_gameObject;
};

Component::Type operator|( Component::Type a, Component::Type b );

#endif
