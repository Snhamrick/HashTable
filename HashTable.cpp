//============================================================================
// Name        : HashTable.cpp
// Author      : John Watson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;                                         //Set default size to 1000 due to size of MonthlySales

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    nodes.resize(tableSize);                                                //default vector to default size of 1000
      }

    


/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    this->tableSize = size;                                             //invoke this tablesize with size
    nodes.resize(tableSize);                                            //resize the vector
}


/**
 * Destructor
 */
HashTable::~HashTable() {                                               
    nodes.erase(nodes.begin(), nodes.end());                            //remove all elements of the vector

    
    
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    key = key % tableSize;                              //has the given key by performing modulo with tablesize
    return key;                                         //return the hashed key
}
/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    int key = hash(stoi(bid.bidId));                            //create new integer and set eqaul to the key of the hashed bidId
    Node* newNodePtr = new Node(bid, key);                      //create newNode pointer to Node to be added
    Node* previousNode;                                         //create pointer for previous node
    previousNode = &(nodes.at(key));                            //set previous node equal to the node at the hashed key index
    if (previousNode == nullptr) {                              //if previous node is nullptr
        nodes.at(key) = *newNodePtr;                                //set node at key position equal to new node
    }
    else if (previousNode->key == UINT_MAX) {                   //else if previous node's key is equal to UINT_MAX (never used node)
        nodes.at(key) = *newNodePtr;                                //set node at key position equal to new node
    }
    else {                                                      //else (node is occupied)
        Node* nextNode = previousNode->next;                        //create new pointer for nextNode set equal to previous node's next
        while (nextNode != nullptr) {                               //while nextNode is not null
            nextNode = nextNode->next;                              //move to next node
            previousNode = previousNode->next;                      //move previous node to next
        }
        previousNode->next = newNodePtr;                            //set previous node's next equal to newNode pointer (place at end of bucket list)
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    for (unsigned int i = 0; i < nodes.size(); i++) {                                                       //iterate through vector of nodes
        Node currentNode = nodes.at(i);                                                                     //create new node for current iteration
        if (currentNode.key != UINT_MAX) {                                                                  //if current node's key does not equal UINT_MAX (node is not empty)
                                                    //Output all of the current iterations bid information
            cout << "Key:" << currentNode.key << " " << currentNode.bid.bidId << " " << currentNode.bid.title << " " << currentNode.bid.fund << " " << currentNode.bid.amount << endl;

            if (currentNode.next != nullptr) {                                                              //if currentNode's next is not equal to null (bucket has remaining items) 
                Node* nextPrint = currentNode.next;                                                         //create new pointer for the next node to be printed
                while (nextPrint != nullptr) {                                                              //while next node to be printed is not null
                                                   //Output all of the current iterations bid information
                    cout << "Key:" << nextPrint->key << " " << nextPrint->bid.bidId << " " << nextPrint->bid.title << " " << nextPrint->bid.fund << " " << nextPrint->bid.amount << endl;
                    
                    nextPrint = nextPrint->next;                                                            //move to next bucket list item                
                }
            }
        }

    }

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    int key = hash(stoi(bidId));                                //create new integer for store key set equal to hashed bidId
    Bid emptyBid = Bid();                                       //create new empty bid
    Node headNode = nodes.at(key);                              //Create new node set equal to node at key position in vector
    if (headNode.bid.bidId == bidId) {                          //if the head node's bidId matches that to be removed
        nodes.at(key) = Node(emptyBid, key);                        //Remove node by setting node at key position to empty node with key value
        nodes.at(key).next = headNode.next;                         //Ensure that the next pointer remains the same
    }
    else if (headNode.next != nullptr) {                        //else if head node's next does not equal nullptr (bucket list is not empty)
        Node* previousNode = &(headNode);                           //create new pointer node to point to current iterations node
        Node* nextNode = headNode.next;                             //create new poiner to point to the next node in the bucket list
        while (nextNode != nullptr) {                               //while the next node is not null (item remain in list)
            if (nextNode->bid.bidId == bidId) {                     //if the next node's bidId matches that to be removed
                previousNode->next = nextNode->next;                    //previous node now points to next node's next (removing node)
                break;                                                  //break
            }
            nextNode = nextNode->next;                              //move to next node
            previousNode = previousNode->next;                      //move previous node to next
        }
    }
}
/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;                                                    //create empty bid
    int key = hash(stoi(bidId));                                //create integer to store key set equal to hashed bidId
    Node headNode = nodes.at(key);                              //create new node set equal to the node at key position
    if (headNode.bid.bidId == bidId) {                          //if the current nodes's bidId matches that of the search
        return headNode.bid;                                        //return current node's bid
    }
    else if (headNode.next != nullptr) {                        //else if current node's next is not null (items remain in bucket list)
        Node* listPointer = headNode.next;                          //Create new node point set equal to the next node in the bucket list
        while (listPointer != nullptr) {                            //while the next node is not null
            if (listPointer->bid.bidId == bidId) {                      //if the current bidId matches that of the search
                return listPointer->bid;                                    //return the current bid
            }
            listPointer = listPointer->next;                            //move to the next node in the bucket list
        }
        return bid;                                             //if no bid is found then return empty bid
    }
    else {
        return bid;                                             //return empty bid if no matches found

    }
    
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "96300";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
