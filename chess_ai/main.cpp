#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;


Sprite p[32];

// numbers on board represent offset on figure
int aboard[8][8] = {{-5,-4,-3,-2,-1,-3,-4,-5},
                    {-6,-6,-6,-6,-6,-6,-6,-6},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 0, 0, 0, 0, 0, 0, 0, 0},
                    { 6, 6, 6, 6, 6, 6, 6, 6},
                    { 5, 4, 3, 2, 1, 3, 4, 5} };

void loadPos(float sw, float sh, float psize, float scaled) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n = aboard[i][j];
            if (!n) continue;
            int row = abs(n) - 1;
            int col = n<0;
            p[count].setTextureRect(IntRect(sw * row, sh * col, sw, sh));
            p[count].setOrigin(sw / 2, sh / 2);
            p[count].setScale( (psize / sw) * scaled, (psize / sh) * scaled);
            p[count].setPosition(psize * (j + 0.5), psize * (i + 0.5));
            count++;
        }
    }
}

RectangleShape boundingBox(Sprite s) {
    // draw rectangle around sprite
    RectangleShape rect;
    rect.setSize(Vector2f(s.getGlobalBounds().width, s.getGlobalBounds().height));
    rect.setFillColor(Color::Transparent);
    rect.setOutlineColor(Color::Red);
    rect.setOutlineThickness(1);
    rect.setPosition(s.getPosition() - rect.getSize() / 2.0f);
    return rect;
}

int main()
{
    // parameters
    int width = 512;
    int height = 512;
    float psize = (float)width / 8.0f; // piece size
    float scaled = 0.9; // how much to scale down the pieces

    // render window
    RenderWindow window(VideoMode(width, height), "Chess", Style::Close | Style::Resize);
 
    // Adding images and sprites
    Texture pieces, boardi;
    pieces.loadFromFile("images/pieces2.png");
    boardi.loadFromFile("images/board1.png");

    // scale board size to fit window size
    Vector2u boardSize = boardi.getSize();
    Sprite board(boardi);
    board.setScale(width / (float)boardSize.x, height / (float)boardSize.y);

    // scale sprites
    Vector2u textureSize = pieces.getSize();
    int sw = textureSize.x / 6;
    int sh = textureSize.y / 2;
    // pass in window width/height and sprite width/height
    //pieces.setSmooth(true); // can't tell if this helps
    for (int i=0;i<32;i++) p[i].setTexture(pieces);
    loadPos((float)sw, (float)sh, psize, scaled);
    
   

    bool moving = 0; // is piece being dragged
    int pmoving = 0; // piece being moved
    //float offx = 0, offy = 0;

    while (window.isOpen())
    {
        Vector2i mouse = Mouse::getPosition(window);
        Vector2i mousePos = Vector2i(window.mapPixelToCoords(mouse)); // fixes mouse position when rescaling window

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed){
                window.close();
            }

            // dragging
            if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left)
            {
                for (int i = 0; i < 32; i++) {
                    if (p[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        moving = 1;
                        pmoving = i;
                        //offx = mousePos.x - piece.getPosition().x;
                        //offy = mousePos.y - piece.getPosition().y;
                    }
                }
                
                
            }

            if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left) {
                moving = 0;
                // position of center of piece
                
                Vector2f pos = p[pmoving].getPosition();
                Vector2f nPos = Vector2f(float(int(pos.x / psize)+0.5) * psize, float(int(pos.y / psize)+0.5) * psize);
                p[pmoving].setPosition(nPos);
            }
        }

        if (moving) {
            // use offx or offy to drag and keep relative position of cursor to piece
            //piece.setPosition(mousePos.x-offx, mousePos.y-offy);
            p[pmoving].setPosition(Vector2f(mousePos));

        }
        // circle at cursor
        //CircleShape c(10);
        //c.setPosition(Vector2f(mousePos)-Vector2f(5,5));

        
        // draw
        window.clear();
        window.draw(board);
        for (int i = 0; i < 32; i++) {
            window.draw(p[i]);
            window.draw(boundingBox(p[i])); // draw bounding box around pieces
        }
        //window.draw(c);
        window.display();
    }

    return 0;
}