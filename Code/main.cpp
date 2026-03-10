#include <iosfwd>

#include "grid/ChunkManager.hpp"
#include "grid/GridRendering.hpp"
#include "generation/ChunkGenerator.hpp"


[[nodiscard]] sf::Vector2f gatherOffset(sf::Time dt) {
    static constexpr float axisOffset = 2.0f;
    
    static sf::Vector2f velocity{};
    velocity *= std::pow(0.70f, dt.asSeconds() * 60.0f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        velocity += sf::Vector2f(0.0f, -axisOffset);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        velocity += sf::Vector2f(-axisOffset, 0.0f);
    }        
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        velocity += sf::Vector2f(0.0f, axisOffset);
    }       
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        velocity += sf::Vector2f(axisOffset, 0.0f);
    }

    return velocity;
}




int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Test");
    window.setVerticalSyncEnabled(true);
    
    sf::Clock clock;
    ne::ChunkManager manager;

    while(window.isOpen())
    {
        sf::Time dt = clock.restart();

        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if(auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f pos = window.mapPixelToCoords(mouse->position);
                manager.setTile(ne::Tile{8, 0}, ne::GridPosition::fromWorld(pos));
            }
        }
        
        //Hacked together movement script -> used for debugging
        sf::View view = window.getView();
        view.move(gatherOffset(dt));
        window.setView(view);

        ne::visibleChunkGenerator(window, manager);
        

        const sf::Vector2i windowSize{window.getSize()};
    
        const sf::Vector2f topLeft     = window.mapPixelToCoords({0, 0});
        const sf::Vector2f bottomRight = window.mapPixelToCoords(windowSize);
        
        const sf::FloatRect windowRect{topLeft, bottomRight - topLeft};
        const ne::GridArea chunkArea = ne::GridArea::fromWorld(windowRect);
        
        //Sample showcasing how easy it is to apply properties to tiles
        manager.queryTiles(chunkArea, [&](ne::Tile& tile, ne::GridPosition pos) {            
            if(tile.value == ne::Tiles::Dirt)
            {
                sf::Vector2i adjustedTilePos = pos.asTile();
                adjustedTilePos.y -= 1;
                
                const ne::Tile* tileAbovePtr = manager.getTile(ne::GridPosition::fromTile(adjustedTilePos));
                
                if(tileAbovePtr && tileAbovePtr->value == ne::Tiles::Air)
                {
                    manager.setTile({ne::Tiles::Grass, tile.rotation}, pos);
                }
            }
        });


        window.clear(sf::Color::Black);
        
        ne::drawVisibleChunks(window, manager);
        
        window.display();
    }
}
