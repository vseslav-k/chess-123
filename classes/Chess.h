#pragma once

#include "Game.h"
#include "Grid.h"
#include "ChessVals.h"
#include "bitops.h"
#include <array>

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

    static constexpr std::array<char, 7> WPIECES = { '0', 'P','N','B','R','Q','K' };
    static constexpr std::array<char, 7> BPIECES = { '0', 'p','n','b','r','q','k' };

    Bit* PieceForPlayer(const int playerNumber, ChessPiece piece);
    Player* ownerAt(int x, int y) const;
    void FENtoBoard(const std::string& fen);
    char pieceNotation(int x, int y) const;


    
    static ChessPiece charToPiece(const char p);
    static char pieceToChar(const ChessPiece p);

    static int cordsToIdx(std::pair<uint8_t, uint8_t> cords);
    static std::pair<uint8_t, uint8_t> idxToCords(uint8_t idx);

    
    Grid* _grid;
};