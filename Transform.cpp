#include "Transform.h"

RTTI_CLASS_DERIVATIONS( Transform,
                        RTTI_DERIVATION( Component ),
                        RTTI_DERIVATIONS_END
                        )

Transform::Transform( sf::Vector2f pos, float rot, sf::Vector2f scl )
: RTTI_CLASS_DEFINE( Transform )
, Component( Component::None )
, Position( this, &Transform::getPosition, &Transform::setPosition )
, Rotation( this, &Transform::getRotation, &Transform::setRotation )
, Scale( this, &Transform::getScale, &Transform::setScale )
, m_position( pos )
, m_rotation( rot )
, m_scale( scl )
{
}

Transform::~Transform()
{
}
