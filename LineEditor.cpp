#include "FileHandler.hpp"
#include<iostream>
#include<vector>

#define MAX_CHARS_PER_LINE 80
using namespace std;

class LineEditor{
    private:
        vector<FileHandler*> file_handler;
        int current_file_index;

        void showWelcomeScreen(){
            cout << endl << "----------------------------------------" << endl;
            cout << "|      Line Editor Application         |" << endl;
            cout << "|            Instructions              |" << endl;
            cout << "----------------------------------------" << endl;
            cout << "\t\t\t\t  by Ayush Bhande"<< endl << endl;
            cout << "Enter a command:" << endl;
            cout << "1.  r  <filename> - read buffer" << endl;
            cout << "2.  cr <filename> - create a file" << endl;
            cout << "3.  sv <filename> - save a file" << endl;
            cout << "4.  a  <filename> <text> - append to a file" << endl;
            cout << "5.  rl <filename> <lineNumber> - read specific line" << endl;
            cout << "6.  d  <filename> <lineNumber> - delete a line with specified line number" << endl;
            cout << "7.  i  <filename> <lineNumber> <text> - insert a new line at specified line number with the text" << endl;
            cout << "8.  u  <filename> <lineNumber> <text> - update a line with specified text" << endl;
            cout << "9.  iw <filename> <lineNumber> <cursor position> <word> - insert word at specified index" << endl;
            cout << "10. sr <filename> <search word> <replace word> - search and replace word in the entire file" << endl;
            cout << "12. m  <filename> <lineNumber> <start position> <end position> <new content> - modify specific region in specified line in the file with new text" << endl;
            cout << "12. rw <filename> <lineNumber> <cursor position> - remove word at specified cursor position" << endl;
            cout << "13. q  - quit and save" << endl;
        }

    public:
        //constructors
        LineEditor(){
            current_file_index = -1;
        }

        LineEditor(string filename){
            loadFileIfNeeded(filename);
            current_file_index = 0;
        }

        //method to load a file in memory
        void loadFileIfNeeded(string filename){
            if(current_file_index >= 0 && filename == file_handler[current_file_index]->file_name) return;
            else if(current_file_index>=0){
                for(int i=0;i<file_handler.size();i++){
                    if(file_handler[i]->file_name == filename){
                        current_file_index = i;
                        return;
                    }
                }
            }
            else{
                FileHandler* newFile =  new FileHandler(filename);
                if(!newFile->openFileInReadMode()) delete newFile;
                else {
                    file_handler.push_back(newFile);
                    current_file_index = file_handler.size()-1;
                    file_handler.back()->readFile();
                }
            }
        }

        // method to add a new file to buffer
        void addNewFile(string filename){
            file_handler.push_back(new FileHandler(filename));
            file_handler.back()->openFileInWriteMode();
            current_file_index = file_handler.size()-1;
        }

        void saveFile(int index){
            file_handler[index]->openFileInWriteMode();
            file_handler[index]->writeToFile();
            file_handler[index]->closeFile();
        }

        //method to free memory
        void reclaimMemory(){
            for(int i=0;i<file_handler.size();i++){
                file_handler[i]->openFileInWriteMode();
                file_handler[i]->writeToFile();
                file_handler[i]->closeFile();

                file_handler[i]->reclaimMemory();
            }
        }

        void run() {
            string filename(100,'\0');
            string command(2,'\0');

            showWelcomeScreen();

            while (true) {
                cout << endl << "Line_Editor>> ";
                cin >> command;

                if (command == "q") {
                    for(int i=0;i<file_handler.size();i++)
                        saveFile(i);
                    
                    reclaimMemory();
                    cout << "Editor closed." << endl;
                    break;
                
                }

                cin.ignore();// ignoring space in input buffer
                cin >> filename;
                
                if(command != "cr")
                    loadFileIfNeeded(filename);
                
                if(command == "r") {
                    if(current_file_index>=0 && filename == file_handler[current_file_index]->file_name)
                        file_handler[current_file_index]->printBuffer();
                }
                
                else if (command == "rl") {
                    int line = -1;
                    cin.ignore();
                    cin >> line;
                    cout<<file_handler[current_file_index]->readLine(line) << endl;
                }
                
                else if (command == "cr") {
                    addNewFile(filename);
                    loadFileIfNeeded(filename);

                    int line;
                    string data;
                    cout << "Enter number of lines to be entered:";
                    cin >> line;
                    cin.ignore();

                    for(int i=1;i<=line;i++){
                        cout<<"[LINE:" << i <<"] ";
                        getline(cin,data);
                        file_handler[current_file_index]->insertLine(i,data);
                    }
                    cout << "File created " << endl;
                }
                else if(command == "sv"){
                    saveFile(current_file_index);
                    cout << "File saved" << endl;
                }
                else if (command == "a") {
                    string text;

                    cin.ignore();
                    getline(cin,text);
                    
                    int number_of_lines = file_handler[current_file_index]->getNumberOfLines();
                    int line_size = file_handler[current_file_index]->getLineSize(number_of_lines);

                    file_handler[current_file_index]->insertText(number_of_lines,line_size,text);

                    file_handler[current_file_index]->openFileInWriteMode();
                    file_handler[current_file_index]->writeToFile();
                    file_handler[current_file_index]->closeFile();
                }
                
                else if (command == "i") {
                    int line = -1;
                    string text;

                    cin >> line;
                    cin.ignore();
                    
                    getline(cin,text);
                    file_handler[current_file_index]->insertLine(line,text);
                } 
                
                else if (command == "u") {
                    int line = -1;
                    string text;

                    cin >> line;
                    cin.ignore();
                    getline(cin,text);
                    
                    file_handler[current_file_index]->updateLine(line,text);
                } 
                
                else if (command == "d") {
                    int line = -1;
                    cin >> line;
                    
                    file_handler[current_file_index]->removeLine(line);
                } 
                
                else if (command == "iw") {
                    int line = -1;
                    int cursor= -1;
                    string word;

                    cin >> line >> cursor;
                    cin.ignore();
                    getline(cin,word);
                    
                    file_handler[current_file_index]->insertText(line,cursor,word);
                } 
                
                else if (command == "sr") {
                    string search;
                    string replace;

                    cin.ignore();
                    cin >> search;
                    cin.ignore();
                    cin >> replace;

                    file_handler[current_file_index]->searchReplace(search,replace);
                } 
                
                else if (command == "m") {
                    int line = -1;
                    int start = -1;
                    int end = -1;
                    string content;
                    
                    cin >> line >> start >> end;
                    cin.ignore();
                    getline(cin,content);
                    
                    file_handler[current_file_index]->modifyLine(line,start,end,content);
                } 
                
                else if (command == "rw") {
                    int line = -1;
                    int start = -1;
                    int end = -1;
    
                    cin >> line >> start;

                    string current_line = file_handler[current_file_index]->readLine(line);
                    for(int i=start-1;i<current_line.length();i++){
                        if(current_line[i] == ' '){
                            end = i;
                            break;
                        }
                    }
        
                    file_handler[current_file_index]->modifyLine(line,start,end+1,"");
                }
                else {
                    cout << "Invalid command entered!" << endl;
                    cin.ignore();
                }
            }
    }
};

int main(){
    LineEditor le;
    le.run();
    return 0;
}