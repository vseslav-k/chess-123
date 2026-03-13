#pragma once
#include <array>
#include <string>
#include "bitops.h"
#include "ChessVals.h"
#include <utility> 
#include <algorithm> 
#include "C:\Libraries\imgui\logger\logger.h"
struct ChessMove;
struct BoardBackup;

class Board{
public:


    uint64_t        getBitBoard(Color color, ChessPiece piece)  const{return _pieces[color][piece-1];}
    uint64_t        getBitBoard(PieceIdentity identity)  const{return getBitBoard(identity.color, identity.piece);}

    uint64_t        getOccupancy(Color color)  const{switch(color){case White: return _whites; case Black: return _blacks; default: return _whites;}}
    uint64_t        getOccupancyAll() const{return _occupied;}
    uint64_t        getFree() const{return _free;}

    bool            pieceExists(Color color, ChessPiece piece, uint8_t idx) const;
    bool            pieceExists(uint8_t idx) const;

    MoveResults     movePiece(ChessMove move);
    MoveResults     movePiece(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx);
    MoveResults     movePiece(PieceIdentity identity, uint8_t srcIdx, uint8_t dstIdx) {return movePiece(identity.color, identity.piece, srcIdx, dstIdx);}
    MoveResults     movePiece(uint8_t srcIdx, uint8_t dstIdx);


    BoardBackup makeBackup() const;
    void undoMove(ChessMove move, BoardBackup backup);

    uint64_t        getMoves(Color color, ChessPiece piece, uint8_t idx) const;
    uint64_t        getMoves(PieceIdentity identity, uint8_t idx) const{return getMoves(identity.color, identity.piece, idx);}
    uint64_t        getMoves(uint8_t idx) const;

    PieceIdentity   determinePiece(uint8_t idx) const;

    bool            detectMate(Color color, uint8_t idx);
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


    std::vector<ChessMove> getAllMoves(Color color);

    int eval(Color color);
    bool inMate(const Color col)const;
    bool inCheck(const Color col)const{return _inCheck[col];}

    bool kingCaptureAvailible(Color col) {return (inCheck(col) && (_currColor != col));}
    bool detectKingCapture(Color col){return (detectCheck(col) && (_currColor != col));}

    bool getIllegalState() {return (inCheck(White) && !_currColor) || (inCheck(Black) && _currColor);}
    bool detectIllegalState(){return (detectCheck(White) && !_currColor) || (detectCheck(Black) && _currColor);}




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

    std::array<bool, 2> _inCheck;



    uint64_t&        accessBitBoard(Color color, ChessPiece piece){return _pieces[color][piece-1];}
    uint64_t&        accessOccupancy(Color color)   {switch(color){case White: return _whites; case Black: return _blacks; default: return _whites;}}
    uint64_t&        accessOccupancyAll() {return _occupied;}

    void updateBitBoards(Color color, ChessPiece piece, uint64_t oldPiecePos, uint64_t newPiecePos);

    uint64_t getMovesPawnWhite(uint8_t idx) const;
    uint64_t getMovesKnightWhite(uint8_t idx) const;
    uint64_t getMovesBishopWhite(uint8_t idx) const;
    uint64_t getMovesRookWhite(uint8_t idx) const;
    uint64_t getMovesQueenWhite(uint8_t idx) const;
    uint64_t getMovesKingWhite(uint8_t idx) const;

    uint64_t getMovesPawnBlack(uint8_t idx) const;
    uint64_t getMovesKnightBlack(uint8_t idx) const;
    uint64_t getMovesBishopBlack(uint8_t idx) const;
    uint64_t getMovesRookBlack(uint8_t idx) const;
    uint64_t getMovesQueenBlack(uint8_t idx) const;
    uint64_t getMovesKingBlack(uint8_t idx) const;


    
    void handleMoveResult(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx);

    MoveResults handleSpecialMove(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx, uint64_t newPiecePos, uint64_t oldPiecePos);


};


struct ChessMove{
    Color color;
    ChessPiece piece;
    uint8_t src;
    uint8_t dst;
    ChessPiece capturePiece;

    ChessMove(Color c, ChessPiece p, uint8_t s, uint8_t d, ChessPiece cp): color{c}, piece{p}, src{s}, dst{d}, capturePiece{cp}{}
    


    bool operator>(ChessMove m)const{
        return static_cast<uint8_t>(capturePiece) > static_cast<uint8_t>(m.capturePiece);
    }
    bool operator<(ChessMove m)const{
        return static_cast<uint8_t>(capturePiece) < static_cast<uint8_t>(m.capturePiece);
    }
    bool operator==(ChessMove m)const{
        return static_cast<uint8_t>(capturePiece) == static_cast<uint8_t>(m.capturePiece);
    }

};


struct BoardBackup{
    uint8_t castling;
    uint8_t enPassant;
    uint8_t halfMoveCount;
    std::array<bool, 2> inCheck;

    BoardBackup(uint8_t castle, uint8_t en, uint8_t half, std::array<bool, 2> check): castling{castle}, enPassant{en}, halfMoveCount{half}, inCheck(check){}
};