#ifndef __SERIALIZED__INLINE__
#define __SERIALIZED__INLINE__

void Serialized::serializableProperty( const std::string& name )
{
    for( std::vector< std::string >::iterator it = m_properties.begin(); it != m_properties.end(); it++ )
        if( *it == name )
            return;
    m_properties.push_back( name );
}

#endif
