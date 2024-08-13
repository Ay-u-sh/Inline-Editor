#ifndef PIECETABLE_HPP
#define PIECETABLE_HPP
#include "PieceNode.hpp"
#include<iostream>

class PieceTable{
    private:
        PieceNode* head;
        PieceNode* tail;
        std::string original;
        std::string buffer;

        void insertAtHead(PieceNode* newnode);
        void collectGarbage();
        void spiltNode(PieceNode*,int);

    public:
        int size;
        PieceTable* next;

        PieceTable(std::string);

        void insert(int,std::string);
        void remove(int,int);
        void replace(int,int,std::string);

        void getString(std::string &);

        void reclaimMemory();
};

#endif