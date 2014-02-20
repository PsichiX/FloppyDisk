#ifndef __ASSETS__
#define __ASSETS__

#include <XeCore/Common/Base.h>
#include <XeCore/Common/IRtti.h>
#include <XeCore/Common/MemoryManager.h>
#include <XeCore/Common/Singleton.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

class Assets
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public XeCore::Common::Singleton< Assets >
{
    RTTI_CLASS_DECLARE( Assets );

public:
    Assets();
    ~Assets();

    sf::Texture* addTexture( const std::string& id, const sf::Texture* ptr );
    sf::Shader* addShader( const std::string& id, const sf::Shader* ptr );
    sf::Sound* addSound( const std::string& id, const sf::SoundBuffer* ptrbuff, const sf::Sound* ptr );
    sf::Music* addMusic( const std::string& id, const sf::Music* ptr );
    sf::Font* addFont( const std::string& id, const sf::Font* ptr );

    sf::Texture* loadTexture( const std::string& id, const std::string& path );
    sf::Shader* loadShader( const std::string& id, const std::string& vspath, const std::string& fspath );
    sf::Sound* loadSound( const std::string& id, const std::string& path );
    sf::Music* loadMusic( const std::string& id, const std::string& path );
    sf::Font* loadFont( const std::string& id, const std::string& path );

    sf::Texture* getTexture( const std::string& id );
    sf::Shader* getShader( const std::string& id );
    sf::Sound* getSound( const std::string& id );
    sf::Music* getMusic( const std::string& id );
    sf::Font* getFont( const std::string& id );

    void freeTexture( const std::string& id );
    void freeShader( const std::string& id );
    void freeSound( const std::string& id );
    void freeMusic( const std::string& id );
    void freeFont( const std::string& id );
    void freeAllTextures();
    void freeAllShaders();
    void freeAllSounds();
    void freeAllMusics();
    void freeAllFonts();
    void freeAll();

private:
    std::map< std::string, sf::Texture* > m_textures;
    std::map< std::string, sf::Shader* > m_shaders;
    std::map< std::string, sf::Sound* > m_sounds;
    std::map< std::string, sf::SoundBuffer* > m_soundsBuffs;
    std::map< std::string, sf::Music* > m_musics;
    std::map< std::string, sf::Font* > m_fonts;
};

#endif