#include "SpriteRenderer.h"
#include "Transform.h"
#include "GameObject.h"

RTTI_CLASS_DERIVATIONS( SpriteRenderer,
                        RTTI_DERIVATION( Component ),
                        RTTI_DERIVATIONS_END
                        )

SpriteRenderer::SpriteRenderer()
: RTTI_CLASS_DEFINE( SpriteRenderer )
, Component( Component::Update | Component::Render )
, Shape( this, &SpriteRenderer::getShape, 0 )
, RenderStates( this, &SpriteRenderer::getRenderStates, 0 )
, m_renderStates( sf::RenderStates::Default )
{
    m_shape = xnew sf::RectangleShape();
}

SpriteRenderer::~SpriteRenderer()
{
    DELETE_OBJECT( m_shape );
}

void SpriteRenderer::onUpdate( float dt )
{
    Transform* trans = (Transform*)getGameObject()->getComponent( RTTI_DERIVATION( Transform ) );
    if( trans )
    {
        m_shape->setPosition( trans->getPosition() );
        m_shape->setRotation( trans->getRotation() );
        m_shape->setScale( trans->getScale() );
    }
}

void SpriteRenderer::onRender( sf::RenderTarget* target )
{
    target->draw( *m_shape, m_renderStates );
}
