#include <iostream>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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

// --------------------------------------------------
// Directory Management
// --------------------------------------------------

void makeDirectory(const string &dirName) {
    try {
        if (fs::create_directory(dirName))
            cout << "Directory created: " << dirName << endl;
        else
            cout << "Failed to create directory (it may already exist).\n";
    } catch (fs::filesystem_error &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void removeDirectory(const string &dirName) {
    try {
        if (fs::remove(dirName))
            cout << "Directory removed: " << dirName << endl;
        else
            cout << "Failed to remove directory (it may not exist or is not empty).\n";
    } catch (fs::filesystem_error &e) {
        cout << "Error: " << e.what() << endl;
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
void searchFiles(const string &path, const string &keyword) {
    cout << "\nSearching for \"" << keyword << "\" in: " << fs::absolute(path) << "\n";
    bool found = false;

    try {
        for (const auto &entry : fs::recursive_directory_iterator(path)) {
            string name = entry.path().filename().string();
            if (name.find(keyword) != string::npos) {
                cout << (entry.is_directory() ? "[DIR]  " : "       ")
                     << fs::absolute(entry.path()).string() << endl;
                found = true;
            }
        }
    } catch (fs::filesystem_error &e) {
        cout << "Warning: " << e.what() << endl;
    }

    if (!found)
        cout << "No files or directories found matching \"" << keyword << "\".\n";
}

// --------------------------------------------------
// Permission Management
// --------------------------------------------------

void showPermissions(const string &filepath) {
    try {
        fs::perms p = fs::status(filepath).permissions();

        cout << "Permissions for " << filepath << ":\n";
        cout << "  Owner: "
             << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
             << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
             << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-") << endl;

        cout << "  Group: "
             << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
             << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
             << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-") << endl;

        cout << "  Others: "
             << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
             << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
             << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-") << endl;
    } catch (fs::filesystem_error &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void changePermissions(const string &filepath, const string &mode) {
    if (mode.size() != 3 || !isdigit(mode[0]) || !isdigit(mode[1]) || !isdigit(mode[2])) {
        cout << "Invalid mode. Use numeric format (e.g. 755, 644).\n";
        return;
    }

    int perms = stoi(mode, nullptr, 8);
    fs::perms newPerms = static_cast<fs::perms>(perms);

    try {
        fs::permissions(filepath, newPerms);
        cout << "Permissions changed successfully.\n";
    } catch (fs::filesystem_error &e) {
        cout << "Failed to change permissions: " << e.what() << endl;
    }
}
// --------------------------------------------------
// File size formatting and  sorting
// --------------------------------------------------
string formatSize(uintmax_t size) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unitIndex = 0;
    double displaySize = size;

    while (displaySize >= 1024 && unitIndex < 3) {
        displaySize /= 1024;
        unitIndex++;
    }

    char buffer[50];
    sprintf(buffer, "%.2f %s", displaySize, units[unitIndex]);
    return string(buffer);
}
void listFilesSortedBySize(const string &path, bool descending = false){
    vector<pair<fs::path, uintmax_t>> files;

    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back({entry.path(), fs::file_size(entry.path())});
        } else {
            files.push_back({entry.path(), 0});
        }
    }

    // Sort by file size ascending or descending
   sort(files.begin(), files.end(),
    [descending](const auto &a, const auto &b) {
        return descending ? a.second > b.second : a.second < b.second;
    });
    cout << "\nFiles in Directory (Sorted by Size): " << fs::absolute(path) << "\n\n";

    for (auto &item : files) {
        if (fs::is_directory(item.first))
            cout << "[DIR]  " << item.first.filename().string() << endl;
        else
            cout << "       " << item.first.filename().string()
                 << " (" << formatSize(item.second) << ")" << endl;
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
    cout << "  lssize [desc]      -> list files sorted by size (add 'desc' for descending order)\n";
    cout << "  mkdir <folder>     -> create a new directory\n";
    cout << "  rmdir <folder>     -> remove an existing directory\n";

    cout << "  cd <folder>      -> change directory\n";
    cout << "  cd ..            -> go back\n";
    cout << "  cp <src> <dest>  -> copy file\n";
    cout << "  mv <src> <dest>  -> move or rename file\n";
    cout << "  rm <file>        -> delete file\n";
    cout << "  touch <file>     -> create file\n";
    cout << "  search <keyword> -> search for files or folders\n";
    cout << "  perm <file>        -> show file permissions\n";
    cout << "  chmod <file> <mode>-> change file permissions (e.g. 755)\n";  
    cout << "  exit               -> quit program\n\n";
    while (true) {
        cout << ">> ";
        cin >> command;

        if (command == "ls") {
            listFiles(currentPath);
        }
	else if (command == "lssize") {
    string order;
    getline(cin, order);
    if (!order.empty() && order[0] == ' ') order.erase(0, order.find_first_not_of(' '));
    bool descending = false;
    if (order == "desc" || order == "DESC") descending = true;
    listFilesSortedBySize(currentPath, descending);
}
else if (command == "mkdir") {
    string folder;
    cin >> folder;
    makeDirectory((fs::path(currentPath) / folder).string());
}

else if (command == "rmdir") {
    string folder;
    cin >> folder;
    removeDirectory((fs::path(currentPath) / folder).string());
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
	 else if (command == "search") {
            string keyword;
            cin >> keyword;
            searchFiles(currentPath, keyword);
        }
	else if (command == "perm") {
            string file;
            cin >> file;
            showPermissions((fs::path(currentPath) / file).string());
        }
        else if (command == "chmod") {
            string file, mode;
            cin >> file >> mode;
            changePermissions((fs::path(currentPath) / file).string(), mode);
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

