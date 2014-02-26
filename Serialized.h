#ifndef __SERIALIZED__
#define __SERIALIZED__

#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <json/json.h>
#include <vector>
#include <string>

class Serialized
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
{
    RTTI_CLASS_DECLARE( Serialized );

public:
    Serialized();
    virtual ~Serialized();

    FORCEINLINE void serializableProperty( const std::string& name );
    FORCEINLINE void notSerializableProperty( const std::string& name );
    void serialize( Json::Value& dstRoot );
    void deserialize( const Json::Value& srcRoot );

protected:
    virtual Json::Value onSerialize( const std::string& property ) = 0;
    virtual void onDeserialize( const std::string& property, const Json::Value& root ) = 0;

private:
    std::vector< std::string > m_properties;
};

#include "Serialized.inl"

#endif
