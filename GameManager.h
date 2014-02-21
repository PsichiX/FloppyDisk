#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include <XeCore/Common/Property.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <list>
#include <string>

class DestructionListener;
class ContactListener;
class b2World;
class GameObject;
namespace sf
{
    class RenderTarget;
}

class GameManager
    : public virtual XeCore::Common::IRtti
    , public virtual XeCore::Common::MemoryManager::Manageable
{
    RTTI_CLASS_DECLARE( GameManager );

public:
    enum SceneContentType
    {
        None = 0,
        Assets = 1 << 0,
        PrefabGameObjects = 1 << 1,
        GameObjects = 1 << 2,
        All = -1
    };

    static const int DEFAULT_VEL_ITERS = 6;
    static const int DEFAULT_POS_ITERS = 2;

    GameManager( float gravX, float gravY );
    ~GameManager();

    FORCEINLINE b2World* getPhysicsWorld() { return m_world; };
    Json::Value loadJson( const std::string& path );
    bool saveJson( const std::string& path, const Json::Value& root );
    void jsonToScene( const Json::Value& root, SceneContentType contentFlags = All );
    Json::Value sceneToJson( SceneContentType contentFlags = All );
    void removeScene( SceneContentType contentFlags = All );
    void addGameObject( GameObject* go, bool prefab = false );
    void removeGameObject( GameObject* go, bool prefab = false );
    void removeGameObject( const std::string& id, bool prefab = false );
    void removeAllGameObjects( bool prefab = false );
    bool hasGameObject( GameObject* go, bool prefab = false );
    bool hasGameObject( const std::string& id, bool prefab = false );
    GameObject* getGameObject( const std::string& id, bool prefab = false );
    void processUpdate( float dt );
    void processRender( sf::RenderTarget* target );
    void processPhysics( float dt, int velIters = DEFAULT_VEL_ITERS, int posIters = DEFAULT_POS_ITERS );

    XeCore::Common::Property< b2World*, GameManager > PhysicsWorld;

private:
    b2World* m_world;
    DestructionListener* m_destructionListener;
    ContactListener* m_contactListener;
    std::list< GameObject* > m_prefabGameObjects;
    std::list< GameObject* > m_gameObjects;
};

GameManager::SceneContentType operator|( GameManager::SceneContentType a, GameManager::SceneContentType b );

#endif
