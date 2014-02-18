#ifndef __ACTOR__
#define __ACTOR__

#include <XeCore/Common/Base.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <map>
#include <string>

class Component;
class GameManager;
namespace sf
{
    class RenderTarget;
}

class GameObject
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
{
    RTTI_CLASS_DECLARE( GameObject );
    friend class GameManager;

public:
    GameObject( const std::string& id );
    virtual ~GameObject();

    FORCEINLINE std::string getId() { return m_id; };
    FORCEINLINE void setId( const std::string& id ) { m_id = id; };
    FORCEINLINE bool isActive() { return m_active; };
    FORCEINLINE void setActive( bool mode = true ) { m_active = mode; };
    FORCEINLINE GameManager* getGameManager() { return m_gameManager; };
    void addComponent( Component* c );
    void removeComponent( Component* c );
    void removeComponent( XeCore::Common::IRtti::Derivation d );
    void removeAllComponents();
    bool hasComponent( Component* c );
    bool hasComponent( XeCore::Common::IRtti::Derivation d );
    Component* getComponent( XeCore::Common::IRtti::Derivation d );

protected:
    void onUpdate( float dt );
    void onRender( sf::RenderTarget* target );
    void onCollide( GameObject* other );

private:
    void setGameManager( GameManager* gm );

    std::string m_id;
    bool m_active;
    std::map< XeCore::Common::IRtti::Derivation, Component* > m_components;
    GameManager* m_gameManager;
};

#endif
