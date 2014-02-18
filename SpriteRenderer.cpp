#include "SpriteRenderer.h"
#include "Transform.h"
#include "GameObject.h"

RTTI_CLASS_DERIVATIONS( SpriteRenderer,
                        RTTI_DERIVATION( Component ),
                        RTTI_DERIVATIONS_END
                        )

SpriteRenderer::SpriteRenderer( sf::Texture* texture, float width, float height, float pox, float poy )
: RTTI_CLASS_DEFINE( SpriteRenderer )
, Component( Component::Update | Component::Render )
, m_renderStates( sf::RenderStates::Default )
{
    m_shape = xnew sf::RectangleShape(
        texture ?
        sf::Vector2f( width < 0.0f ? texture->getSize().x : width, height < 0.0f ? texture->getSize().y : height ) :
        sf::Vector2f( std::max( 0.0f, width ), std::max( 0.0f, height ) )
    );
    m_shape->setOrigin( m_shape->getSize().x * pox, m_shape->getSize().y * poy );
    if( texture )
        m_shape->setTexture( texture );
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
