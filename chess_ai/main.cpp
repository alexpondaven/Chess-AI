#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctype.h>
#include <vector>
using namespace sf;


Sprite p[32];
Sprite* pboard[8][8] = { 0 }; // 2d array of addresses for each piece to know each location
std::string ptypes[8][8] = { "" }; // corresponding type of piece in pboard
std::vector<Vector2i> legalMoves[8][8]; // each piece has a vector of legal moves
bool defended[8][8] = { 0 };

// Special variables for special moves e.g. checking, castling and en passant
//king variables: store if each king is in check or if one of them (or the rook) has moved from original position 


// numbers on board represent offset on figure (used for extracting pieces from image)
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

// remember legal moves has to include staying still

// need to see what is defended (so king can't attack), if king is in check, if move is valid, if move hopped over a piece
// defending only happens if king is attacking (so could just call isDefended function on capturePiece to determine if it is defended

int legalSquare(std::string moveType, std::string squareType, Vector2i coord) {
    if (squareType == "") { // empty square
        return 1;
    }
    else if (isupper(moveType[0]) ^ isupper(squareType[0])) { // if piece moving to square with piece of opposite colour it is allowed
        if (!(squareType == "K" || squareType == "k")) {// legal move only if piece is not a king
            return 2; 
        }
        else { // king is in check
            if (isupper(squareType[0])) { // K in check

            }
            else { // k in check

            }
        }
    }
    else { // piece is of same colour so piece is defended
        defended[coord.x][coord.y] = 1;
    }
    return 0;
}

bool legalPawn(std::string moveType, std::string squareType, Vector2i coord, std::string direction) {
    // update 

    if (squareType == "") { // empty square
        // check if pawn below this new position has just moved 2 forward
        bool en_passant = 0;
        return (direction != "d" || en_passant); // cannot move diagonally to empty square (IF NOT EN PASSANT)
    }
    else if (isupper(moveType[0]) ^ isupper(squareType[0])) { // if piece moving to square with piece of opposite colour it is allowed
        return (direction == "d"); // can only capture diagonally
    }
    else { // piece is of same colour so piece is defended (but only if it is diagonal)
        if (direction=="d") defended[coord.x][coord.y] = 1;
    }
    return 0;
}

void updateLegalMoves() { // update legal moves for the moving player (uppercase) and defended ones
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // reset legalMoves and defended arrays
            legalMoves[i][j] = std::vector<Vector2i>{};
            defended[i][j] = 0;
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("inspecting coord (%i,%i)", i, j);
            
            // calculate legal moves for piece on pboard[i][j] and append to legalMoves 2d array
            std::string pieceType = ptypes[i][j];
            std::string squareType;
            int legal = 0;
            // always include coord of itself
            legalMoves[i][j].push_back(Vector2i(i, j));
            if (pieceType == "R" || pieceType == "r" || pieceType == "Q" || pieceType == "q") { // rook 
                printf("rook moves\n");
                for (int v = i-1; v >=0; v--) {
                    squareType = ptypes[v][j];
                    printf("to %i,%i at ",v,j);
                    std::cout << squareType << std::endl;
                    legal = legalSquare(pieceType, squareType, Vector2i(v, j));
                    if (legal) {
                        printf("legal square at (%i,%i)\n", v, j);
                        legalMoves[i][j].push_back(Vector2i(v, j));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;
                    
                }
                for (int v = i+1; v <8; v++) {
                    squareType = ptypes[v][j];
                    printf("to %i,%i at ", v, j);
                    std::cout << squareType << std::endl;
                    legal = legalSquare(pieceType, squareType, Vector2i(v, j));
                    if (legal) {
                        printf("legal square at (%i,%i)", v, j);
                        legalMoves[i][j].push_back(Vector2i(v, j));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;
                }
                for (int h = j-1; h >= 0; h--) {
                    squareType = ptypes[i][h];
                    printf("to %i,%i at ", i, h);
                    std::cout << squareType << std::endl;
                    legal = legalSquare(pieceType, squareType, Vector2i(i, h));
                    if (legal) {
                        printf("legal square at (%i,%i)", i, h); 
                        legalMoves[i][j].push_back(Vector2i(i, h));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;
                }
                for (int h = j+1; h <8; h++) {
                    squareType = ptypes[i][h];
                    printf("to %i,%i at ", i, h);
                    std::cout << squareType << std::endl;
                    legal = legalSquare(pieceType, squareType, Vector2i(i, h));
                    if (legal) {
                        printf("legal square at (%i,%i)", i, h); 
                        legalMoves[i][j].push_back(Vector2i(i, h));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;
                }
            }
            if (pieceType == "B" || pieceType == "b" || pieceType == "Q" || pieceType == "q") { // bishop
                for (std::pair<int, int> v(i + 1, j + 1); v.first < 8 && v.second < 8; v.first++, v.second++) {
                    squareType = ptypes[v.first][v.second];
                    legal = legalSquare(pieceType, squareType, Vector2i(v.first, v.second));
                    if (legal) {
                        legalMoves[i][j].push_back(Vector2i(v.first, v.second));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;

                }
                for (std::pair<int, int> v(i - 1, j - 1); v.first >= 0 && v.second >= 0; v.first--, v.second--) {
                    squareType = ptypes[v.first][v.second];
                    legal = legalSquare(pieceType, squareType, Vector2i(v.first, v.second));
                    if (legal) {
                        legalMoves[i][j].push_back(Vector2i(v.first, v.second));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;

                }
                for (std::pair<int, int> v(i + 1, j - 1); v.first < 8 && v.second >=0; v.first++, v.second--) {
                    squareType = ptypes[v.first][v.second];
                    legal = legalSquare(pieceType, squareType, Vector2i(v.first, v.second));
                    if (legal) {
                        legalMoves[i][j].push_back(Vector2i(v.first, v.second));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;

                }
                for (std::pair<int, int> v(i - 1, j + 1); v.first >=0 && v.second < 8; v.first--, v.second++) {
                    squareType = ptypes[v.first][v.second];
                    legal = legalSquare(pieceType, squareType, Vector2i(v.first, v.second));
                    if (legal) {
                        legalMoves[i][j].push_back(Vector2i(v.first, v.second));
                        if (legal == 2) break; // it was a capture
                    }
                    else break;

                }
            }
            if (pieceType == "N" || pieceType == "n") { // knight
                // create list of 8 possible moves
                std::vector<Vector2i> knightmoves;
                if (i + 2 < 8 && j + 1 < 8) knightmoves.push_back(Vector2i(i + 2, j + 1));
                if (i + 2 < 8 && j - 1 >= 0) knightmoves.push_back(Vector2i(i + 2, j - 1));
                if (i + 1 < 8 && j + 2 < 8) knightmoves.push_back(Vector2i(i + 1, j + 2));
                if (i - 1 >= 0 && j + 2 < 8) knightmoves.push_back(Vector2i(i - 1, j + 2));
                if (i - 2 >= 0 && j + 1 < 8) knightmoves.push_back(Vector2i(i - 2, j + 1));
                if (i - 2 >= 0 && j - 1 >= 0) knightmoves.push_back(Vector2i(i - 2, j - 1));
                if (i + 1 < 8 && j - 2 >= 0) knightmoves.push_back(Vector2i(i + 1, j - 2));
                if (i - 1 >= 0 && j - 2 >= 0) knightmoves.push_back(Vector2i(i - 1, j - 2));

                for (int n = 0; n < knightmoves.size(); n++) {
                    squareType = ptypes[knightmoves[n].x][knightmoves[n].y];
                    if (legalSquare(pieceType, squareType, knightmoves[n])) legalMoves[i][j].push_back(knightmoves[n]);
                }

            }
            if (pieceType == "P") { // player pawn
                // can move up one or diagonally each time
                // can move up 2 if it is in it's original position (or just at j==1 for 'p' and j==6 for 'P')
                // en passant: pawn can take diagonally if opposing pawn is to it's left/right and has just moved up by 2
                if (j - 1 >= 0) {
                    squareType = ptypes[i][j - 1];
                    if (legalPawn(pieceType, squareType, Vector2i(i, j - 1), "f")) legalMoves[i][j].push_back(Vector2i(i, j - 1));

                    if (i - 1 >= 0) {
                        squareType = ptypes[i - 1][j - 1];
                        if (legalPawn(pieceType, squareType, Vector2i(i - 1, j - 1), "d")) legalMoves[i][j].push_back(Vector2i(i - 1, j - 1));
                    }
                    if (i + 1 < 8) {
                        squareType = ptypes[i + 1][j - 1];
                        if (legalPawn(pieceType, squareType, Vector2i(i + 1, j - 1), "d")) legalMoves[i][j].push_back(Vector2i(i + 1, j - 1));
                    }
                }
                if (j == 6) { // pawn in initial position and can move 2 forward
                    squareType = ptypes[i][j - 2];
                    if (legalPawn(pieceType, squareType, Vector2i(i, j - 2), "ff")) legalMoves[i][j].push_back(Vector2i(i, j - 2));
                }
            }
            if (pieceType == "p"){ // opponent pawn
                if (j + 1 < 8) {
                    squareType = ptypes[i][j+1];
                    if (legalPawn(pieceType, squareType, Vector2i(i,j+1),"f")) legalMoves[i][j].push_back(Vector2i(i, j + 1));

                    if (i - 1 >= 0) {
                        squareType = ptypes[i-1][j + 1];
                        if (legalPawn(pieceType, squareType, Vector2i(i-1, j + 1), "d")) legalMoves[i][j].push_back(Vector2i(i-1, j + 1));
                    }
                    if (i + 1 < 8) {
                        squareType = ptypes[i + 1][j + 1];
                        if (legalPawn(pieceType, squareType, Vector2i(i + 1, j + 1),"d")) legalMoves[i][j].push_back(Vector2i(i + 1, j + 1));
                    }
                }
                if (j==1) { // pawn in initial position and can move 2 forward
                    squareType = ptypes[i][j + 2];
                    if (legalPawn(pieceType, squareType, Vector2i(i, j + 2), "ff")) legalMoves[i][j].push_back(Vector2i(i, j + 2));
                }
            }
            if (pieceType == "K" || pieceType == "k") { // king

            }

        }
    }
}

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
    for (int i = 0; i < 32; i++) p[i].setTexture(pieces);
    loadPos((float)sw, (float)sh, psize, scaled);



    bool moving = 0; // is piece being dragged
    Sprite* pmoving = 0; // address of piece being moved
    //float offx = 0, offy = 0;

    while (window.isOpen())
    {
        Vector2i mouse = Mouse::getPosition(window);
        Vector2i mousePos = Vector2i(window.mapPixelToCoords(mouse)); // fixes mouse position when rescaling window
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) {
                window.close();
            }

            // event of mouse button being pressed
            if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left && moving == 0)
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
            if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left && (pmoving != 0)) {
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

                // legal moves table
                updateLegalMoves();
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        for (int k = 0; k < legalMoves[i][j].size(); k++) {
                            std::cout << ptypes[i][j]<< "(" << legalMoves[i][j][k].x << ", " << legalMoves[i][j][k].y << ")" << defended[i][j] << std::endl;
                        }
                        std::cout << std::endl;
                    }
                }
                
            }
        }

        if (moving) { // piece follows cursor if being dragged
            // use offx or offy to drag and keep relative position of cursor to piece
            //piece->setPosition(mousePos.x-offx, mousePos.y-offy);
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