#include "Body.h"
#include <Box2D/Box2D.h>

RTTI_CLASS_DERIVATIONS( Body,
                        RTTI_DERIVATION( Component ),
                        RTTI_DERIVATIONS_END
                        )

Body::Body()
: RTTI_CLASS_DEFINE( Body )
, Component( Component::Physics )
{
    m_shape = xnew b2PolygonShape();
    m_bodyDef.type = b2_dynamicBody;
    m_fixtureDef.shape = m_shape;
    m_fixtureDef.density = 1.0f;
    m_fixtureDef.friction = 1.0f;
}

Body::~Body()
{
    DELETE_OBJECT( m_shape );
    m_fixtureDef.shape = 0;
}

Json::Value Body::onSerialize( const std::string& property )
{
    return Component::onSerialize( property );
}

void Body::onDeserialize( const std::string& property, const Json::Value& root )
{
    Component::onDeserialize( property, root );
}

void Body::onCreate()
{
    DELETE_OBJECT( m_body );
    m_body = getGameObject()->getGameManager()->getPhysicsWorld()->CreateBody( &m_bodyDef );
    m_body->CreateFixture( &m_fixtureDef );
    m_body->SetUserData( this );
}

void Body::onDestroy()
{
    if( m_body )
        getGameObject()->getGameManager()->getPhysicsWorld()->DestroyBody( m_body );
    DELETE_OBJECT( m_body );
}

void Body::onDuplicate( Component* dst )
{
    Component::onDuplicate( dst );
    Body* c = XeCore::Common::IRtti::derivationCast< Component, Body >( dst );
}
