#include "GameManager.h"
#include "GameObject.h"
#include <Box2D/Box2D.h>

class DestructionListener
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager
    , public b2DestructionListener
{
    RTTI_CLASS_DECLARE( DestructionListener );

public:
                            DestructionListener( GameManager* owner );
                            ~DestructionListener();

    void                    SayGoodbye( b2Joint* joint );
    void                    SayGoodbye( b2Fixture* fixture );

private:
    GameManager*            m_owner;
};

RTTI_CLASS_DERIVATIONS( DestructionListener,
                        RTTI_DERIVATIONS_END
                        );

DestructionListener::DestructionListener( GameManager* owner )
: RTTI_CLASS_DEFINE( DestructionListener )
, m_owner( owner )
{
}

DestructionListener::~DestructionListener()
{
}

void DestructionListener::SayGoodbye( b2Joint* joint )
{

}

void DestructionListener::SayGoodbye( b2Fixture* fixture )
{

}

class ContactListener
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager
    , public b2ContactListener
{
    RTTI_CLASS_DECLARE( ContactListener );

public:
                            ContactListener( GameManager* owner );
                            ~ContactListener();

    void                    BeginContact( b2Contact* contact );
    void                    EndContact( b2Contact* contact );
    void                    PreSolve( b2Contact* contact, const b2Manifold* oldManifold );
    void                    PostSolve( b2Contact* contact, const b2ContactImpulse* impulse );

private:
    GameManager*            m_owner;
};

RTTI_CLASS_DERIVATIONS( ContactListener,
                        RTTI_DERIVATIONS_END
                        );

ContactListener::ContactListener( GameManager* owner )
: RTTI_CLASS_DEFINE( ContactListener )
, m_owner( owner )
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact( b2Contact* contact )
{

}

void ContactListener::EndContact( b2Contact* contact )
{

}

void ContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{

}

void ContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{

}

RTTI_CLASS_DERIVATIONS( GameManager,
                        RTTI_DERIVATIONS_END
                        );

GameManager::GameManager( float gravX, float gravY )
: RTTI_CLASS_DEFINE( GameManager )
, m_world( 0 )
, m_destructionListener( 0 )
, m_contactListener( 0 )
{
    m_world = xnew b2World( b2Vec2( gravX, gravY ) );
    m_destructionListener = xnew DestructionListener( this );
    m_contactListener = xnew ContactListener( this );
}

GameManager::~GameManager()
{
    DELETE_OBJECT( m_world );
    DELETE_OBJECT( m_destructionListener );
    DELETE_OBJECT( m_contactListener );
    removeAllGameObjects();
}

void GameManager::addGameObject( GameObject* go )
{
    if( !go || hasGameObject( go ) )
        return;
    m_gameObjects.push_back( go );
    go->setGameManager( this );
}

void GameManager::removeGameObject( GameObject* go )
{
    if( !hasGameObject( go ) )
        return;
    m_gameObjects.remove( go );
    go->setGameManager( 0 );
    DELETE_OBJECT( go );
}

void GameManager::removeGameObject( const std::string& id )
{
    GameObject* go = getGameObject( id );
    if( !go )
        return;
    m_gameObjects.remove( go );
    go->setGameManager( 0 );
    DELETE_OBJECT( go );
}

void GameManager::removeAllGameObjects()
{
    GameObject* go;
    for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
    {
        go = *it;
        go->setGameManager( 0 );
        DELETE_OBJECT( *it );
    }
    m_gameObjects.clear();
}

bool GameManager::hasGameObject( GameObject* go )
{
    for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        if( *it == go )
            return true;
    return false;
}

bool GameManager::hasGameObject( const std::string& id )
{
    for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        if( (*it)->getId() == id )
            return true;
    return false;
}

GameObject* GameManager::getGameObject( const std::string& id )
{
    for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
        if( (*it)->getId() == id )
            return *it;
    return 0;
}

void GameManager::processUpdate( float dt )
{
    GameObject* go;
    for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
    {
        go = *it;
        if( go->isActive() )
            go->onUpdate( dt );
    }
}

void GameManager::processRender( sf::RenderTarget* target )
{
    GameObject* go;
    for( std::list< GameObject* >::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++ )
    {
        go = *it;
        if( go->isActive() )
            go->onRender( target );
    }
}

void GameManager::processPhysics( float dt, int velIters, int posIters )
{
    m_world->Step( dt, velIters, posIters );
}
