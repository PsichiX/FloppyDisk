#ifndef __SPRITE_RENDERER__
#define __SPRITE_RENDERER__

#include "Component.h"
#include <SFML/Graphics.hpp>

class SpriteRenderer
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public Component
{
    RTTI_CLASS_DECLARE( SpriteRenderer );

public:
    SpriteRenderer();
    virtual ~SpriteRenderer();

    FORCEINLINE static Component* onBuildComponent() { return xnew SpriteRenderer(); }

    FORCEINLINE sf::RectangleShape* getShape() { return m_shape; };
    FORCEINLINE sf::RenderStates& getRenderStates() { return m_renderStates; };

    XeCore::Common::Property< sf::RectangleShape*, SpriteRenderer > Shape;
    XeCore::Common::Property< sf::RenderStates&, SpriteRenderer > RenderStates;

protected:
    virtual void onUpdate( float dt );
    virtual void onRender( sf::RenderTarget* target );

private:
    sf::RectangleShape* m_shape;
    sf::RenderStates m_renderStates;
};

#endif
