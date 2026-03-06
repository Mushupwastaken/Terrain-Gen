#include <iosfwd>

#include "grid/ChunkManager.hpp"
#include "grid/GridRendering.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Test");
    window.setFramerateLimit(20u);
    
    ne::ChunkManager manager;

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
        }
        
        //Hacked together movement script -> used for debugging
        sf::View view = window.getView();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            view.move({0.0f, -ne::constants::renderScale});
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            view.move({-ne::constants::renderScale, 0.0f});
        }        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            view.move({0.0f, ne::constants::renderScale});
        }       
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            view.move({ne::constants::renderScale, 0.0f});
        }
        window.setView(view);   
  
        ne::visibleChunkGenerator(window, manager);
        
        window.clear(sf::Color::Black);
        
        ne::drawVisibleChunks(window, manager);
        
        window.display();
    }
}


