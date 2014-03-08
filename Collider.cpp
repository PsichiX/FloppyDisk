#include "Collider.h"
#include <Ptakopysk/System/GameObject.h>
#include <Ptakopysk/Components/SpriteRenderer.h>
#include <XeCore/Common/Logger.h>

/// setup RTTI information for Collider class.
RTTI_CLASS_DERIVATIONS( Collider, // for class Collider...
						RTTI_DERIVATION( Component ), // ...that derives from class Component.
						RTTI_DERIVATIONS_END
						)

Collider::Collider()
: RTTI_CLASS_DEFINE( Collider )
, Component( Component::tPhysics ) // collider will listen for events from world physics.
, AlphaWhenCollide( this, &Collider::getAlphaWhenCollide, &Collider::setAlphaWhenCollide ) // bind property to methods.
, m_alphaWhenCollide( 0.0f )
{
	serializableProperty( "AlphaWhenCollide" ); // add property name to serializables.
}

Collider::~Collider()
{
}

Json::Value Collider::onSerialize( const std::string& property )
{
	if( property == "AlphaWhenCollide" )
		return Json::Value( m_alphaWhenCollide );
	else
		return Component::onSerialize( property );
}

void Collider::onDeserialize( const std::string& property, const Json::Value& root )
{
	if( property == "AlphaWhenCollide" && root.isNumeric() )
		m_alphaWhenCollide = (float)root.asDouble();
	else
		Component::onDeserialize( property, root );
}

void Collider::onDuplicate( Component* dst )
{
	if( !dst )
		return;
	Component::onDuplicate( dst );
	if( !XeCore::Common::IRtti::isDerived< Collider >( dst ) )
		return;
	Collider* c = (Collider*)dst;
	c->setAlphaWhenCollide( getAlphaWhenCollide() );
}

void Collider::onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact )
{
    /// find sprite renderer...
    SpriteRenderer* spr = getGameObject()->getComponent< SpriteRenderer >();
    if( spr )
    {
        /// ...then set alpha when start collide...
        if( beginOrEnd )
            spr->setColor( sf::Color( 255, 255, 255, byte( 255.0f * m_alphaWhenCollide ) ) );
        /// ...or reset alpha when stop collide with other game object.
        else
            spr->setColor( sf::Color( 255, 255, 255, 255 ) );
    }
}
