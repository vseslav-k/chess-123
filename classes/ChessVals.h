#pragma once
#include <cstdint>
#include <array>

constexpr std::array<uint64_t, 16> makeUtilBoards(){
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
    Row8=0, Row7=1, Row6=2, Row5=3, Row4=4, Row3=5, Row2=6, Row1=7,
    Col1=8, Col2=9, Col3=10, Col4=11, Col5=12, Col6=13, Col7=14, Col8=15
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

constexpr std::array<uint64_t, 16> UTIL_BOARDS = makeUtilBoards();