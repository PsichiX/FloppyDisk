#include "Transform.h"

RTTI_CLASS_DERIVATIONS( Transform,
                        RTTI_DERIVATION( Component ),
                        RTTI_DERIVATIONS_END
                        )

Transform::Transform()
: RTTI_CLASS_DEFINE( Transform )
, Component( Component::None )
, Position( this, &Transform::getPosition, &Transform::setPosition )
, Rotation( this, &Transform::getRotation, &Transform::setRotation )
, Scale( this, &Transform::getScale, &Transform::setScale )
, m_position( sf::Vector2f( 0.0f, 0.0f ) )
, m_rotation( 0.0f )
, m_scale( sf::Vector2f( 1.0f, 1.0f ) )
{
}

Transform::~Transform()
{
}
