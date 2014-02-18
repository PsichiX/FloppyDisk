#ifndef __PROPERTY__
#define __PROPERTY__

#define DEFINE_PROPERTY( name, getter, setter ) name( this, &get##getter, &set##setter )

template< typename PT, typename OT >
class Property
{
private:
    typedef PT ( OT::* _Getter )() const;
    typedef void ( OT::* _Setter )( PT );

public:
    Property( OT* obj, _Getter getter, _Setter setter)
    : m_obj( obj )
    , m_getter( getter )
    , m_setter( setter )
    {
    }

    operator PT () { return m_obj && m_getter ? ( *m_obj.*m_getter )() : PT(); };
    void operator= ( PT value ) { if( m_obj && m_setter ) ( *m_obj.*m_setter )( value ); };

private:
    OT* m_obj;
    _Getter m_getter;
    _Setter m_setter;
};

#endif
