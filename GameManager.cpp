#include "GameManager.h"
#include "GameObject.h"
#include "Assets.h"
#include <Box2D/Box2D.h>
#include <fstream>
#include <list>

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
, PhysicsWorld( this, &GameManager::getPhysicsWorld, 0 )
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
    removeScene();
}

Json::Value GameManager::loadJson( const std::string& path )
{
    std::ifstream file( path.c_str(), std::ifstream::in | std::ifstream::binary );
    if( !file )
        return Json::Value::null;
    file.seekg( 0, std::ifstream::end );
    unsigned int fsize = file.tellg();
    file.seekg( 0, std::ifstream::beg );
    std::string content;
    content.resize( fsize + 1, 0 );
    file.read( (char*)content.c_str(), fsize );
    file.close();
    Json::Value root;
    Json::Reader reader;
    reader.parse( content, root );
    return root;
}

bool GameManager::saveJson( const std::string& path, const Json::Value& root )
{
    std::ofstream file( path.c_str(), std::ifstream::out | std::ifstream::binary );
    if( !file )
        return false;
    Json::StyledWriter writer;
    std::string content = writer.write( root );
    file.write( content.c_str(), content.length() );
    file.close();
    return true;
}

void GameManager::jsonToScene( const Json::Value& root, SceneContentType contentFlags )
{
    if( contentFlags == GameManager::None || !root.isObject() )
        return;
    Json::Value assets = root[ "assets" ];
    if( contentFlags & GameManager::Assets && assets.isObject() )
        Assets::use().jsonToAssets( assets );
    Json::Value prefabs = root[ "prefabs" ];
    Json::Value scene = root[ "scene" ];
}

Json::Value GameManager::sceneToJson( SceneContentType contentFlags )
{
    Json::Value root;
    if( contentFlags & GameManager::Assets )
    {
        Json::Value assets = Assets::use().assetsToJson();
        if( !assets.isNull() )
            root[ "assets" ] = assets;
    }
    return root;
}

void GameManager::removeScene( SceneContentType contentFlags )
{
    if( contentFlags & GameManager::Assets )
        Assets::use().freeAll();
    if( contentFlags & GameManager::PrefabGameObjects )
        removeAllGameObjects();
    if( contentFlags & GameManager::GameObjects )
        removeAllGameObjects( true );
}

void GameManager::addGameObject( GameObject* go, bool prefab )
{
    if( !go || hasGameObject( go, prefab ) )
        return;
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    cgo.push_back( go );
    go->setGameManager( this );
}

void GameManager::removeGameObject( GameObject* go, bool prefab )
{
    if( !hasGameObject( go, prefab ) )
        return;
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    cgo.remove( go );
    go->setGameManager( 0 );
    DELETE_OBJECT( go );
}

void GameManager::removeGameObject( const std::string& id, bool prefab )
{
    GameObject* go = getGameObject( id, prefab );
    if( !go )
        return;
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    cgo.remove( go );
    go->setGameManager( 0 );
    DELETE_OBJECT( go );
}

void GameManager::removeAllGameObjects( bool prefab )
{
    GameObject* go;
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
    {
        go = *it;
        go->setGameManager( 0 );
        DELETE_OBJECT( *it );
    }
    cgo.clear();
}

bool GameManager::hasGameObject( GameObject* go, bool prefab )
{
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
        if( *it == go )
            return true;
    return false;
}

bool GameManager::hasGameObject( const std::string& id, bool prefab )
{
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
        if( (*it)->getId() == id )
            return true;
    return false;
}

GameObject* GameManager::getGameObject( const std::string& id, bool prefab )
{
    std::list< GameObject* >& cgo = prefab ? m_prefabGameObjects : m_gameObjects;
    for( std::list< GameObject* >::iterator it = cgo.begin(); it != cgo.end(); it++ )
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

GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b )
{
    return (GameManager::SceneContentType)( (int)a | (int)b );
};
