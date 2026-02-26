#pragma once
#include <array>
#include <string>
#include "bitops.h"
#include "ChessVals.h"
#include "C:\Libraries\imgui\logger\logger.h"

class Board{
public:


    uint64_t        getBitBoard(Color color, ChessPiece piece)  {return _pieces[color][piece-1];}
    uint64_t        getBitBoard(PieceIdentity identity)  {return getBitBoard(identity.color, identity.piece);}

    uint64_t        getOccupancy(Color color)   {switch(color){case White: return _whites; case Black: return _blacks; default: return _whites;}}
    uint64_t        getOccupancyAll() {return _occupied;}
    uint64_t        getFree() {return _free;}

    bool            pieceExists(Color color, ChessPiece piece, uint8_t idx);
    bool            pieceExists(uint8_t idx);


    MoveResults     movePiece(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx);
    MoveResults     movePiece(PieceIdentity identity, uint8_t srcIdx, uint8_t dstIdx) {return movePiece(identity.color, identity.piece, srcIdx, dstIdx);}
    MoveResults     movePiece(uint8_t srcIdx, uint8_t dstIdx);

    uint64_t        getMoves(Color color, ChessPiece piece, uint8_t idx);
    uint64_t        getMoves(PieceIdentity identity, uint8_t idx) {return getMoves(identity.color, identity.piece, idx);}
    uint64_t        getMoves(uint8_t idx);

    PieceIdentity   determinePiece(uint8_t idx);

    bool            detectCheck(Color color);
    bool            detectMate(Color color);

    static bool     canPieceMoveFromTo(uint64_t moves, uint8_t dstIdx){return moves & setBit(0ULL, dstIdx, true);}
    static bool     canPieceMoveFromTo(uint64_t moves, uint64_t dst){return moves & dst;}

    static std::string idxToBoardCord(uint8_t idx);
    static uint8_t     boardCordToIdx(char l, char n);

    std::string toString();
    std::string getFen();
    void buildFromFen(const std::string & fen);

    
    Board();
    Board(const std::string & fen);

    Color getCurrColor(){return _currColor;}




private:

    std::array<std::array<uint64_t, 6>, 2> _pieces;
    
    uint64_t _occupied;
    uint64_t _free;
    uint64_t _whites;
    uint64_t _blacks;

    uint32_t _moveCount;
    uint8_t _enPassantIdx;
    uint8_t _castling;
    
    uint8_t _halfMoveCount;
    Color _currColor;



    uint64_t&        accessBitBoard(Color color, ChessPiece piece){return _pieces[color][piece-1];}
    uint64_t&        accessOccupancy(Color color)   {switch(color){case White: return _whites; case Black: return _blacks; default: return _whites;}}
    uint64_t&        accessOccupancyAll() {return _occupied;}

    void updateBitBoards(Color color, ChessPiece piece, uint64_t oldPiecePos, uint64_t newPiecePos);

    uint64_t getMovesPawnWhite(uint8_t idx);
    uint64_t getMovesKnightWhite(uint8_t idx);
    uint64_t getMovesBishopWhite(uint8_t idx);
    uint64_t getMovesRookWhite(uint8_t idx);
    uint64_t getMovesQueenWhite(uint8_t idx);
    uint64_t getMovesKingWhite(uint8_t idx);

    uint64_t getMovesPawnBlack(uint8_t idx);
    uint64_t getMovesKnightBlack(uint8_t idx);
    uint64_t getMovesBishopBlack(uint8_t idx);
    uint64_t getMovesRookBlack(uint8_t idx);
    uint64_t getMovesQueenBlack(uint8_t idx);
    uint64_t getMovesKingBlack(uint8_t idx);


    
    void handleMoveResult(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx);

    MoveResults handleSpecialMove(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx, uint64_t newPiecePos, uint64_t oldPiecePos);


};