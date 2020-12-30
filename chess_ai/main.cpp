#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

int main()
{
    int width = 64;
    int height = 64;
    RenderWindow window(VideoMode(width, height), "Chess", Style::Close | Style::Resize);
 
    Texture pieces, boardi;
    pieces.loadFromFile("images/pieces.png");
    boardi.loadFromFile("images/board.png");

    Sprite piece(pieces);
    Sprite board(boardi);
    board.setScale(3,3);

    

    Vector2u textureSize = pieces.getSize();
    textureSize.x /= 6;
    textureSize.y /= 2;

    //player.setTextureRect(IntRect(textureSize.x * 0, textureSize.y * 0, textureSize.x, textureSize.y));

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case Event::Closed:
                window.close();
                break;
            }

            // dragging
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                Vector2i mousePos = Mouse::getPosition(window);
                piece.setPosition((float)mousePos.x - 150.0f, (float)mousePos.y - 150.0f);
            }
        }



        // draw
        window.clear();
        // draw board (181, 136, 99)
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                RectangleShape r(Vector2f(width / 8, width / 8));
                if (i + j % 2 == 0) { // white square
                    r.setFillColor(Color(240, 217, 181));
                } else {
                    r.setFillColor(Color(181, 136, 99));
                }
                //printf("x: %i, y: %i\n", i * (width / 8), j * (height / 8));
                r.setPosition(i * (int)(width/8), j * (int)(height/8));
                window.draw(r);
                //std::cout << i << " " << j << '\n' << std::endl;
            }
        }
        
        //window.draw(board);
        //window.draw(piece);
        window.display();
    }

    return 0;
}