#include "GameObject.h"
#include "Component.h"

RTTI_CLASS_DERIVATIONS( GameObject,
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
}

GameObject::~GameObject()
{
    removeAllComponents();
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

void GameObject::setGameManager( GameManager* gm )
{
    m_gameManager = gm;
}
