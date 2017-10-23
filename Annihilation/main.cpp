#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "ResourcePath.hpp"

bool windowOpen = true;
int windowWidth = 800;
int windowHeight = 600;

void renderThread(sf::RenderWindow* window)
{
    while (windowOpen)
    {
        // Clear screen
        window->clear();
        
        // Update the window
        window->display();
    }
    
    window->close();
}

int main(int, char const**)
{
    bool fullscreen = false;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Annihilation");
    window.setActive(false);
    
    // Create and start render thread
    sf::Thread thread(&renderThread, &window);
    thread.launch();

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png"))
    {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
            {
                windowOpen = false;
                thread.wait();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                windowOpen = false;
                thread.wait();
            }
            
            // Fullscreen check
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F)
            {
                windowOpen = false;
                thread.wait();
                
                if (fullscreen)
                {
                    window.create(sf::VideoMode(windowWidth, windowHeight), "Annihilation");
                    fullscreen = false;
                }
                else
                {
                    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
                    int resX, resY;
                    
                    resX = desktopMode.width;
                    resY = desktopMode.height;
                    
                    window.create(sf::VideoMode(resX, resY), "Annihilation", sf::Style::Fullscreen);
                    fullscreen = true;
                }
            
                windowOpen = true;
                thread.launch();
            }
        }
    }

    return EXIT_SUCCESS;
}
