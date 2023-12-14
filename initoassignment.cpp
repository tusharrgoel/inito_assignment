
#include<bits/stdc++.h>
using namespace std;
class FileSystem{
    private:
    struct Node{
        string name;
        vector<Node*> children;
    };
        Node* root;
        Node* currentDirectory;
        
    public:
    FileSystem() {
        root = new Node{"root", {}};
        currentDirectory = root;
    }
    
    ~FileSystem(){
        deleteTree(root);
    }
    
    void deleteTree(Node* node) {
        for (Node* child : node->children) {
            deleteTree(child);
        }
        delete node;
    }
    
    void mkdir(string& dirname) {
    if (findChild(currentDirectory, dirname) != nullptr) {
        cout << "Directory already exists: " << dirname <<endl;
    } else {
        Node* newDir = new Node{dirname, {}};
        currentDirectory->children.push_back(newDir);
     }
   }
   
    void cd(string& dirname) {
        if (dirname == "..") {
            if (currentDirectory != root) {
                currentDirectory = getParent(currentDirectory);
            }
        } else {
            Node* targetDir = findChild(currentDirectory, dirname);
            if (targetDir != nullptr) {
                currentDirectory = targetDir;
            } else {
                cout << "Directory not found: " << dirname << endl;
            }
        }
    }
    void ls() const {
        cout << "Contents of " << currentDirectory->name << ":" << endl;
        for (Node* child : currentDirectory->children) {
            cout << "  " << child->name << endl;
        }
    }
    
    void cat(string& filename) {
        ifstream file(filename);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
    }
   }
   void touch(const string& filename) {
        if (findChild(currentDirectory, filename) != nullptr) {
            cout << "File already exists: " << filename << endl;
        } else {
            Node* newFile = new Node{filename, {}};
            currentDirectory->children.push_back(newFile);
            ofstream file(filename);
            file.close();
        }
    }
     void echo(const string& text, const string& filename) {
        Node* existingFile = findChild(currentDirectory, filename);
        if (existingFile != nullptr) {
            ofstream file(filename, ios::app);
            file << text << endl;
            file.close();
        } else {
            touch(filename);
            ofstream file(filename);
            file << text << endl;
            file.close();
        }
    }
     void mv(const string& source, const string& destination) {
        Node* sourceNode = findChild(currentDirectory, source);
        Node* destinationNode = findChild(currentDirectory, destination);

        if (sourceNode == nullptr) {
            cout << "Source not found: " << source << endl;
        } else if (destinationNode != nullptr) {
            cout << "Destination already exists: " << destination << endl;
        } else {
            sourceNode->name = destination;
        }
    }
    void cp(const string& source, const string& destination) {
        Node* sourceNode = findChild(currentDirectory, source);
        Node* destinationNode = findChild(currentDirectory, destination);

        if (sourceNode == nullptr) {
            cout << "Source not found: " << source << endl;
        } else if (destinationNode != nullptr) {
            cout << "Destination already exists: " << destination << endl;
        } else {
            // Copy source to destination
            Node* newCopy = new Node{source, {}};
            currentDirectory->children.push_back(newCopy);
        }
    }

    void rm(const string& name) {
        auto it = remove_if(
            currentDirectory->children.begin(),
            currentDirectory->children.end(),
            [name](Node* node) { return node->name == name; });

        currentDirectory->children.erase(it, currentDirectory->children.end());
    }
   private:
    Node* getParent(Node* node) const {
        for (Node* parent : root->children) {
            if (isChild(parent, node)) {
                return parent;
            }
        }
        return nullptr;
    }

    bool isChild(Node* parent, Node* child) const {
        for (Node* node : parent->children) {
            if (node == child) {
                return true;
            }
        }
        return false;
    }
    
    Node* findChild(Node* parent, const string& name) const {
        for (Node* node : parent->children) {
            if (node->name == name) {
                return node;
            }
        }
        return nullptr;
    }
};

int main()
{
    FileSystem fileSystem;
    while (true) {
        string command;
        cout << "> ";
        getline(cin, command);

        istringstream iss(command);
        string cmd;
        iss >> cmd;

        if (cmd == "mkdir") {
            string dirname;
            iss >> dirname;
            fileSystem.mkdir(dirname);
            cout << "Directory created: " << dirname << endl;
        } else if (cmd == "cd") {
            string dirname;
            iss >> dirname;
            fileSystem.cd(dirname);
        } else if (cmd == "ls") {
            fileSystem.ls();
        }else if (cmd == "touch") {
            string filename;
            iss >> filename;
            fileSystem.touch(filename);
        } else if (cmd == "cat") {
            string filename;
            iss >> filename;
            fileSystem.cat(filename);
        }else if (cmd == "echo") {
            string text;
            getline(iss >> ws, text);  
            string filename;
            iss >> filename;
            fileSystem.echo(text, filename);
        } else if (cmd == "mv") {
            string source, destination;
            iss >> source >> destination;
            fileSystem.mv(source, destination);
        } else if (cmd == "cp") {
            string source, destination;
            iss >> source >> destination;
            fileSystem.cp(source, destination);
        } else if (cmd == "rm") {
            string name;
            iss >> name;
            fileSystem.rm(name);
        } else if (cmd == "exit") {
            break;
        } else {
            cout << "Invalid command" << endl;
        }
    }
    return 0;
}
