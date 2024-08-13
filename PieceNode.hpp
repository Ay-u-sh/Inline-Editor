#ifndef PIECENODE_HPP
#define PIECENODE_HPP

class PieceNode {
public:
    bool isOriginal;
    int start;
    int length;
    PieceNode* next;
    PieceNode(int,int,bool = true);
};

#endif
