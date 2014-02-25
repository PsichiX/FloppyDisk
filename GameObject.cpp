#include "GameObject.h"
#include "GameManager.h"
#include "Component.h"
#include <XeCore/Common/Logger.h>

RTTI_CLASS_DERIVATIONS( GameObject,
                        RTTI_DERIVATION( Serialized ),
                        RTTI_DERIVATIONS_END
                        )

GameObject::GameObject( const std::string& id )
: RTTI_CLASS_DEFINE( GameObject )
, Id( this, &GameObject::getId, &GameObject::setId )
, Active( this, &GameObject::isActive, &GameObject::setActive )
, Owner( this, &GameObject::getGameManager, 0 )
, m_id( id )
, m_active( true )
, m_gameManager( 0 )
{
    serializableProperty( "Id" );
    serializableProperty( "Active" );
}

GameObject::~GameObject()
{
    removeAllComponents();
}

void GameObject::fromJson( const Json::Value& root )
{
    if( !root.isObject() )
        return;
    Json::Value properties = root[ "properties" ];
    if( !properties.isNull() )
        deserialize( properties );
    Json::Value components = root[ "components" ];
    if( components.isArray() )
    {
        Json::Value item;
        Json::Value itemType;
        Json::Value itemProps;
        Component* comp;
        for( unsigned int i = 0; i < components.size(); i++ )
        {
            item = components[ i ];
            if( item.isObject() )
            {
                itemType = item[ "type" ];
                if( itemType.isString() )
                {
                    std::string typeId = itemType.asString();
                    XeCore::Common::IRtti::Derivation type = GameManager::findComponentFactoryTypeById( typeId );
                    if( !type )
                        XWARNING( "Component factory for type '", typeId.c_str(), "' not found!" );
                    comp = getComponent( type );
                    if( !comp )
                        comp = GameManager::buildComponent( typeId );
                    if( comp )
                    {
                        comp->fromJson( item );
                        addComponent( comp );
                    }
                }
            }
        }
    }
}

Json::Value GameObject::toJson()
{
    Json::Value root;
    serialize( root[ "properties" ] );
    Json::Value components;
    Json::Value comp;
    for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
    {
        comp = it->second->toJson();
        if( !comp.isNull() )
            components.append( comp );
    }
    if( !components.isNull() )
        root[ "components" ] = components;
    return root;
}

void GameObject::addComponent( Component* c )
{
    if( !c || m_components.count( c->getType() ) )
        return;
    m_components[ c->getType() ] = c;
    c->setGameObject( this );
}

void GameObject::removeComponent( Component* c )
{
    if( !c || m_components.count( c->getType() ) )
        return;
    m_components.erase( c->getType() );
    c->setGameObject( 0 );
    DELETE_OBJECT( c );
}

void GameObject::removeComponent( XeCore::Common::IRtti::Derivation d )
{
    if( !m_components.count( d ) )
        return;
    Component* c = m_components[ d ];
    m_components.erase( d );
    c->setGameObject( 0 );
    DELETE_OBJECT( c );
}

void GameObject::removeAllComponents()
{
    Component* c;
    for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
    {
        c = it->second;
        c->setGameObject( 0 );
        DELETE_OBJECT( c );
    }
    m_components.clear();
}

bool GameObject::hasComponent( Component* c )
{
    return c && m_components.count( c->getType() );
}

bool GameObject::hasComponent( XeCore::Common::IRtti::Derivation d )
{
    return m_components.count( d );
}

Component* GameObject::getComponent( XeCore::Common::IRtti::Derivation d )
{
    return m_components.count( d ) ? m_components[ d ] : 0;
}

void GameObject::onUpdate( float dt )
{
    Component* c;
    for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
    {
        c = it->second;
        if( c->getTypeFlags() & Component::Update )
            c->onUpdate( dt );
    }
}

void GameObject::onRender( sf::RenderTarget* target )
{
    Component* c;
    for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
    {
        c = it->second;
        if( c->getTypeFlags() & Component::Render )
            c->onRender( target );
    }
}

void GameObject::onCollide( GameObject* other )
{
    Component* c;
    for( std::map< XeCore::Common::IRtti::Derivation, Component* >::iterator it = m_components.begin(); it != m_components.end(); it++ )
    {
        c = it->second;
        if( c->getTypeFlags() & Component::Physics )
            c->onCollide( other );
    }
}

Json::Value GameObject::onSerialize( const std::string& property )
{
    if( property == "Id" )
        return Json::Value( m_id );
    else if( property == "Active" )
        return Json::Value( m_active );
    return Json::Value::null;
}

void GameObject::onDeserialize( const std::string& property, const Json::Value& root )
{
    if( property == "Id" && root.isString() )
        m_id = root.asString();
    else if( property == "Active" && root.isBool() )
        m_active = root.asBool();
}

void GameObject::setGameManager( GameManager* gm )
{
    m_gameManager = gm;
}
