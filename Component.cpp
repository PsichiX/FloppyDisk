#include "Component.h"
#include "GameObject.h"

RTTI_CLASS_DERIVATIONS( Component,
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
}

Component::~Component()
{
    if( m_gameObject )
        m_gameObject->removeComponent( this );
}

void Component::onUpdate( float dt ) {}

void Component::onRender( sf::RenderTarget* target ) {}

void Component::onCollide( GameObject* other ) {}

void Component::setGameObject( GameObject* gameObject )
{
    m_gameObject = gameObject;
}

Component::Type operator|( Component::Type a, Component::Type b ) { return (Component::Type)( (int)a | (int)b ); };
