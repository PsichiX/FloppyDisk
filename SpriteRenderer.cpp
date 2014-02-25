#include "SpriteRenderer.h"
#include "Transform.h"
#include "GameObject.h"
#include "Assets.h"

RTTI_CLASS_DERIVATIONS( SpriteRenderer,
                        RTTI_DERIVATION( Component ),
                        RTTI_DERIVATIONS_END
                        )

SpriteRenderer::SpriteRenderer()
: RTTI_CLASS_DEFINE( SpriteRenderer )
, Component( Component::Update | Component::Render )
, Texture( this, &SpriteRenderer::getTexture, &SpriteRenderer::setTexture )
, Size( this, &SpriteRenderer::getSize, &SpriteRenderer::setSize )
, Origin( this, &SpriteRenderer::getOrigin, &SpriteRenderer::setOrigin )
, RenderStates( this, &SpriteRenderer::getRenderStates, 0 )
, m_renderStates( sf::RenderStates::Default )
{
    serializableProperty( "RenderStates" );
    serializableProperty( "Texture" );
    serializableProperty( "Size" );
    serializableProperty( "Origin" );
    m_shape = xnew sf::RectangleShape();
}

SpriteRenderer::~SpriteRenderer()
{
    DELETE_OBJECT( m_shape );
}

void SpriteRenderer::setSize( sf::Vector2f size )
{
    sf::Texture* t = getTexture();
    if( size.x < 0.0f )
        size.x = t ? t->getSize().x : 0.0f;
    if( size.y < 0.0f )
        size.y = t ? t->getSize().y : 0.0f;
    m_shape->setSize( size );
}

Json::Value SpriteRenderer::onSerialize( const std::string& property )
{
    if( property == "Texture" )
        return Json::Value( Assets::use().findTexture( getTexture() ) );
    else if( property == "Size" )
    {
        Json::Value v;
        v.append( Json::Value( getSize().x ) );
        v.append( Json::Value( getSize().y ) );
        return v;
    }
    else if( property == "Origin" )
    {
        Json::Value v;
        v.append( Json::Value( getOrigin().x ) );
        v.append( Json::Value( getOrigin().y ) );
        return v;
    }
    else if( property == "RenderStates" )
    {
        Json::Value v;
        v[ "blendMode" ] = Json::Value( (int)m_renderStates.blendMode );
        v[ "shader" ] = Json::Value( Assets::use().findShader( m_renderStates.shader ) );
        return v;
    }
    else
        return Component::onSerialize( property );
}

void SpriteRenderer::onDeserialize( const std::string& property, const Json::Value& root )
{
    if( property == "Texture" && root.isString() )
        setTexture( Assets::use().getTexture( root.asString() ) );
    else if( property == "Size" && root.isArray() && root.size() >= 2 )
    {
        setSize( sf::Vector2f(
            (float)root[ 0u ].asDouble(),
            (float)root[ 1u ].asDouble()
        ) );
    }
    else if( property == "Origin" && root.isArray() && root.size() >= 2 )
    {
        setOrigin( sf::Vector2f(
            (float)root[ 0u ].asDouble(),
            (float)root[ 1u ].asDouble()
        ) );
    }
    else if( property == "RenderStates" && root.isObject() )
    {
        Json::Value blendMode = root[ "blendMode" ];
        if( blendMode.isInt() )
            m_renderStates.blendMode = (sf::BlendMode)blendMode.asInt();
        Json::Value shader = root[ "shader" ];
        if( shader.isString() )
            m_renderStates.shader = Assets::use().getShader( shader.asString() );
    }
    else
        Component::onDeserialize( property, root );
}

void SpriteRenderer::onUpdate( float dt )
{
    Transform* trans = (Transform*)getGameObject()->getComponent( RTTI_CLASS_TYPE( Transform ) );
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
