#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

class DictionaryNode {
private:
    std::unordered_map<std::string, std::string> store;
    std::string nodeName;
    std::vector<DictionaryNode*> neighbors;

public:
    DictionaryNode(std::string name) : nodeName(name) {}

    void connect(DictionaryNode* node) {
        neighbors.push_back(node);
    }

    void put(const std::string& key, const std::string& value) {
        store[key] = value;
        std::cout << "[INFO] Stored in " << nodeName << ": " << key << " = " << value << "\n";
    }

    std::string get(const std::string& key) {
        if (store.find(key) != store.end()) {
            std::cout << "[INFO] Found in " << nodeName << "\n";
            return store[key];
        }

        for (auto neighbor : neighbors) {
            std::string val = neighbor->get(key);
            if (!val.empty()) return val;
        }

        return "";
    }

    void del(const std::string& key) {
        if (store.erase(key)) {
            std::cout << "[INFO] Deleted " << key << " from " << nodeName << "\n";
        }
        else {
            for (auto neighbor : neighbors) {
                neighbor->del(key);
            }
        }
    }

    std::string getName() {
        return nodeName;
    }

    void printStore() {
        std::cout << "=== Store in " << nodeName << " ===\n";
        for (auto& pair : store)
            std::cout << pair.first << " = " << pair.second << "\n";
        std::cout << "===========================\n";
    }
};

DictionaryNode* findNode(const std::string& name, const std::vector<DictionaryNode*>& nodes) {
    for (auto node : nodes) {
        if (node->getName() == name) return node;
    }
    return nullptr;
}

int main() {
    DictionaryNode node1("node1"), node2("node2"), node3("node3");

    // Connect nodes (bidirectional simulation)
    node1.connect(&node2);
    node1.connect(&node3);

    node2.connect(&node1);
    node2.connect(&node3);

    node3.connect(&node1);
    node3.connect(&node2);

    std::vector<DictionaryNode*> nodes = { &node1, &node2, &node3 };

    std::string input;
    std::cout << "Distributed Dictionary CLI (commands: put, get, delete, print, exit)\n";

    while (true) {
        std::cout << "> ";
        getline(std::cin, input);
        std::stringstream ss(input);
        std::string command, nodeName, key, value;
        ss >> command;

        if (command == "exit") break;

        if (command == "put") {
            ss >> nodeName >> key >> value;
            DictionaryNode* node = findNode(nodeName, nodes);
            if (node) node->put(key, value);
            else std::cout << "Node not found!\n";
        }
        else if (command == "get") {
            ss >> nodeName >> key;
            DictionaryNode* node = findNode(nodeName, nodes);
            if (node) {
                std::string val = node->get(key);
                if (!val.empty()) std::cout << "Value: " << val << "\n";
                else std::cout << "Key not found\n";
            }
            else std::cout << "Node not found!\n";
        }
        else if (command == "delete") {
            ss >> nodeName >> key;
            DictionaryNode* node = findNode(nodeName, nodes);
            if (node) node->del(key);
            else std::cout << "Node not found!\n";
        }
        else if (command == "print") {
            for (auto n : nodes) n->printStore();
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
