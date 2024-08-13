#include "PieceNode.hpp"
#include <iostream>
using namespace std;

PieceNode::PieceNode(int startidx,int length,bool isOriginal){
    this->start = startidx;
    this->length = length;
    this->isOriginal = isOriginal;
    next = NULL;
}