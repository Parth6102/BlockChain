#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Node class that represents a node of a binary tree
class Node {
public:
    std::string data;
    Node* left;
    Node* right;

    // function to initialise the node with data, left child, and right child, representing a binary
    // tree
    Node(std::string data) : data(data), left(nullptr), right(nullptr) {}

    // utility function to check if a given node has 2 children
    bool isFull() {
        return left && right;
    }



    // utility function to print the contents of the node
    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << node.data;
        return os;
    }

    // utility function to check if a given node has 0 children (is a leaf node)
    bool isLeaf() {
        return !left && !right;
    }
};

// Merkle tree class for actual implementation of the tree
class MerkleTree {
private:
    Node* root;
    std::string merkleRoot;

    // function to generate a tree from a list. The list is expected to contain
    // the list of transactions(as strings) we wish to seal in a block
    void makeTreeFromArray(std::vector<std::string>& arr) {
        int nodesReqd = 2 * arr.size() - 1;
        std::vector<int> nodeArr(nodesReqd);
        for (int i = 0; i < nodesReqd; ++i) {
            nodeArr[i] = i + 1;
        }
        root = buildTree(nodeArr, 0, nodesReqd);
        addLeafData(arr, root);
    }

    Node* buildTree(std::vector<int>& arr, int i, int n) {
        if (i < n) {
            Node* temp = new Node(std::to_string(arr[i]));
            temp->left = buildTree(arr, 2 * i + 1, n);
            temp->right = buildTree(arr, 2 * i + 2, n);
            return temp;
        }
        return nullptr;
    }

    void addLeafData(std::vector<std::string>& arr, Node* node) {
        if (!node)
            return;

        addLeafData(arr, node->left);
        if (node->isLeaf()) {
            node->data = returnHash(arr[0]);
            arr.erase(arr.begin());
        } else {
            node->data = "";
        }
        addLeafData(arr, node->right);
    }

    // utility function to traverse the tree using inorder Traversal algorithm
    void inorderTraversal(Node* node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            std::cout << *node << std::endl;
            inorderTraversal(node->right);
        }
    }

    // function to calculate merkle root of the tree.
    std::string calculateMerkleRoot() {
        merkleRoot = merkleHash(root)->data;
        return merkleRoot;
    }

    Node* merkleHash(Node* node) {
        if (node->isLeaf()) {
            return node;
        }

        std::string left = merkleHash(node->left)->data;
        std::string right = merkleHash(node->right)->data;
        node->data = left + right; // Concatenation instead of hashing
        return node;
    }

    // utility function to get the merkle Root of that tree
    std::string getMerkleRoot() {
        return merkleRoot;
    }

    // utility function to verify if the transactions are intact with respect to this tree.
    bool verifyUtil(std::vector<std::string>& arr1) {
        std::string hash1 = getMerkleRoot();
        MerkleTree newTree;
        newTree.makeTreeFromArray(arr1);
        std::string hash2 = newTree.calculateMerkleRoot();
        if (hash1 == hash2) {
            std::cout << "Transactions verified Successfully" << std::endl;
            return true;
        } else {
            std::cout << "Transactions have been tampered" << std::endl;
            return false;
        }
    }

    std::string returnHash(std::string x) {
        // Simple concatenation instead of hashing
        return x;
    }

public:
    MerkleTree() : root(nullptr) {}

    // Driver function to run the Merkle tree
    void run() {
        std::vector<std::string> transactions = {"Transaction1", "Transaction2", "Transaction3", "Transaction4"};
        makeTreeFromArray(transactions);
        std::cout << "Inorder Traversal of Merkle Tree:" << std::endl;
        inorderTraversal(root);
        std::cout << "Merkle Root: " << calculateMerkleRoot() << std::endl;
        std::vector<std::string> newTransactions = {"Transaction1", "Transaction2", "Transaction3", "Transaction4"};
        verifyUtil(newTransactions);
    }
};

int main() {
    MerkleTree merkleTree;
    merkleTree.run();
    return 0;
}
