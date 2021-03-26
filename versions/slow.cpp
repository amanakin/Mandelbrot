#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>

//=================================================================================================

int main()
{
    const int    window_width  = 16 * 50,
                 window_height = 9  * 50;

    const size_t screen_width  = 1920,
                 screen_height = 1080;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Mandelbrot");
    window.setPosition(sf::Vector2i(1920.f / 2 - window_width / 2, 1080.f / 2 - window_height / 2)); // Set window to the center of screen

    sf::Sprite sprite;
    sf::Texture texture;
    texture.create(window_width, window_height);

    sf::Uint32 screen[window_height][window_width];

    const int nMax = 250;
    
    double graph_dot = 1.6f / window_width, 
           rMax2 = 10.f;

    double x_offset = 0,
           y_offset = 0;

    int colwidth  = 27;
    int coloffset = 81;

    sf::Clock clock;
    while(window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            x_offset -= graph_dot * 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            x_offset += graph_dot * 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            y_offset -= graph_dot * 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            y_offset += graph_dot * 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
            graph_dot *= 1.1f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            graph_dot /= 1.1f;

        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int y_window = 0; y_window < window_height; y_window++)
        {
            double x0 = (-window_width / 2) * graph_dot + x_offset,
                   y0 = (y_window - window_height / 2) * graph_dot + y_offset;

            for (int x_window = 0; x_window < window_width; x_window++, x0 += graph_dot)
            {
                double X = x0,
                       Y = y0;

                int N = 0;

                for (; N < nMax; N++)
                {
                    double x2 = X * X,
                           y2 = Y * Y,
                           xy = X * Y;

                    double r2 = x2 + y2;

                    if (r2 >= rMax2) break;
                    X = x2 - y2 + x0,
                    Y = xy + xy + y0;
                }
                
                sf::Color color = sf::Color::Black;

                if (N < nMax)
                {                
                    N = (N + coloffset) % (3 * colwidth);
            
                    if (N / colwidth == 0)
                    {
                        color.r = 237*N/colwidth;
                        color.g = 11+244*N/colwidth;
                        color.b = 116+139*N/colwidth;
                    }
                    else if (N / colwidth == 1)
                    {
                        N -= colwidth;
                        color.r = 238;
                        color.g = 255-123*N/colwidth;
                        color.b = 255-253*N/colwidth;
                    }
                    else
                    {
                        N -= 2 * colwidth;
                        color.r = 239-239*N/colwidth;
                        color.g = 132-121*N/colwidth;
                        color.b = 2+114*N/colwidth;
                    }
                }
                screen[y_window][x_window] = __builtin_bswap32(color.toInteger());
            }
        }

        sf::Time timer = clock.getElapsedTime();
        clock.restart();
        fflush(stdout);
        printf("%lg\r", 1. / timer.asSeconds());
        texture.update((sf::Uint8 *)screen);
        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();
    }
}
