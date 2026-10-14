// C++ program to implement huffman coding

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// A Huffman tree node
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    Node(char ch, int freq)
        : ch(ch)
        , freq(freq)
        , left(nullptr)
        , right(nullptr)
    {
    }
    Node(char ch, int freq, Node* left, Node* right)
        : ch(ch)
        , freq(freq)
        , left(left)
        , right(right)
    {
    }
};

// Comparison object to be used to order the heap
struct compare {
    bool operator()(Node* l, Node* r)
    {
        return l->freq > r->freq;
    }
};

// Function to print the Huffman Codes
void printCodes(Node* root, string str,
                unordered_map<char, string>& huffmanCode)
{
    if (root == nullptr)
        return;

    // Found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

// Function to build the Huffman Tree and generate Huffman
// Codes
int buildHuffmanTree(string text, string filename)
{
    // Count frequency of appearance of each character and
    // store it in a map
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    // Create a priority queue to store live nodes of the
    // Huffman tree
    priority_queue<Node*, vector<Node*>, compare> pq;

    // Create a leaf node for each character and add it to
    // the priority queue
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Do till there is more than one node in the queue
    while (pq.size() != 1) {
        // Remove the two nodes of highest priority (lowest
        // frequency) from the queue
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        // Create a new internal node with these two nodes
        // as children and with frequency equal to the sum
        // of the two nodes' frequencies
        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    // Root stores pointer to the root of the Huffman Tree
    Node* root = pq.top();

    // Traverse the Huffman Tree and store Huffman Codes in
    // a map
    unordered_map<char, string> huffmanCode;
    printCodes(root, "", huffmanCode);
    
    // Store Huffman Codes
    ofstream key_file(filename + ".key");
    if (key_file.is_open()){
        for (auto pair : huffmanCode) {
            key_file << pair.first;
            //key_file << pair.second;
        }
        key_file.close();
    }else{
        cerr << "Error: could not create printcode file" << endl;
    }

    // Store encoded string
    ofstream lock_file(filename + ".lock");

    if (lock_file.is_open()){
        string str = "";
        for (char ch : text) {
            str += huffmanCode[ch];
        }
        lock_file << str;
        lock_file.close();
    }else{
        cerr << "Error: Could not create lock file" << endl;
    }

    return 0;
}

int decode(string filename){
    priority_queue<Node*, vector<Node*>, compare> pq;

    ifstream key(filename + ".key");
    if (!key.is_open()) {
        cerr << "Error: Could not open file " << keyfile << "\n";
        return 1;
    }

    ifstream lock(filename + ".lock");
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << keyfile << "\n";
        return 2;
        string str;
        stringstream buffer;
        buffer << lock.rdbuf();
        str = buffer.str();
    }

    ofstream out_file(filename);
    if (!out_file.is_open()) {
        cerr << "Error: Could not create file " << out_file << "\n";
        return 3;
    }else{
        Node* curr = root;
        for (char bit : str) {
            if (bit == '0') {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }

            // Reached a leaf node
            if (!curr->left && !curr->right) {
                cout << curr->ch;
                curr = root;
            }
        }
    }
    
    return 0;
};

int main(int argc, char* argv[]){

    if (argc < 2) {
        cerr << "Error: Missing input file.\n";
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    string filename = argv[1];

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return 1;
    }

    string text;
    stringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();

    buildHuffmanTree(text, filename);
    
    return 0;
}
