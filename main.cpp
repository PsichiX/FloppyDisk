#include <iostream>
#include <list>
#include <sstream>
#include "Assets.h"
#include "Events.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include <XeCore/Common/Base.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <Box2D/Box2D.h>

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 574;
const sf::Color WINDOW_COLOR = sf::Color( 128, 128, 128, 255 );

void onEvent( Events::Event* ev )
{
}

int main()
{
    Events::use().setCallback( &onEvent );

    /// scene
    sf::RenderWindow* window = xnew sf::RenderWindow(
        sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ),
        "Floppy Disk",
        sf::Style::Titlebar | sf::Style::Close
    );
    sf::View* camera = xnew sf::View(
        sf::Vector2f( (float)window->getSize().x * 0.5f, (float)window->getSize().y * 0.5f ),
        sf::Vector2f( (float)window->getSize().x, (float)window->getSize().y )
    );
    camera->zoom( 0.5f );

    /// assets
    Assets::use().loadFont( "font", "assets/fonts/CONTF.ttf" );
    Assets::use().loadTexture( "floppy", "assets/textures/floppy.png" );
    Assets::use().loadTexture( "magnet", "assets/textures/magnet.png" );

    /// game manager
    GameManager* gameManager = xnew GameManager( 10.0f, 0.0f );

    /// game objects
    GameObject* floppy = xnew GameObject( "floppy" );
    floppy->addComponent( xnew Transform( sf::Vector2f( camera->getCenter() ) ) );
    floppy->addComponent( xnew SpriteRenderer( Assets::use().getTexture( "floppy" ) ) );
    gameManager->addGameObject( floppy );

    /// main loop
    srand( time( 0 ) );
    sf::Clock timer;
    sf::Clock deltaTimer;
    while( window->isOpen() )
    {
        sf::Event event;
        while( window->pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
            {
                window->close();
            }
            else if( event.type == sf::Event::KeyPressed )
            {
                if( event.key.code == sf::Keyboard::Escape )
                    window->close();
            }
        }
        /// timers update
        float dt = deltaTimer.getElapsedTime().asSeconds();
        deltaTimer.restart();

        /// process frame
        Events::use().dispatch();
        gameManager->processPhysics( dt * 1000.0f );
        gameManager->processUpdate( dt );
        window->clear( WINDOW_COLOR );
        window->setView( *camera );
        gameManager->processRender( window );
        window->display();
        XeCore::Common::Concurrent::Thread::sleep( 1000 / 30 );
    }

    DELETE_OBJECT( window );
    DELETE_OBJECT( camera );
    DELETE_OBJECT( gameManager );
    Assets::destroy();
    Events::destroy();

    return 0;
}
