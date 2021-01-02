#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;


Sprite p[32];
Sprite* pboard[8][8] = { 0 }; // 2d array of addresses for each piece to know each location
std::string ptypes[32] = { "" }; // corresponding piece of sprite in p

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
    std::string initial_types = "KQBNR"; // initial types of pieces (from image order)
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
            pboard[j][i] = &p[count];
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
    float psize = (float)width / 8.0f; // square size
    float scaled = 0.9; // how much to scale down the pieces
    Vector2i oldCoord, newCoord;

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
    Sprite* pmoving=0; // address of piece being moved
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
            if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left && moving==0)
            {
                Vector2i clickCoord = Vector2i(int(mousePos.x / psize), int(mousePos.y / psize));

                pmoving = pboard[clickCoord.x][clickCoord.y];
                //printf("moving piece at %i %i", clickCoord.x, clickCoord.y);

                if (pmoving != 0) {
                    //printf("moving");
                    moving = 1;
                    oldCoord = clickCoord;
                    
                    // remove piece from board
                    pboard[oldCoord.x][oldCoord.y] = 0;
                    //offx = mousePos.x - piece.getPosition().x;
                    //offy = mousePos.y - piece.getPosition().y;
                }  
            }

            if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left && (pmoving !=0)) {
                //printf("mouserelease\n");
                moving = 0;
                Vector2f pos = pmoving->getPosition();
                Vector2f newPos = Vector2f(float(int(pos.x / psize) + 0.5) * psize, float(int(pos.y / psize) + 0.5) * psize);
                newCoord = Vector2i(int(pos.x / psize), int(pos.y / psize));

                // only move to piece to new coordinate if newCoord is in window
                if ((mousePos.x >= window.getSize().x) || (mousePos.x < 0) || (mousePos.y >= window.getSize().y) || (mousePos.y < 0)) { // outside window
                    //printf("%i, %i\n", oldCoord.x, oldCoord.y);
                    pmoving->setPosition(float((oldCoord.x + 0.5)) * psize, float((oldCoord.y + 0.5)) * psize);
                    pboard[oldCoord.x][oldCoord.y] = pmoving; 
                }
                else {
                    //remove old piece on button release coordinate and add new piece to pboard
                    if (pboard[newCoord.x][newCoord.y] != 0) {
                        //printf("removing piece at x=%i, y=%i", newCoord.x, newCoord.y);
                        pboard[newCoord.x][newCoord.y]->setPosition(-100, -100); // move piece in new position away from board
                    }
                    pmoving->setPosition(newPos);
                    pboard[newCoord.x][newCoord.y] = pmoving;

                }
            }
        }

        if (moving) {
            // use offx or offy to drag and keep relative position of cursor to piece
            //piece.setPosition(mousePos.x-offx, mousePos.y-offy);
            pmoving->setPosition(Vector2f(mousePos));

        }
        // circle at cursor
        //CircleShape c(10);
        //c.setPosition(Vector2f(mousePos)-Vector2f(5,5));

        
        // draw
        window.clear();
        window.draw(board);
        for (int i = 0; i < 32; i++) {
            window.draw(p[i]);
            //window.draw(boundingBox(p[i])); // draw bounding box around pieces
        }
        if (pmoving != 0) { // make sure piece that is moving is drawn ontop of the other pieces
            window.draw(*pmoving);
        }
        //window.draw(c);
        window.display();
    }

    return 0;
}