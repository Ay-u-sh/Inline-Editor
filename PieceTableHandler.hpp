#ifndef PIECETABLEHANDLER_H
#define PIECETABLEHANDLER_H
#include "PieceTable.hpp"  // Assuming PieceTable class is defined in PieceTable.

class PieceTableHandler {
private:
    PieceTable* head;
    PieceTable* tail;

    bool navigateToLine(int, PieceTable**);

    void insertAtHead(PieceTable*);
    void insertAtEnd(PieceTable*);
    void removeFromHead();

public:
    int size;
    PieceTableHandler();
    PieceTableHandler(std::string);

    void insertTable(int,std::string);
    void removeTable(int line);

    void insertText(int,int,std::string);
    void removeText(int,int,int);
    void replaceText(int,int,int, std::string);
    std::string getText(int);
    
    int getTableSize(int);
    void reclaimMemory();
};

#endif
