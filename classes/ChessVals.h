#pragma once
#include <cstdint>
#include <array>
#include "C:\Libraries\imgui\logger\logger.h"


enum UtilBoardsIdx: uint8_t{
    Row7=0, Row6=1, Row5=2, Row4=3, Row3=4, Row2=5, Row1=6, Row0=7,
    Col0=8, Col1=9, Col2=10, Col3=11, Col4=12, Col5=13, Col6=14, Col7=15,
    DiaI0=16,DiaI1=17,DiaI2=18,DiaI3=19,DiaI4=20,DiaI5=21,DiaI6=22,DiaI7=23,DiaI8=24,DiaI9=25,DiaI10=26,DiaI11=27,DiaI12=28,DiaI13=29,DiaI14=30,
    DiaD0=31,DiaD1=32,DiaD2=33,DiaD3=34,DiaD4=35,DiaD5=36,DiaD6=37,DiaD7=38,DiaD8=39,DiaD9=40,DiaD10=41,DiaD11=42,DiaD12=43,DiaD13=44,DiaD14=45,
};

inline UtilBoardsIdx operator-(UtilBoardsIdx b, int x){
    return static_cast<UtilBoardsIdx>(static_cast<uint8_t>(b)-x);
}
inline UtilBoardsIdx operator+(UtilBoardsIdx b, int x){
    return static_cast<UtilBoardsIdx>(static_cast<uint8_t>(b)+x);
}

inline constexpr std::array<uint64_t, 46> makeUtilBoards(){
    std::array<uint64_t, 46> utilBoards = {};

    utilBoards[0] = 0b1111111100000000000000000000000000000000000000000000000000000000;

    for(int i = 1; i < 8; ++i){
        utilBoards[i] = utilBoards[i-1] >> 8;
    }
    utilBoards[8] = 0b1000000010000000100000001000000010000000100000001000000010000000;
    for(int i = 9; i < 16; ++i){
        utilBoards[i] = utilBoards[i-1] >> 1;
    }

    utilBoards[16] = 0b1000000000000000000000000000000000000000000000000000000000000000ULL;
    utilBoards[17] = 0b0100000010000000000000000000000000000000000000000000000000000000ULL;
    utilBoards[18] = 0b0010000001000000100000000000000000000000000000000000000000000000ULL;
    utilBoards[19] = 0b0001000000100000010000001000000000000000000000000000000000000000ULL;
    utilBoards[20] = 0b0000100000010000001000000100000010000000000000000000000000000000ULL;
    utilBoards[21] = 0b0000010000001000000100000010000001000000100000000000000000000000ULL;
    utilBoards[22] = 0b0000001000000100000010000001000000100000010000001000000000000000ULL;
    utilBoards[23] = 0b0000000100000010000001000000100000010000001000000100000010000000ULL;
    utilBoards[24] = 0b0000000000000001000000100000010000001000000100000010000001000000ULL;
    utilBoards[25] = 0b0000000000000000000000010000001000000100000010000001000000100000ULL;
    utilBoards[26] = 0b0000000000000000000000000000000100000010000001000000100000010000ULL;
    utilBoards[27] = 0b0000000000000000000000000000000000000001000000100000010000001000ULL;
    utilBoards[28] = 0b0000000000000000000000000000000000000000000000010000001000000100ULL;
    utilBoards[29] = 0b0000000000000000000000000000000000000000000000000000000100000010ULL;
    utilBoards[30] = 0b0000000000000000000000000000000000000000000000000000000000000001ULL;

    utilBoards[31] = 0b0000000000000000000000000000000000000000000000000000000010000000ULL;
    utilBoards[32] = 0b0000000000000000000000000000000000000000000000001000000001000000ULL;
    utilBoards[33] = 0b0000000000000000000000000000000000000000100000000100000000100000ULL;
    utilBoards[34] = 0b0000000000000000000000000000000010000000010000000010000000010000ULL;
    utilBoards[35] = 0b0000000000000000000000001000000001000000001000000001000000001000ULL;
    utilBoards[36] = 0b0000000000000000100000000100000000100000000100000000100000000100ULL;
    utilBoards[37] = 0b0000000010000000010000000010000000010000000010000000010000000010ULL;
    utilBoards[38] = 0b1000000001000000001000000001000000001000000001000000001000000001ULL;
    utilBoards[39] = 0b0100000000100000000100000000100000000100000000100000000100000000ULL;
    utilBoards[40] = 0b0010000000010000000010000000010000000010000000010000000000000000ULL;
    utilBoards[41] = 0b0001000000001000000001000000001000000001000000000000000000000000ULL;
    utilBoards[42] = 0b0000100000000100000000100000000100000000000000000000000000000000ULL;
    utilBoards[43] = 0b0000010000000010000000010000000000000000000000000000000000000000ULL;
    utilBoards[44] = 0b0000001000000001000000000000000000000000000000000000000000000000ULL;
    utilBoards[45] = 0b0000000100000000000000000000000000000000000000000000000000000000ULL;



    return utilBoards;
}



enum ChessPiece: uint8_t
{
    NoPiece,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum Color: bool{
    White = 0,
    Black = 1
};

enum MoveResults: uint8_t{
    Illegal,
    Legal,
    EnPassant,
    CastleWL,
    CastleWR,
    CastleBL,
    CastleBR,
    Promotion,
    Check,
    Mate
};

inline Color operator!(Color c){
    return static_cast<Color>(!static_cast<bool>(c));
}


struct PieceIdentity{
    Color color;
    ChessPiece piece;

    PieceIdentity(Color c = White, ChessPiece p = NoPiece): color{c}, piece{p}{}
    PieceIdentity(uint8_t c, uint8_t p): color{static_cast<Color>(c)}, piece{static_cast<ChessPiece>(p)}{}
};


inline constexpr std::array<uint64_t, 46> UTIL_BOARDS = makeUtilBoards();


inline uint64_t getDiaI(uint8_t idx) {
    uint8_t row = idx >> 3;   // 0..7 (rank 8..1)
    uint8_t col = idx & 7;    // 0..7 (file a..h)
    uint8_t anti = uint8_t(row + col);        // 0..14  ('/')
    return UTIL_BOARDS[static_cast<uint8_t>(DiaI0) + anti];
}

inline uint64_t getDiaD( uint8_t idx) {
    uint8_t row = idx >> 3;
    uint8_t col = idx & 7;
    uint8_t diag = uint8_t(col + (7 - row));  // 0..14  ('\')
    return UTIL_BOARDS[static_cast<uint8_t>(DiaD0) + diag];
}

inline constexpr std::array<char, 7> WPIECES = { '0', 'P','N','B','R','Q','K' };
inline constexpr std::array<char, 7> BPIECES = { '0', 'p','n','b','r','q','k' };



inline ChessPiece charToPieceFunc(const char p){
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