#pragma once
#include <cstdint>
#include <array>

inline constexpr std::array<uint64_t, 16> makeUtilBoards(){
    std::array<uint64_t, 16> utilBoards = {};

    utilBoards[0] = 0b1111111100000000000000000000000000000000000000000000000000000000;

    for(int i = 1; i < 8; ++i){
        utilBoards[i] = utilBoards[i-1] >> 8;
    }
    utilBoards[8] = 0b1000000010000000100000001000000010000000100000001000000010000000;
    for(int i = 9; i < 16; ++i){
        utilBoards[i] = utilBoards[i-1] >> 1;
    }



    return utilBoards;
}

enum UtilBoardsIdx: uint8_t{
    Row7=0, Row6=1, Row5=2, Row4=3, Row3=4, Row2=5, Row1=6, Row0=7,
    Col0=8, Col1=9, Col2=10, Col3=11, Col4=12, Col5=13, Col6=14, Col7=15
};

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

inline Color operator!(Color c){
    return static_cast<Color>(!static_cast<bool>(c));
}



struct PieceIdentity{
    Color color;
    ChessPiece piece;

    PieceIdentity(Color c = White, ChessPiece p = NoPiece): color{c}, piece{p}{}
    PieceIdentity(uint8_t c, uint8_t p): color{static_cast<Color>(c)}, piece{static_cast<Color>(p)}{}
};


inline constexpr std::array<uint64_t, 16> UTIL_BOARDS = makeUtilBoards();

inline constexpr std::array<char, 7> WPIECES = { '0', 'P','N','B','R','Q','K' };
inline constexpr std::array<char, 7> BPIECES = { '0', 'p','n','b','r','q','k' };