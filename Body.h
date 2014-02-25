#ifndef __BODY__
#define __BODY__

#include "Component.h"

class b2Body;
class b2PolygonShape;

class Body
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public Component
{
    RTTI_CLASS_DECLARE( Body );

public:
    Body();
    virtual ~Body();

    FORCEINLINE static Component* onBuildComponent() { return xnew Body(); }

    FORCEINLINE b2Body* getBody() { return m_body; };
    FORCEINLINE b2PolygonShape* getShape() { return m_shape; };

protected:
    virtual Json::Value onSerialize( const std::string& property );
    virtual void onDeserialize( const std::string& property, const Json::Value& root );

    virtual void onCreate();
    virtual void onDestroy();
    virtual void onDuplicate( Component* dst );

private:
    b2Body* m_body;
    b2PolygonShape* m_shape;
    b2BodyDef m_bodyDef;
    b2FixtureDef m_fixtureDef;
};

#endif
