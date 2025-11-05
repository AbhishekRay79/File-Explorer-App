#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

// --------------------------------------------------
// Basic File Operations
// --------------------------------------------------

void listFiles(const string &path) {
    cout << "\nCurrent Directory: " << fs::absolute(path) << "\n\n";
    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_directory())
            cout << "[DIR]  " << entry.path().filename().string() << endl;
        else
            cout << "       " << entry.path().filename().string() << endl;
    }
}

void copyFile(const string &source, const string &dest) {
    try {
        fs::copy(source, dest, fs::copy_options::overwrite_existing);
        cout << "File copied successfully.\n";
    } catch (fs::filesystem_error &e) {
        cout << "Copy failed: " << e.what() << endl;
    }
}

void moveFile(const string &source, const string &dest) {
    try {
        fs::rename(source, dest);
        cout << "File moved successfully.\n";
    } catch (fs::filesystem_error &e) {
        cout << "Move failed: " << e.what() << endl;
    }
}

void deleteFile(const string &target) {
    try {
        if (fs::remove(target))
            cout << "File deleted successfully.\n";
        else
            cout << "File not found.\n";
    } catch (fs::filesystem_error &e) {
        cout << "Delete failed: " << e.what() << endl;
    }
}

void createFile(const string &filename) {
    ofstream file(filename);
    if (file) {
        cout << "File created: " << filename << endl;
        file.close();
    } else {
        cout << "Failed to create file.\n";
    }
}

// --------------------------------------------------
// Main Program
// --------------------------------------------------

int main() {
    string currentPath = ".";
    string command;

    cout << "File Explorer - Basic Version\n";
    cout << "Commands:\n";
    cout << "  ls               -> list files\n";
    cout << "  cd <folder>      -> change directory\n";
    cout << "  cd ..            -> go back\n";
    cout << "  cp <src> <dest>  -> copy file\n";
    cout << "  mv <src> <dest>  -> move or rename file\n";
    cout << "  rm <file>        -> delete file\n";
    cout << "  touch <file>     -> create file\n";
    cout << "  exit             -> quit program\n\n";

    while (true) {
        cout << ">> ";
        cin >> command;

        if (command == "ls") {
            listFiles(currentPath);
        } 
        else if (command == "cd") {
            string folder;
            cin >> folder;
            fs::path newPath;

            if (folder == "..")
                newPath = fs::path(currentPath).parent_path();
            else
                newPath = fs::path(currentPath) / folder;

            if (fs::exists(newPath) && fs::is_directory(newPath)) {
                currentPath = newPath;
            } else {
                cout << "Directory not found.\n";
            }
        } 
        else if (command == "cp") {
            string src, dest;
            cin >> src >> dest;
            copyFile((fs::path(currentPath) / src).string(), (fs::path(currentPath) / dest).string());
        } 
        else if (command == "mv") {
            string src, dest;
            cin >> src >> dest;
            moveFile((fs::path(currentPath) / src).string(), (fs::path(currentPath) / dest).string());
        } 
        else if (command == "rm") {
            string file;
            cin >> file;
            deleteFile((fs::path(currentPath) / file).string());
        } 
        else if (command == "touch") {
            string filename;
            cin >> filename;
            createFile((fs::path(currentPath) / filename).string());
        } 
        else if (command == "exit") {
            cout << "Exiting File Explorer.\n";
            break;
        } 
        else {
            cout << "Unknown command.\n";
        }
    }

    return 0;
}
