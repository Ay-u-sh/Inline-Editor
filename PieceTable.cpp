#include "PieceTable.hpp"
#include<iostream>
using namespace std;

// constructor
PieceTable::PieceTable(string data){
    head = NULL;
    tail = NULL;
    next = NULL;
    
    size = data.length();
    original = data;

    PieceNode* newnode = new PieceNode(0,data.length());
    insertAtHead(newnode);
    tail = head;
} 

// private methods
void PieceTable::insertAtHead(PieceNode* newnode){
    if(head == NULL){
        head = newnode;
        return;
    }
    newnode->next = head;
    head = newnode;
    return;
}

// method to reclaim memory
void PieceTable::collectGarbage(){
    if(!head) return;
    
    PieceNode* prevnode = head;
    PieceNode* currnode = head->next;
    while(currnode){
        PieceNode* todelete = NULL;
        if(currnode->length == 0){
            todelete = currnode;
            prevnode->next = currnode->next;
        }
        if(todelete){
            todelete->next = NULL;
            delete todelete;//tail + segmentation fault
        }
        else prevnode = currnode;
        currnode = currnode->next;
    }
    if(head->length == 0){
        PieceNode* todelete = head;
        head = head->next;
        delete todelete;
    }
    return;
}

// method for splitting current node if the index at which an operation is to be performed lies between the range of start and start+length of piece table
void PieceTable::spiltNode(PieceNode* currnode,int newnode_length){
    if(!currnode) return;
    if(newnode_length == 0) newnode_length = 1;
    currnode->length -= newnode_length;
    PieceNode* newnode = new PieceNode(currnode->start+currnode->length,newnode_length,currnode->isOriginal);
    newnode->next = currnode->next;
    currnode->next = newnode;
}


// public methods
// method of inserting a text or Piece Node in a line or piece table
void PieceTable::insert(int startidx,string data){
    if(size == startidx){
        PieceNode* newnode = new PieceNode(buffer.length(),data.length(),false);
        tail->next = newnode;
        tail = tail->next;
        buffer += data;
        size += data.length();
    }
    else {
        size += 1;
        replace(startidx,startidx,data);
    }
}

void PieceTable::replace(int startidx,int endidx,string data){
    if(startidx>endidx) return;

    int curridx = 0;
    PieceNode* currnode = head;
    PieceNode* newnode = new PieceNode(buffer.length(),data.length(),false);
    
    buffer += data;
    size += data.length()-(endidx-startidx)-1;

    while(currnode){
        curridx += currnode->length;
        if(curridx>startidx) break;
        currnode = currnode->next;
    }

    if(curridx>startidx){
        int newnode_length = curridx-startidx;
        spiltNode(currnode,newnode_length);
        if(startidx == endidx && data.empty()) currnode->next->start += 1;
    }

    curridx = startidx;
    newnode->next = currnode->next;
    currnode->next = newnode;
    currnode = newnode->next;

    while(currnode){
        curridx += currnode->length;
        if(curridx>endidx) break;
        currnode->length = 0;
        currnode = currnode->next;
    }
    if(curridx - currnode->length < endidx){
        int newnode_length = curridx-endidx-1;
        spiltNode(currnode,newnode_length);
        currnode->length = 0;
    }

    while(tail->next) tail = tail->next;
    collectGarbage();
}

void PieceTable::remove(int startidx,int endidx){
    replace(startidx,endidx,"");
}

// method to get resultant string
void PieceTable::getString(string &str){
    if(head == NULL) return;

    string result(size,'\0');
    int result_idx = 0;
    PieceNode* currnode = head;
    
    while(currnode){
        if(currnode->isOriginal){
            for(int i=currnode->start;i<currnode->start+currnode->length;i++)
                result[result_idx++] = original[i];
        }
        else{
            for(int i=currnode->start;i<currnode->start+currnode->length;i++)
                result[result_idx++] = buffer[i];
        }
        currnode = currnode->next;
    }
    str = result;
}

void PieceTable::reclaimMemory(){
    PieceNode* currnode = head;
    while(currnode){
        currnode->length = 0;
        currnode = currnode->next;
    }
    collectGarbage();
}