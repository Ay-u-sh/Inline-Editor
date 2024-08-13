#include "PieceTableHandler.hpp"
#include <iostream>
using namespace std;

// constructors
PieceTableHandler::PieceTableHandler() {
    head = NULL;
    tail = NULL;
    size = 0;
}

PieceTableHandler::PieceTableHandler(string data) {
    head = new PieceTable(data);
    tail = head;
    size = 1;
}


//private methods
//method to change the current line
bool PieceTableHandler::navigateToLine(int line, PieceTable** currTable) {
    if (line <= 0 || line>size) {
        cerr << "Line does not exist" << endl;
        return false;
    }
    
    while (--line && *currTable)
        *currTable = (*currTable)->next;

    if (!(*currTable)) {
        cerr << "Line does not exist" << endl;
        return false;
    }

    return true;
}

void PieceTableHandler::insertAtHead(PieceTable* newTable) {
    if(!head){
        head = newTable;
        tail = head;
    }
    else{
        newTable->next = head;
        head = newTable;
    }
    size += 1;
}

void PieceTableHandler::insertAtEnd(PieceTable* newTable) {
    if(!tail){
        tail = newTable;
        head = tail;
    }
    else {
        tail->next = newTable;
        tail = tail->next;
    }
    size += 1;
}

void PieceTableHandler::removeFromHead() {
    if(!head) return;

    PieceTable* todelete = head;
    head = head->next;

    if(tail->next == head) tail = tail->next;

    todelete->next = NULL;
    todelete->reclaimMemory();
    delete todelete;
    // head tail locha after delete
    size -= 1;
}


//public methods
// method to insert a new line
void PieceTableHandler::insertTable(int line, string data) {
    if(line <= 0) {
        cerr << "Line does not exist" << endl;
        return;
    }
    PieceTable* newTable = new PieceTable(data);
    if(line == 1 || !head)
        insertAtHead(newTable);
    else if(line == size + 1)
        insertAtEnd(newTable);
    else {
        PieceTable* currTable = head;
        line -= 1;

        while (line - 1 && currTable) {
            currTable = currTable->next;
            line--;
        }

        if(!currTable)
            insertAtEnd(newTable);
        else{
            newTable->next = currTable->next;
            currTable->next = newTable;

            if(currTable == tail) tail = tail->next;
            size += 1;
        }
    }
}

//method to remove a line
void PieceTableHandler::removeTable(int line) {
    if (line <= 0 || line > size)
        cerr << "Line does not exist" << endl;
    else if(line == 1) 
        removeFromHead();
    else{
        PieceTable* currTable = head;
        line -= 1;
        while (line - 1 && currTable) {
            currTable = currTable->next;
            line--;
        }
        if(!currTable) return;
        PieceTable* todelete = currTable->next;
        currTable->next = todelete->next;
        todelete->next = NULL;

        todelete->reclaimMemory();

        if (todelete == tail) tail = currTable;
        delete todelete;

        size -= 1;
    }
}

void PieceTableHandler::insertText(int line, int startidx, string data) {
    PieceTable* currTable = head;
    if(!navigateToLine(line, &currTable)) return;

    currTable->insert(startidx, data);
}

void PieceTableHandler::removeText(int line, int startidx, int endidx) {
    PieceTable* currTable = head;
    if(!navigateToLine(line, &currTable)) return;

    currTable->remove(startidx, endidx);
}

void PieceTableHandler::replaceText(int line, int startidx, int endidx, string data) {
    PieceTable* currTable = head;
    if(!navigateToLine(line, &currTable)) return;

    currTable->replace(startidx, endidx, data);
}

string PieceTableHandler::getText(int line) {
    PieceTable* currTable = head;
    if(!navigateToLine(line, &currTable)) return "";

    string text;
    currTable->getString(text);
    return text;
}

// method to get size of the current line
int PieceTableHandler::getTableSize(int line) {
    PieceTable* currTable = head;
    if(!navigateToLine(line, &currTable)) return 0;

    return currTable->size;
}

void PieceTableHandler::reclaimMemory() {
    while(head) {
        PieceTable* todelete = head;
        head = head->next;
        todelete->next = NULL;
        todelete->reclaimMemory();
        delete todelete;
    }
}