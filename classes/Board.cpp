#include "Board.h"

PieceIdentity Board::determinePiece(uint8_t idx){

    for(uint8_t color = 0; color < 2; ++color){
        for(uint8_t piece = 1; piece < 7; ++piece){

            if(getBit(getBitBoard(static_cast<Color>(color), static_cast<ChessPiece>(piece)),idx)){
                return PieceIdentity(color, piece);
            }

        }
    }

    return PieceIdentity();
}

MoveResults Board::handleMoveResult(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx){
    //if moving pawn twice, record enpassant
    if(piece == Pawn){
        if(srcIdx + 16 == dstIdx){
            _enPassantIdx = srcIdx + 8;
            return EnPassant;
        }
        else if(srcIdx - 16 == dstIdx){
            _enPassantIdx = srcIdx -8;
            return EnPassant;
        }
        else{
            _enPassantIdx = 0;
        }
    }else{
        _enPassantIdx = 0;
    }

    //if moving king remove casteling
    if(piece == King){
        if(color == White){
            setBitInPlace(_castling, 0, 0);
            setBitInPlace(_castling, 1, 0);
        }
        if(color == Black){
            setBitInPlace(_castling, 0, 2);
            setBitInPlace(_castling, 1, 3);
        }
    }

    //if moving rook remove casteling on that side
    if(piece == Rook){
        if(color == White && srcIdx == 56){
            setBitInPlace(_castling, 0, 0);
        }
        if(color == White && srcIdx == 63){
            setBitInPlace(_castling, 1, 0);
        }

        if(color == Black && srcIdx == 0){
            setBitInPlace(_castling, 2, 0);
        }
        if(color == Black && srcIdx == 7){
            setBitInPlace(_castling, 3, 0);
        }
    }

    return Legal;
}



void Board::updateBitBoards(Color color, ChessPiece piece, uint64_t oldPiecePos, uint64_t newPiecePos){
    uint64_t & bitboard = accessBitBoard(color, piece) ;
    bitboard &= ~oldPiecePos;
    bitboard |= newPiecePos;

    uint64_t & coloredOcc =  accessOccupancy(color);
    coloredOcc &= ~oldPiecePos;
    coloredOcc |= newPiecePos;

    _occupied &= ~oldPiecePos;
    _occupied |= newPiecePos;

    _free = ~_occupied;
}

bool Board::pieceExists(Color color, ChessPiece piece, uint8_t idx){
    return getBit(getBitBoard(color, piece), idx);
}


bool Board::pieceExists(uint8_t idx){
    PieceIdentity identity = determinePiece(idx);
    return pieceExists(identity.color, identity.piece, idx);
}

MoveResults Board::movePiece(Color color, ChessPiece piece, uint8_t srcIdx, uint8_t dstIdx){
    uint64_t moves = getMoves(color, piece, srcIdx);
    uint64_t newPiecePos = setBit(0ULL, dstIdx, true);
    if(!canPieceMoveFromTo(moves, newPiecePos)) return Illegal;


    PieceIdentity dstPiece = determinePiece(dstIdx);
    uint8_t moveRes = Legal;

    if(color != dstPiece.color && dstPiece.piece != NoPiece) [[likely]]
        updateBitBoards(dstPiece.color , dstPiece.piece, newPiecePos, 0ULL);

    if(dstIdx == _enPassantIdx && piece == Pawn) [[unlikely]]{
        updateBitBoards(!color , Pawn, newPiecePos<<8, 0ULL);
        updateBitBoards(!color , Pawn, newPiecePos>>8, 0ULL);
        //log(Info, "Pawn BB:"+ numToStrBin(getBitBoard(!color, Pawn)));
        ++moveRes;
    }
        
    updateBitBoards(color, piece, setBit(0ULL, srcIdx, true), newPiecePos);
    
    handleMoveResult(color, piece, srcIdx, dstIdx);

    return static_cast<MoveResults>(moveRes);
}

MoveResults Board::movePiece(uint8_t srcIdx, uint8_t dstIdx){
    PieceIdentity identity = determinePiece(srcIdx);
    return movePiece(identity.color, identity.piece,srcIdx,dstIdx);
}

uint64_t Board::getMoves(Color color, ChessPiece piece, uint8_t idx){
    switch(color){
        case White:
            switch(piece){
                case Pawn:  return getMovesPawnWhite(idx);
                case Knight:return getMovesKnightWhite(idx);
                case Bishop:return getMovesBishopWhite(idx);
                case Rook:  return getMovesRookWhite(idx);
                case Queen: return getMovesQueenWhite(idx);
                case King:  return getMovesKingWhite(idx);
            }
        case Black:
            switch(piece){
                case Pawn:  return getMovesPawnBlack(idx);
                case Knight:return getMovesKnightBlack(idx);
                case Bishop:return getMovesBishopBlack(idx);
                case Rook:  return getMovesRookBlack(idx);
                case Queen: return getMovesQueenBlack(idx);
                case King:  return getMovesKingBlack(idx);
            }
    }
    log(Error, "getMoves("+numToStr(static_cast<int>(color))+", "+numToStr(static_cast<int>(piece))+", "+numToStr(idx)+") is invalid. No such piece found");
    return 0;
}

uint64_t Board::getMoves(uint8_t idx){
    PieceIdentity identity = determinePiece(idx);
    return getMoves(identity.color, identity.piece, idx);
}

bool Board::detectCheck(Color color){
    return false;
}

bool Board::detectMate(Color color){
    return false;
}




uint64_t Board::getMovesPawnWhite(uint8_t idx){
    uint64_t me = setBit(0ULL, idx, 1);
    uint64_t moves = 0;

    uint64_t forward = me << 8;
    uint64_t forward2 = me << 16;
    uint64_t left = me << 9;
    uint64_t right = me << 7;

    /*
    if(forward & _free)
        moves |= forward;
    */
    moves |= (forward & _free);
    /*
    if((forward & _free) && (forward2 & _free) && (me & UTIL_BOARDS[Row1]))
        moves |= forward2;
    */
    moves |= ((forward & _free) << 8) & (forward2 & _free) & ((me & UTIL_BOARDS[Row1]) << 16);
    /*
    if((left & _blacks) && !(left & Col7))
        moves |= left;
    */
    moves |= (left & (_blacks | setBit(0ULL, _enPassantIdx, 1)));   
    /*
    if((right & _blacks) && !(right & Col0))
        moves |= right;
    */
    moves |= (right & (_blacks | setBit(0ULL, _enPassantIdx, 1)));
    

    moves &= ~((idx % 8 == 0) * UTIL_BOARDS[Col7]);
    moves &= ~(((idx-7) % 8 == 0) * UTIL_BOARDS[Col0]);


    return moves;
}
uint64_t Board::getMovesKnightWhite(uint8_t idx){
    uint64_t me = setBit(0ULL, idx, 1);
    uint64_t moves = 0;


    moves |= (me << 15) | (me << 17) | (me << 6) | (me << 10);
    moves |= (me >> 15) | (me >> 17) | (me >> 6) | (me >> 10);

    /*
    if(idx % 8 == 0 || (idx-1) % 8 == 0){
        moves &= ~(Col6 | Col7);
    }
    */
    moves &= ~((UTIL_BOARDS[Col6] | UTIL_BOARDS[Col7]) * (idx % 8 == 0 || (idx-1) % 8 == 0));
    /*
    if((idx+1) % 8 == 0 || (idx+2) % 8 == 0){
        moves &= ~(Col0 | Col1);
    }
    */
    moves &= ~((UTIL_BOARDS[Col0] | UTIL_BOARDS[Col1]) * ((idx+1) % 8 == 0 || (idx+2) % 8 == 0));

    moves &= ~_whites;

    return moves;
}
uint64_t Board::getMovesBishopWhite(uint8_t idx){
    return UINT64_MAX;
}
uint64_t Board::getMovesRookWhite(uint8_t idx){
    return UINT64_MAX;
}
uint64_t Board::getMovesQueenWhite(uint8_t idx){
    return UINT64_MAX;
}
uint64_t Board::getMovesKingWhite(uint8_t idx){
    uint64_t me = setBit(0ULL, idx, 1);
    uint64_t moves = 0;

    uint64_t line = (me >> 8) | (me << 8) | (me);

    moves |= line;

    moves |= ((line>>1) & ~UTIL_BOARDS[Col0]);
    moves |= ((line<<1) & ~UTIL_BOARDS[Col7]);

    moves &= ~me;
    moves &= ~_whites;

    return moves;
}

uint64_t Board::getMovesPawnBlack(uint8_t idx){
    uint64_t me = setBit(0ULL, idx, 1);
    uint64_t moves = 0;

    uint64_t forward = me >> 8;
    uint64_t forward2 = me >> 16;
    uint64_t left = me >> 7;
    uint64_t right = me >> 9;

    /*
    if(forward & _free)
        moves |= forward;
    */
    moves |= (forward & _free);
    /*
    if((forward & _free) && (forward2 & _free) && (me & UTIL_BOARDS[Row6]))
        moves |= forward2;
    */
    moves |= ((forward & _free) >> 8) & (forward2 & _free) & ((me & UTIL_BOARDS[Row6]) >> 16);
    /*
    if((left & _whites) && !(left & Col7))
        moves |= left;
    */
    moves |= (right & (_whites | setBit(0ULL, _enPassantIdx, 1)));
    /*
    if((right & _whites) && !(right & Col0))
        moves |= right;
    */
    moves |= (left & (_whites | setBit(0ULL, _enPassantIdx, 1)));


    moves &= ~((idx % 8 == 0) * UTIL_BOARDS[Col7]);
    moves &= ~(((idx-7) % 8 == 0) * UTIL_BOARDS[Col0]);

    
    return moves;
}
uint64_t Board::getMovesKnightBlack(uint8_t idx){
    uint64_t me = setBit(0ULL, idx, 1);
    uint64_t moves = 0;


    moves |= (me << 15) | (me << 17) | (me << 6) | (me << 10);
    moves |= (me >> 15) | (me >> 17) | (me >> 6) | (me >> 10);

    /*
    if(idx % 8 == 0 || (idx-1) % 8 == 0){
        moves &= ~(Col6 | Col7);
    }
    */
    moves &= ~((UTIL_BOARDS[Col6] | UTIL_BOARDS[Col7]) * (idx % 8 == 0 || (idx-1) % 8 == 0));
    /*
    if((idx+1) % 8 == 0 || (idx+2) % 8 == 0){
        moves &= ~(Col0 | Col1);
    }
    */
    moves &= ~((UTIL_BOARDS[Col0] | UTIL_BOARDS[Col1]) * ((idx+1) % 8 == 0 || (idx+2) % 8 == 0));

    moves &= ~_blacks;

    return moves;
}
uint64_t Board::getMovesBishopBlack(uint8_t idx){
    return UINT64_MAX;
}
uint64_t Board::getMovesRookBlack(uint8_t idx){
    return UINT64_MAX;
}
uint64_t Board::getMovesQueenBlack(uint8_t idx){
    return UINT64_MAX;
}
uint64_t Board::getMovesKingBlack(uint8_t idx){
    uint64_t me = setBit(0ULL, idx, 1);
    uint64_t moves = 0;

    uint64_t line = (me >> 8) | (me << 8) | (me);

    moves |= line;

    moves |= ((line>>1) & ~UTIL_BOARDS[Col0]);
    moves |= ((line<<1) & ~UTIL_BOARDS[Col7]);

    moves &= ~me;
    moves &= ~_blacks;

    return moves;
}



std::string Board::toString(){

    std::string res = "";
    res.reserve(73);
    for(int i = 0; i < 64; ++i){
        if(getBit(getBitBoard(White, Pawn), i)) res +=      'P';
        if(getBit(getBitBoard(White, Knight), i)) res +=    'N';
        if(getBit(getBitBoard(White, Bishop), i)) res +=    'B';
        if(getBit(getBitBoard(White, Rook), i)) res +=      'R';
        if(getBit(getBitBoard(White, Queen), i)) res +=     'Q';
        if(getBit(getBitBoard(White, King), i)) res +=      'K';
        


        if(getBit(getBitBoard(Black, Pawn), i)) res +=      'p';
        if(getBit(getBitBoard(Black, Knight), i)) res +=    'n';
        if(getBit(getBitBoard(Black, Bishop), i)) res +=    'b';
        if(getBit(getBitBoard(Black, Rook), i)) res +=      'r';
        if(getBit(getBitBoard(Black, Queen), i)) res +=     'q';
        if(getBit(getBitBoard(Black, King), i)) res +=      'k';

        if(getBit(getOccupancyAll(), i) == 0) res +=        '#';
        if((i-7)%8 == 0) res +=                             '\n';
    }

    return res;
}
std::string Board::getFen(){
    //"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
    
    std::string simpleStr = toString();
    std::string res = "";

    int counter = 0;
    for(int i = 0; i < 71; ++i){
        if(simpleStr[i] == '#'){
            ++counter;
            continue;
        }
        if(simpleStr[i] == '\n'){
            res += counter > 0 ? numToStr(counter): "";
            counter = 0;
            res += "/";
            continue;
        }

        res += counter > 0 ? numToStr(counter): "";
        counter = 0;
        res += simpleStr[i];
    }

    return res;
}
void Board::buildFromFen(const std::string & fen){

}

Board::Board():  _moveCount(0), _enPassantIdx(0), _castling(0b11110000), _halfMoveCount(0), _currPlayer(White){
    _pieces[White][Pawn-1] =    0b0000000000000000000000000000000000000000000000001111111100000000;
    _pieces[White][Knight-1] =  0b0000000000000000000000000000000000000000000000000000000001000010;
    _pieces[White][Bishop-1] =  0b0000000000000000000000000000000000000000000000000000000000100100;
    _pieces[White][Rook-1] =    0b0000000000000000000000000000000000000000000000000000000010000001;
    _pieces[White][Queen-1] =   0b0000000000000000000000000000000000000000000000000000000000010000;
    _pieces[White][King-1] =    0b0000000000000000000000000000000000000000000000000000000000001000;

    _pieces[Black][Pawn-1] =    0b0000000011111111000000000000000000000000000000000000000000000000;
    _pieces[Black][Knight-1] =  0b0100001000000000000000000000000000000000000000000000000000000000;
    _pieces[Black][Bishop-1] =  0b0010010000000000000000000000000000000000000000000000000000000000;
    _pieces[Black][Rook-1] =    0b1000000100000000000000000000000000000000000000000000000000000000;
    _pieces[Black][Queen-1] =   0b0001000000000000000000000000000000000000000000000000000000000000;
    _pieces[Black][King-1] =    0b0000100000000000000000000000000000000000000000000000000000000000;

    _whites = _pieces[White][Pawn-1] | _pieces[White][Knight-1] | _pieces[White][Bishop-1] | _pieces[White][Rook-1] | _pieces[White][Queen-1] | _pieces[White][King-1];
    _blacks = _pieces[Black][Pawn-1] | _pieces[Black][Knight-1] | _pieces[Black][Bishop-1] | _pieces[Black][Rook-1] | _pieces[Black][Queen-1] | _pieces[Black][King-1];
    _occupied = _whites | _blacks;
    _free = ~_occupied;
}