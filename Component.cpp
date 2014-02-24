#include "Component.h"
#include "GameObject.h"
#include "GameManager.h"

RTTI_CLASS_DERIVATIONS( Component,
                        RTTI_DERIVATION( Serialized ),
                        RTTI_DERIVATIONS_END
                        )

Component::Component( Component::Type typeFlags )
: RTTI_CLASS_DEFINE( Component )
, TypeFlags( this, &Component::getTypeFlags, 0 )
, Active( this, &Component::isActive, &Component::setActive )
, Owner( this, &Component::getGameObject, 0 )
, m_typeFlags( typeFlags )
, m_active( true )
, m_gameObject( 0 )
{
    serializableProperty( "Active" );
}

Component::~Component()
{
    if( m_gameObject )
        m_gameObject->removeComponent( this );
}

void Component::fromJson( const Json::Value& root )
{
    if( !root.isObject() )
        return;
    Json::Value type = root[ "type" ];
    if( !type.isString() || GameManager::findComponentFactoryTypeById( type.asString() ) != getType() )
        return;
    Json::Value properties = root[ "properties" ];
    deserialize( properties );
}

Json::Value Component::toJson()
{
    Json::Value root;
    std::string typeId = std::string( getName() );
    if( GameManager::findComponentFactoryTypeById( typeId ) != getType() )
        return Json::Value::null;
    root[ "type" ] = typeId;
    serialize( root[ "properties" ] );
    return root;
}

void Component::onUpdate( float dt ) {}

void Component::onRender( sf::RenderTarget* target ) {}

void Component::onCollide( GameObject* other ) {}

Json::Value Component::onSerialize( const std::string& property )
{
    if( property == "Active" )
        return Json::Value( m_active );
    return Json::Value::null;
}

void Component::onDeserialize( const std::string& property, const Json::Value& root )
{
    if( property == "Active" && root.isBool() )
        m_active = root.asBool();
}

void Component::setGameObject( GameObject* gameObject )
{
    m_gameObject = gameObject;
}

Component::Type operator|( Component::Type a, Component::Type b )
{
    return (Component::Type)( (int)a | (int)b );
};
