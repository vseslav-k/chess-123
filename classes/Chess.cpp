#include "Chess.h"
#include <limits>
#include <cmath>

Chess::Chess(): _board()
{
    _grid = new Grid(8, 8);
}

Chess::~Chess()
{
    delete _grid;
}


PieceIdentity Chess::getPieceIdentity(Bit *bit){
    switch(bit->gameTag()){
        case 0: return PieceIdentity();
        case 1: return PieceIdentity(White, Pawn);
        case 2: return PieceIdentity(White, Knight);
        case 3: return PieceIdentity(White, Bishop);
        case 4: return PieceIdentity(White, Rook);
        case 5: return PieceIdentity(White, Queen);
        case 6: return PieceIdentity(White, King);
        case 7: return PieceIdentity();
        case 8: return PieceIdentity(Black, Pawn);
        case 9: return PieceIdentity(Black, Knight);
        case 10:return PieceIdentity(Black, Bishop);
        case 11:return PieceIdentity(Black, Rook);
        case 12:return PieceIdentity(Black, Queen);
        case 13:return PieceIdentity(Black, King);
        default:return PieceIdentity();
    }
}

uint8_t Chess::getIdx(BitHolder *target){
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if(static_cast<BitHolder*>(_grid->getSquare(i, j)) == target)
                return cordsToIdx({i,j});
}

ChessPiece Chess::charToPiece(const char p){
    switch((p >= 97? p - 32: p)){
        case '0':
            return NoPiece;
        case 'P':
            return Pawn;
        case 'N':
            return Knight;
        case 'B':
            return Bishop;
        case 'R':
            return Rook;
        case 'Q':
            return Queen;

        case 'K':
            return King;        
    }

    return NoPiece;
}
char Chess::pieceToChar(const ChessPiece p){
    return WPIECES[static_cast<size_t>(p)];
}


int Chess::cordsToIdx(std::pair<uint8_t, uint8_t> cords){
    return (56 - cords.second*8 + cords.first);
}
std::pair<uint8_t, uint8_t> Chess::idxToCords(uint8_t idx){
    return {idx%8, 7-idx/8};
}



char Chess::pieceNotation(int x, int y) const
{
    Bit *bit = _grid->getSquare(x, y)->bit();
    char notation = '0';
    if (bit) {
        notation = bit->gameTag() < 7 ? WPIECES[bit->gameTag()] : BPIECES[bit->gameTag()-7];
    }
    return notation;
}

BitHolder *Chess::getHolder(uint8_t idx){
    std::pair<uint8_t, uint8_t> cords = idxToCords(idx);

    ChessSquare* sqr = _grid->getSquare(cords.first, cords.second);

    return static_cast<BitHolder*>(sqr);
}

Bit* Chess::PieceForPlayer(const int playerNumber, const ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);
    bit->setGameTag(static_cast<int>(piece) + 7*playerNumber);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;

    _grid->initializeChessSquares(pieceSize, "boardsquare.png");
    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KkQq");

    bool playerMismatch = boardToGrid();
    startGame();

    if(playerMismatch) endTurn();
}

bool Chess::boardToGrid(){
    stopGame();

    for(Color c = White; true; c = !c){

        for(ChessPiece p = Pawn; true ; p = static_cast<ChessPiece>(p+1)){

            for(uint8_t i = 0; i < 64; ++i){
                if(getBit(_board.getBitBoard(c,p), i) == 1){

                    Bit* bit = PieceForPlayer(c, p);
                    BitHolder* holder = getHolder(i);
                    holder-> setBit(bit);
                    bit-> setPosition(holder->getPosition());
                }
            }


            if(p == King) break;
        }
    
        if(c==Black) break;
    }

    if(_board.getCurrColor() != static_cast<Color>(getCurrentPlayer()->playerNumber())){
        return true;
    }

     return false;

}



void Chess::FENtoBoard(const std::string& fen) {
    // convert a FEN string to a board
    // FEN is a space delimited string with 6 fields
    // 1: piece placement (from white's perspective)
    // NOT PART OF THIS ASSIGNMENT BUT OTHER THINGS THAT CAN BE IN A FEN STRING
    // ARE BELOW
    // 2: active color (W or B)
    // 3: castling availability (KQkq or -)
    // 4: en passant target square (in algebraic notation, or -)
    // 5: halfmove clock (number of halfmoves since the last capture or pawn advance)



    _board.buildFromFen(fen);


    /*
    int i = 0;

    for(char c : fen){
        if(c == '/') continue;
        if(c == ' ') break;

        if('1' <= c && c <= '8'){
            i += (c -'0');
            continue;
        }

        Bit* bit = PieceForPlayer(c < 97, charToPiece(c));
        BitHolder* holder = _grid->getSquareByIndex(i);

        holder-> setBit(bit);
        bit-> setPosition(holder->getPosition());

        ++i;
    }
    */
}

bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    log(Debug, _board.getFen());
    return false;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    return( (getCurrentPlayer()->playerNumber() == 0 && bit.gameTag() < 7) || (getCurrentPlayer()->playerNumber() == 1 && bit.gameTag() >= 7) );



}

bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    return Board::canPieceMoveFromTo(
        _board.getMoves(
            getPieceIdentity(bit), 
            getIdx(src)
        ),

        getIdx(dst)
    );
}


void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst){
    MoveResults res = _board.movePiece(getPieceIdentity(bit), getIdx(src), getIdx(dst));
    //log(Info, "MoveRes " + numToStr(static_cast<uint8_t>(res)));
    handleMoveResult(bit, src, dst, res);

    endTurn();
}


void Chess::handleMoveResult(Bit &bit, BitHolder &src, BitHolder &dst, MoveResults r){
    switch(r){
        case Illegal:{
            //log(Warn, "Illegal move attempted from: " + numToStr(getIdx(src)) + " to: " + numToStr(getIdx(dst)) + " bit: " + numToStr(bit.gameTag()));
            return;
        }
        case EnPassant:{
            //log(Info, "EnPassant in handleMoveResult");
            if(getIdx(src) > getIdx(dst)){
                getHolder(getIdx(dst)+8)->destroyBit();
            }
            if(getIdx(src) < getIdx(dst)){
                getHolder(getIdx(dst)-8)->destroyBit();
            }
            return;
        }
    }
}


void Chess::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Chess::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Chess::checkForWinner()
{
    return nullptr;
}

bool Chess::checkForDraw()
{
    return false;
}

std::string Chess::initialStateString()
{
    return stateString();
}

std::string Chess::stateString()
{
    std::string s;
    s.reserve(64);
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
            s += pieceNotation( x, y );
        }
    );
    return s;
}

void Chess::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 8 + x;
        char playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1, Pawn));
        } else {
            square->setBit(nullptr);
        }
    });
}
