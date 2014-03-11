#ifndef __COLLIDER__
#define __COLLIDER__

#include <Ptakopysk/Components/Component.h>

using namespace Ptakopysk;

class Collider
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public Component
{
	RTTI_CLASS_DECLARE( Collider );

public:
	Collider();
	virtual ~Collider();

    /// new component instance builder.
	FORCEINLINE static Component* onBuildComponent() { return xnew Collider(); }

    /// properties methods.
	FORCEINLINE float getAlpha() { return m_alpha; };
	void setAlpha( float v );

    /// properties.
	XeCore::Common::Property< float, Collider > Alpha;

protected:
    /// serialization.
	virtual Json::Value onSerialize( const std::string& property );
	virtual void onDeserialize( const std::string& property, const Json::Value& root );

	/// events.
	virtual void onDestroy();
	virtual void onDuplicate( Component* dst );
	virtual void onCollide( GameObject* other, bool beginOrEnd, b2Contact* contact );

private:
    /// data.
	float m_alpha;
};

#endif
