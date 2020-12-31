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

void loadPos(int w, int h, int sw, int sh) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n = aboard[i][j];
            if (!n) continue;
            int row = abs(n) - 1;
            int col = n<0;
            p[count].setTextureRect(IntRect(sw * row, sh * col, sw, sh));
            p[count].setScale(((float)w / 8) / (float)sw, ((float)h / 8) / (float)sh);
            p[count].setPosition(((float)w / 8) * j, ((float)h / 8) * i);
            count++;
        }
    }
}

int main()
{
    // setting up window
    int width = 512;
    int height = 512;
    RenderWindow window(VideoMode(width, height), "Chess", Style::Close | Style::Resize);
 
    // Adding images and sprites
    Texture pieces, boardi;
    pieces.loadFromFile("images/pieces.png");
    boardi.loadFromFile("images/board.png");

    Sprite piece(pieces);
    Sprite board(boardi);
    
    // scale board size to fit window size
    Vector2u boardSize = boardi.getSize();
    board.setScale(width / (float)boardSize.x, height / (float)boardSize.y);

    // scale sprites
    Vector2u textureSize = pieces.getSize();
    int sw = textureSize.x / 6;
    int sh = textureSize.y / 2;
    // pass in window width/height and sprite width/height
    for (int i=0;i<32;i++) p[i].setTexture(pieces);
    loadPos(width, height, sw, sh);
    
   

    bool moving = 0;
    int pmoving = 0; // piece being moved
    //float offx = 0, offy = 0;

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
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
            }
        }

        if (moving) {
            // use offx or offy to drag and keep relative position of cursor to piece
            //piece.setPosition(mousePos.x-offx, mousePos.y-offy);
            p[pmoving].setPosition(mousePos.x - p[pmoving].getGlobalBounds().width / 2, mousePos.y - p[pmoving].getGlobalBounds().height / 2);
        }

        // draw
        window.clear();
        // draw board with squares - too laggy
        //for (int i = 0; i < width; i++) {
        //    for (int j = 0; j < height; j++) {
        //        RectangleShape r(Vector2f(width / 8, width / 8));
        //        if ((i + j) % 2 == 0) { // white square
        //            r.setFillColor(Color(240, 217, 181));
        //        } else {
        //            r.setFillColor(Color(181, 136, 99));
        //        }
        //        //printf("x: %i, y: %i\n", i * (width / 8), j * (height / 8));
        //        r.setPosition(i * (int)(width/8), j * (int)(height/8));
        //        window.draw(r);
        //        //std::cout << i << " " << j << '\n' << std::endl;
        //    }
        //}
        
        window.draw(board);
        for (int i = 0; i < 32; i++) window.draw(p[i]);
        window.display();
    }

    return 0;
}