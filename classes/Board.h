#pragma once
#include <array>
#include "bitops.h"
#include "ChessVals.h"


class Board{
public:
    uint64_t get(Color c, ChessPiece p) const{
        return pieces[c][p-1];
    }

    bool move(Color c, ChessPiece p, uint8_t from, uint8_t to){
        setBitInPlace(pieces[c][p-1], from, 0);
        setBitInPlace(pieces[c][p-1], to, 1);
    }

    static uint8_t idx(uint8_t x, uint8_t y){
        
    }

    static std::pair<uint8_t, uint8_t> cords(int idx){
        
    }

private:
    std::array<std::array<uint64_t, 6>, 2> pieces;
    uint64_t occupancy;
    uint64_t whites;
    uint64_t blacks;

    uint64_t makeMovesPawnWhite(uint8_t from){
        uint64_t movelist = 0;

        //forward
        movelist |= ( (~occupancy) & setBit(0, from-8, 1));
        //double forward
        movelist |= ( (~occupancy) & setBit(0, from-16, 1)) & ( 47 < from && from < 56 && movelist);

        //capture left
        movelist |= ( (blacks) & setBit(0, from-9, 1));

        //capture right
        movelist |= ( (blacks) & setBit(0, from-7, 1));
    }


};