#include "Collider.h"
#include <Ptakopysk/System/Tween.h>
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
, Alpha( this, &Collider::getAlpha, &Collider::setAlpha ) // bind property to methods.
, m_alpha( 1.0f )
{
	serializableProperty( "Alpha" ); // add property name to serializables.
}

Collider::~Collider()
{
}

void Collider::setAlpha( float v )
{
    m_alpha = v;
    /// make sure that component is ready (it's added to game object).
    if( !getGameObject() )
        return;
    /// find sprite renderer and set it's alpha from property value.
    SpriteRenderer* spr = getGameObject()->getComponent< SpriteRenderer >();
    if( spr )
        spr->setColor( sf::Color( 255, 255, 255, byte( 255.0f * m_alpha ) ) );
}

Json::Value Collider::onSerialize( const std::string& property )
{
	if( property == "Alpha" )
		return Json::Value( m_alpha );
	else
		return Component::onSerialize( property );
}

void Collider::onDeserialize( const std::string& property, const Json::Value& root )
{
	if( property == "Alpha" && root.isNumeric() )
		m_alpha = (float)root.asDouble();
	else
		Component::onDeserialize( property, root );
}

void Collider::onDestroy()
{
    Tweener::use().killTweensOf( Alpha );
}

void Collider::onDuplicate( Component* dst )
{
	if( !dst )
		return;
	Component::onDuplicate( dst );
	if( !XeCore::Common::IRtti::isDerived< Collider >( dst ) )
		return;
	Collider* c = (Collider*)dst;
	c->setAlpha( getAlpha() );
}

void Collider::onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact )
{
    if( beginOrEnd )
    {
        /// create custom tween and pass it to tweener.
        Tweener::use().startTween(
            xnew Tween<
                float, // property type.
                Collider, // property owner type.
                Math::Easing::Quadratic::inOut // easing function.
            >(
                Alpha, // reference to property.
                0.25f, // value to change into.
                1.0f // change duration.
            )
        );
    }
    else
    {
        Tweener::use().startTween(
            xnew Tween< float, Collider, Math::Easing::Quadratic::inOut >( Alpha, 1.0f, 1.0f )
        );
    }
}
