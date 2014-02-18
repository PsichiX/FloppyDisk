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
    SpriteRenderer( sf::Texture* texture, float width = -1.0f, float height = -1.0f, float pox = 0.5f, float poy = 0.5f );
    virtual ~SpriteRenderer();

    FORCEINLINE sf::RectangleShape* getShape() { return m_shape; };
    FORCEINLINE sf::RenderStates& getRenderStates() { return m_renderStates; };

protected:
    virtual void onUpdate( float dt );
    virtual void onRender( sf::RenderTarget* target );

private:
    sf::RectangleShape* m_shape;
    sf::RenderStates m_renderStates;
};

#endif
