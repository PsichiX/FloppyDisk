#include "Serialized.h"

RTTI_CLASS_DERIVATIONS( Serialized,
                        RTTI_DERIVATIONS_END
                        );

Serialized::Serialized()
: RTTI_CLASS_DEFINE( Serialized )
{
}

Serialized::~Serialized()
{
    m_properties.clear();
}

void Serialized::serialize( Json::Value& dstRoot )
{
    Json::Value item;
    for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
    {
        item = onSerialize( *it );
        if( !item.isNull() )
            dstRoot[ *it ] = item;
    }
}

void Serialized::deserialize( const Json::Value& srcRoot )
{
    if( !srcRoot.isObject() )
        return;
    Json::Value item;
    for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
    {
        item = srcRoot[ *it ];
        if( !item.isNull() )
            onDeserialize( *it, item );
    }
}
