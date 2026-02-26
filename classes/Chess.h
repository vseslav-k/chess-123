#pragma once

#include "Game.h"
#include "Grid.h"
#include "ChessVals.h"
#include "Board.h"
#include "bitops.h"
#include <array>

#include "C:\Libraries\imgui\logger\logger.h"
#include "C:\Libraries\imgui\Timer\Timer.h"

constexpr int pieceSize = 80;



class Chess final: public Game
{
public:
    Chess();
    ~Chess();

    void setUpBoard() override;

    bool canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    bool actionForEmptyHolder(BitHolder &holder) override;
    void bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    void stopGame() override;

    Player *checkForWinner() override;
    bool checkForDraw() override;

    std::string initialStateString() override;
    std::string stateString() override;
    void setStateString(const std::string &s) override;

    Grid* getGrid() override { return _grid; }

private:


    PieceIdentity getPieceIdentity(Bit *bit);

    PieceIdentity getPieceIdentity(Bit &bit) {return getPieceIdentity(&bit);}
    uint8_t getIdx(BitHolder *target);
    uint8_t getIdx(BitHolder &target){return getIdx( &target);}

    BitHolder *getHolder(uint8_t idx);

    Bit* PieceForPlayer(const int playerNumber, const ChessPiece piece);
    Player* ownerAt(int x, int y) const;
    void FENtoBoard(const std::string& fen);
    char pieceNotation(int x, int y) const;
    bool boardToGrid();

    void handleMoveResult(Bit &bit, BitHolder &src, BitHolder &dst, MoveResults r);


    
    static ChessPiece charToPiece(const char p);
    static char pieceToChar(const ChessPiece p);

    static int cordsToIdx(std::pair<uint8_t, uint8_t> cords);
    static std::pair<uint8_t, uint8_t> idxToCords(uint8_t idx);


    /* index cordinates

    00 01 02 03 04 05 06 07
    08 09 10 11 12 13 14 15
    16 17 18 19 20 21 22 23
    24 25 26 27 28 29 30 31
    32 33 34 35 36 37 38 39
    40 41 42 43 44 45 46 47
    48 49 50 51 52 53 54 55
    56 57 58 59 60 61 62 63
    */


    /*(x, y) cordinates

    07 17 27 37 47 57 67 77
    06 16 26 36 46 56 66 76
    05 15 25 35 45 55 65 75
    04 14 24 34 44 54 64 74
    03 13 23 33 43 53 63 73
    02 12 22 32 42 52 62 72
    01 11 21 31 41 51 61 71
    00 10 20 30 40 50 60 70
    */

    
    Grid* _grid;
    Board _board;
};