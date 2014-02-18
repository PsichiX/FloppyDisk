#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
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

    static const int DEFAULT_VEL_ITERS = 6;
    static const int DEFAULT_POS_ITERS = 2;

    GameManager( float gravX, float gravY );
    ~GameManager();

    void addGameObject( GameObject* go );
    void removeGameObject( GameObject* go );
    void removeGameObject( const std::string& id );
    void removeAllGameObjects();
    bool hasGameObject( GameObject* go );
    bool hasGameObject( const std::string& id );
    GameObject* getGameObject( const std::string& id );
    FORCEINLINE b2World* getPhysicsWorld() { return m_world; };
    void processUpdate( float dt );
    void processRender( sf::RenderTarget* target );
    void processPhysics( float dt, int velIters = DEFAULT_VEL_ITERS, int posIters = DEFAULT_POS_ITERS );

private:
    b2World* m_world;
    DestructionListener* m_destructionListener;
    ContactListener* m_contactListener;
    std::list< GameObject* > m_gameObjects;
};

#endif
