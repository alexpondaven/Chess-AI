#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctype.h>
#include <vector>
using namespace sf;


Sprite p[32];
Sprite* pboard[8][8] = { 0 }; // 2d array of addresses for each piece to know each location
std::string ptypes[8][8] = { "" }; // corresponding type of sprite in pboard

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
    std::string initial_types = "KQBNRPprnbqk"; // initial types of pieces (from image order)
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
            // store piece type in ptypes array
            if (n > 0) ptypes[j][i] = initial_types[n - 1]; // white pieces are uppercase (beginning of initial_types string)
            else if (n<0) ptypes[j][i] = initial_types[12 + n]; // black pieces are lowercase (end of initial_types string)
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

// update array of pieces that are protected (and if king is in check)
// could actually make a valid moves list for each piece (thinking may be needed for hidden checks (and any ai))
// for each piece= a list of all possible moves 
// for each Sprite* need 


// need to see what is defended (so king can't attack), if king is in check, if move is valid, if move hopped over a piece
// defending only happens if king is attacking (so could just call isDefended function on capturePiece to determine if it is defended

bool validMove(Sprite* pmoving,Vector2i oldCoord, Vector2i newCoord) { // decide if move is valid
    // helper variables for deciding if move is valid
    Sprite* capturePiece = pboard[newCoord.x][newCoord.y]; // piece on new square that may be captured
    std::string attackType = ptypes[oldCoord.x][oldCoord.y]; // moving piece type
    std::string captureType = ptypes[newCoord.x][newCoord.y]; // type of piece being attacked

    bool isDiagonal = (abs(oldCoord.x - newCoord.x) == abs(oldCoord.y - newCoord.y)); // diagonal move if x and y offset is same
    
    

    // piece specific calculations
    bool validDir = 0; // is the piece being moved as it should
    bool isHop = 0; // is there a piece between the old and new position (i.e. is piece "hopping" over another)
    if (attackType == "R" || attackType == "r") { // rook moving
        validDir = !isDiagonal;
        
        // look in direction of movement if valid
        if (validDir) {
            // check if it hopped over something

            // check if it is now checking
        }
    }
    else if (attackType == "B" || attackType == "b") { // bishop moving
        validDir = isDiagonal;
    }
    else if (attackType == "Q" || attackType == "q") { // queen moving
        validDir = 1;
    }
    else if (attackType == "K" || attackType == "k") { // king moving
        validDir = 1;
    }
    else if (attackType == "N" || attackType == "n") { // knight moving
        validDir = 1;
    }
    else if (attackType == "P" || attackType == "p") { // pawn moving
        validDir = 1;
    }

    //check if king being mated
    
    // decide if it is a valid move
    bool valid = 0;
    if (capturePiece == 0) { // moved to empty square
        valid = validDir;
    }
    else if (capturePiece != 0) { // attacking a piece
        // piece must be moving correctly and be attacking the opposite colour (not both uppercase or both lowercase in ptypes array)
        bool isKing = (captureType == "K") || (captureType == "k");
        valid = validDir && (isupper(attackType[0]) ^ isupper(captureType[0])) && !isKing;
        
    }
    
    return valid;
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

            // event of mouse button being pressed
            if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left && moving==0)
            {
                // Update pmoving with address to piece that is being dragged
                Vector2i clickCoord = Vector2i(int(mousePos.x / psize), int(mousePos.y / psize));
                pmoving = pboard[clickCoord.x][clickCoord.y];
                //printf("moving piece at %i %i", clickCoord.x, clickCoord.y);

                if (pmoving != 0) { // if mouse was pressed on piece
                    //printf("moving");
                    moving = 1;
                    oldCoord = clickCoord; // store previous position of piece (coord from which it was picked up)
                    
                    // remove piece from board
                    pboard[oldCoord.x][oldCoord.y] = 0;


                    //offx = mousePos.x - piece.getPosition().x;
                    //offy = mousePos.y - piece.getPosition().y;
                }  
            }
            // event of mouse button being released
            if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left && (pmoving !=0)) {
                //printf("mouserelease\n");
                moving = 0;
                Vector2f pos = pmoving->getPosition();
                Vector2f newPos = Vector2f(float(int(pos.x / psize) + 0.5) * psize, float(int(pos.y / psize) + 0.5) * psize);
                newCoord = Vector2i(int(pos.x / psize), int(pos.y / psize));

                // only move to piece to new coordinate if newCoord is in window
                bool outOfBounds = (mousePos.x >= window.getSize().x) || (mousePos.x < 0) || (mousePos.y >= window.getSize().y) || (mousePos.y < 0); // outside window
                bool valid = 0;
                if (!outOfBounds) { // need to check this or else newCoord is not a valid square
                    valid = validMove(pmoving, oldCoord, newCoord);
                }
                
                if (!valid || outOfBounds) { // move piece back to old square
                    //printf("%i, %i\n", oldCoord.x, oldCoord.y);

                    // place pmoving (moving piece) back to where it was before dragging
                    pmoving->setPosition(float((oldCoord.x + 0.5)) * psize, float((oldCoord.y + 0.5)) * psize);
                    pboard[oldCoord.x][oldCoord.y] = pmoving; 
                }
                else { // move piece to new square
                    // make decision about 
                    //remove old piece on button release coordinate and add new piece to pboard
                    if (pboard[newCoord.x][newCoord.y] != 0) { // remove what was on the new square (if not empty)
                        //printf("removing piece at x=%i, y=%i", newCoord.x, newCoord.y);
                        pboard[newCoord.x][newCoord.y]->setPosition(-100, -100); // move piece in new position away from board
                    }
                    pmoving->setPosition(newPos);
                    pboard[newCoord.x][newCoord.y] = pmoving;
                    //update type of piece on new square and old square
                    ptypes[newCoord.x][newCoord.y] = ptypes[oldCoord.x][oldCoord.y];
                    // reset old square only if it is different from new square
                    if (newCoord != oldCoord) {
                        ptypes[oldCoord.x][oldCoord.y] = "";
                    }
                    printf("type of piece moved to %i, %i is ", newCoord.x, newCoord.y);
                    std::cout << ptypes[newCoord.x][newCoord.y] << std::endl;

                }
                pmoving = 0; // reset pmoving to 0
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