Chess engine includes the following so far:

Pawn movement, including double pushes and en passant. Generated via bitshift operations
Knight and king movement generated via bitshit operations
Casteling
Promotion
Full Fen support

Custom board class to keep all my bitboards organized and easy to manipulate

sliding piece movement via hyperbola quintessense

negamax ai with depth 4


Filter illegal moves works on bitboard level
Engine lets players make only legal moves
because filter illegal moves is so slow, my negamax implementation does not use it. Instead it just checks if a given state is illegal(king in check and not kings turn) and immediately returns MATE for such state. In

Most of my logic happens in the board class
