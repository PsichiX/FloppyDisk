#include "Assets.h"

RTTI_CLASS_DERIVATIONS( Assets,
                        RTTI_DERIVATIONS_END
                        )

Assets::Assets()
: RTTI_CLASS_DEFINE( Assets )
{
}

Assets::~Assets()
{
    freeAll();
}

sf::Texture* Assets::addTexture( const std::string& id, const sf::Texture* ptr )
{
    sf::Texture* t = getTexture( id );
    if( !t && ptr )
    {
        t = (sf::Texture*)ptr;
        m_textures[ id ] = t;
    }
    return t;
}

sf::Shader* Assets::addShader( const std::string& id, const sf::Shader* ptr )
{
    sf::Shader* t = getShader( id );
    if( !t && ptr )
    {
        t = (sf::Shader*)ptr;
        m_shaders[ id ] = t;
    }
    return t;
}

sf::Sound* Assets::addSound( const std::string& id, const sf::SoundBuffer* ptrbuff, const sf::Sound* ptr )
{
    sf::Sound* t = getSound( id );
    if( !t && ptrbuff && ptr )
    {
        t = (sf::Sound*)ptr;
        m_soundsBuffs[ id ] = (sf::SoundBuffer*)ptrbuff;
        m_sounds[ id ] = t;
    }
    return t;
}

sf::Music* Assets::addMusic( const std::string& id, const sf::Music* ptr )
{
    sf::Music* t = getMusic( id );
    if( !t && ptr )
    {
        t = (sf::Music*)ptr;
        m_musics[ id ] = t;
    }
    return t;
}

sf::Font* Assets::addFont( const std::string& id, const sf::Font* ptr )
{
    sf::Font* t = getFont( id );
    if( !t && ptr )
    {
        t = (sf::Font*)ptr;
        m_fonts[ id ] = t;
    }
    return t;
}

sf::Texture* Assets::loadTexture( const std::string& id, const std::string& path )
{
    sf::Texture* t = getTexture( id );
    if( !t )
    {
        t = xnew sf::Texture();
        if( !t->loadFromFile( path ) )
        {
            DELETE_OBJECT( t );
            return 0;
        }
        m_textures[ id ] = t;
    }
    return t;
}

sf::Shader* Assets::loadShader( const std::string& id, const std::string& vspath, const std::string& fspath )
{
    sf::Shader* t = getShader( id );
    if( !t )
    {
        t = xnew sf::Shader();
        if( !t->loadFromFile( vspath, fspath ) )
        {
            DELETE_OBJECT( t );
            return 0;
        }
        m_shaders[ id ] = t;
    }
    return t;
}

sf::Sound* Assets::loadSound( const std::string& id, const std::string& path )
{
    sf::Sound* t = getSound( id );
    if( !t )
    {
        sf::SoundBuffer* tb = xnew sf::SoundBuffer();
        if( !tb->loadFromFile( path ) )
        {
            DELETE_OBJECT( tb );
            return 0;
        }
        m_soundsBuffs[ id ] = tb;
        t = xnew sf::Sound();
        t->setBuffer( *tb );
        m_sounds[ id ] = t;
    }
    return t;
}

sf::Music* Assets::loadMusic( const std::string& id, const std::string& path )
{
    sf::Music* t = getMusic( id );
    if( !t )
    {
        t = xnew sf::Music();
        if( !t->openFromFile( path ) )
        {
            DELETE_OBJECT( t );
            return 0;
        }
        m_musics[ id ] = t;
    }
    return t;
}

sf::Font* Assets::loadFont( const std::string& id, const std::string& path )
{
    sf::Font* t = getFont( id );
    if( !t )
    {
        t = xnew sf::Font();
        if( !t->loadFromFile( path ) )
        {
            DELETE_OBJECT( t );
            return 0;
        }
        m_fonts[ id ] = t;
    }
    return t;
}

sf::Texture* Assets::getTexture( const std::string& id )
{
    return m_textures.count( id ) ? m_textures[ id ] : 0;
}

sf::Shader* Assets::getShader( const std::string& id )
{
    return m_shaders.count( id ) ? m_shaders[ id ] : 0;
}

sf::Sound* Assets::getSound( const std::string& id )
{
    return m_sounds.count( id ) ? m_sounds[ id ] : 0;
}

sf::Music* Assets::getMusic( const std::string& id )
{
    return m_musics.count( id ) ? m_musics[ id ] : 0;
}

sf::Font* Assets::getFont( const std::string& id )
{
    return m_fonts.count( id ) ? m_fonts[ id ] : 0;
}

void Assets::freeTexture( const std::string& id )
{
    if( m_textures.count( id ) )
    {
        sf::Texture* t = m_textures[ id ];
        DELETE_OBJECT( t );
        m_textures.erase( id );
    }
}

void Assets::freeShader( const std::string& id )
{
    if( m_shaders.count( id ) )
    {
        sf::Shader* t = m_shaders[ id ];
        DELETE_OBJECT( t );
        m_shaders.erase( id );
    }
}

void Assets::freeSound( const std::string& id )
{
    if( m_sounds.count( id ) )
    {
        sf::Sound* t = m_sounds[ id ];
        DELETE_OBJECT( t );
        m_sounds.erase( id );
    }
    if( m_soundsBuffs.count( id ) )
    {
        sf::SoundBuffer* t = m_soundsBuffs[ id ];
        DELETE_OBJECT( t );
        m_soundsBuffs.erase( id );
    }
}

void Assets::freeMusic( const std::string& id )
{
    if( m_musics.count( id ) )
    {
        sf::Music* t = m_musics[ id ];
        DELETE_OBJECT( t );
        m_musics.erase( id );
    }
}

void Assets::freeFont( const std::string& id )
{
    if( m_fonts.count( id ) )
    {
        sf::Font* t = m_fonts[ id ];
        DELETE_OBJECT( t );
        m_fonts.erase( id );
    }
}

void Assets::freeAllTextures()
{
    for( std::map< std::string, sf::Texture* >::iterator it = m_textures.begin(); it != m_textures.end(); it++ )
        DELETE_OBJECT( it->second );
    m_textures.clear();
}

void Assets::freeAllShaders()
{
    for( std::map< std::string, sf::Shader* >::iterator it = m_shaders.begin(); it != m_shaders.end(); it++ )
        DELETE_OBJECT( it->second );
    m_shaders.clear();
}

void Assets::freeAllSounds()
{
    for( std::map< std::string, sf::SoundBuffer* >::iterator it = m_soundsBuffs.begin(); it != m_soundsBuffs.end(); it++ )
        DELETE_OBJECT( it->second );
    m_soundsBuffs.clear();
    for( std::map< std::string, sf::Sound* >::iterator it = m_sounds.begin(); it != m_sounds.end(); it++ )
        DELETE_OBJECT( it->second );
    m_sounds.clear();
}

void Assets::freeAllMusics()
{
    for( std::map< std::string, sf::Music* >::iterator it = m_musics.begin(); it != m_musics.end(); it++ )
        DELETE_OBJECT( it->second );
    m_musics.clear();
}

void Assets::freeAllFonts()
{
    for( std::map< std::string, sf::Font* >::iterator it = m_fonts.begin(); it != m_fonts.end(); it++ )
        DELETE_OBJECT( it->second );
    m_fonts.clear();
}

void Assets::freeAll()
{
    freeAllTextures();
    freeAllShaders();
    freeAllSounds();
    freeAllMusics();
    freeAllFonts();
}
