#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "PieceTableHandler.hpp"
#include <fstream>
#include <vector>

class FileHandler {
private:
    std::fstream file;
    PieceTableHandler* piece_table_handler;

    void prepareLPS(std::vector<int> &,std::string);
    void KMP(std::string,std::string,std::vector<int>&);

public:
    int size;
    std::string file_name;

    FileHandler(std::string);

    int getNumberOfLines();
    int getSize();
    int getLineSize(int);
    
    bool openFileInReadMode();
    bool openFileInWriteMode();
    bool openFileInAppendMode();
    void closeFile();
    void readFile();
    void writeToFile();
    void printBuffer();
    
    std::string readLine(int);
    void insertLine(int, std::string);
    void updateLine(int, std::string);
    void removeLine(int);
    void modifyLine(int, int, int, std::string);
    
    void insertText(int, int, std::string);
    void searchReplace(std::string,std::string);

    void reclaimMemory();
};

#endif
