#include "FileHandler.hpp"
#include <iostream>
using namespace std;

//Constructor
FileHandler::FileHandler(string filename) {
    file_name = filename;
    piece_table_handler = new PieceTableHandler();
    size = 0;
}

//private methods
// method to compute LPS
void FileHandler::prepareLPS(vector<int> &lps,string pattern){
    int i = 0;
    int j = 1;
    lps[i] = 0;
    while(j<pattern.length()){
        if(pattern[i] == pattern[j])
            lps[j++] = i++ + 1;
        else{
            if(i == 0) j++;
            else i = lps[i-1];
        }
    }
}

void FileHandler::KMP(string text,string pattern,vector<int>& index){
    vector<int> lps(pattern.size());
    prepareLPS(lps,pattern);

    int i = 0;
    int j = 0;

    while(i<text.length()){
        if(text[i] == pattern[j]){
            i++;
            j++;
        }
        else{
            if(j == 0) i++;
            else j = lps[j-1];
        }
        if(j == pattern.length()){

            index.push_back(i-j);
            j = 0;
        }
    }
    if(i == text.length() && j == pattern.length())
        index.push_back(i-j);
    return;
}

//public methods
// method to get total lines in a file
int FileHandler::getNumberOfLines() {
    return piece_table_handler->size;
}

// method to get size of the file
int FileHandler::getSize() {
    int total_line = piece_table_handler->size;
    int size = 0;
    for (int i = 1; i <= total_line; i++) {
        size += piece_table_handler->getTableSize(i);
    }
    return size;
}

// method to get size of a specifi line in a file
int FileHandler::getLineSize(int line) {
    return piece_table_handler->getTableSize(line);
}


bool FileHandler::openFileInReadMode() {
    if (file.is_open()) file.close();
    file.open(file_name, ios::in);
    if (!file) {
        cerr << "File does not exist" << endl;
        return false;
    }
    return true;
}

bool FileHandler::openFileInWriteMode() {
    if (file.is_open()) file.close();
    file.open(file_name, ios::out);
    if(file) return true;
    cerr << "Failed to create file"<<endl;
    return false;
}

bool FileHandler::openFileInAppendMode() {
    if (file.is_open()) file.close();
    file.open(file_name, ios::app);
    if (!file) {
        cerr << "File does not exist" << endl;
        return false;
    }
    return true;
}

void FileHandler::closeFile() {
    if (file.is_open()) file.close();
}

void FileHandler::readFile() {
    char character_value;
    string result;
    int line = 1;

    while (file.get(character_value) && character_value != EOF) {
        if (character_value == '\n') {
            piece_table_handler->insertTable(line++, result);
            result = "";
        }
        else
            result.push_back(character_value);
    }
    if (!result.empty()) piece_table_handler->insertTable(line, result);
}

void FileHandler::writeToFile() {
    string text;
    for (int i = 1; i <= piece_table_handler->size; i++) {
        text = piece_table_handler->getText(i);
        file << text << endl;
    }
}

//method to print the file content present in buffer
void FileHandler::printBuffer() {
    for (int i = 1; i <= piece_table_handler->size; i++) {
        cout << piece_table_handler->getText(i) << endl;
    }
}

string FileHandler::readLine(int line) {
    return piece_table_handler->getText(line);
}

void FileHandler::insertLine(int line, string data) {
    piece_table_handler->insertTable(line, data);
}

void FileHandler::updateLine(int line, string data) {
    piece_table_handler->replaceText(line, 0, piece_table_handler->getTableSize(line) - 1, data);
}

void FileHandler::removeLine(int line) {
    piece_table_handler->removeTable(line);
}

void FileHandler::modifyLine(int line, int start, int end, string data) {
    piece_table_handler->replaceText(line, start - 1, end - 1, data);
}

void FileHandler::insertText(int line, int cursor, string word) {
    piece_table_handler->insertText(line, cursor - 1, word);
}

void FileHandler::searchReplace(string search,string replace){
    for(int i=1;i<=getNumberOfLines();i++){
        vector<int> index;
        KMP(piece_table_handler->getText(i),search,index);
        for(int j=0;j<index.size();j++)
            piece_table_handler->replaceText(i,index[j],index[j]+search.length()-1,replace);
    }
}

void FileHandler::reclaimMemory() {
    piece_table_handler->reclaimMemory();
}
