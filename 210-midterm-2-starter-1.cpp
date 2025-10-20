#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        Node(int val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(int value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(int value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(int v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(int v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); // rand() was giving the same numbers every time, so I found this code on stackoverflow to produce different results each run
    
    vector<string> names;
    string custName;
    int prob;

    ifstream file("names.txt");
    while(file >> custName) {
        names.push_back(custName);
    }
    file.close();

    DoublyLinkedList storeLine;
    vector<int> lineID;
    int countID = 1;
    vector<string> nameToID(names.size());

    cout << "Store opens:" << endl;
    
    for(int i = 0; i < 5; i++) {
        int randomNum = rand() % names.size();
        while(find(lineID.begin(), lineID.end(), randomNum) != lineID.end()) {
            randomNum = rand() % names.size();
            cout << names[randomNum] << "ran" << endl;
        }
        string customer = names[randomNum];
        storeLine.push_back(countID);
        lineID.push_back(countID);
        nameToID[countID] = customer;
        cout << "\t" << customer << " joins the line" << endl;
        countID++;
    }

    cout << "\tResulting line:" << endl;
    for(int id : lineID) {
        cout << "\t\t" << nameToID[id] << endl;
    }

    for(int i = 2; i < 21; i++) {
        cout << "Time step #" << i << ":" << endl;

        // A customer being helped at the beginning of the line and ordering their coffee
        prob = rand() % 100 + 1;
        if(prob <= 40 && !lineID.empty()) {
            int servedID = lineID.front();
            cout << "\t" << nameToID[servedID] << " is served" << endl;
            storeLine.pop_front();
            lineID.erase(lineID.begin());
        }

        // A new customer joining the end of the line
        prob = rand() % 100 + 1;
        if(prob <= 60) {
            int randomNum = rand() % names.size();
            string customer = names[randomNum];
            storeLine.push_back(countID);
            lineID.push_back(countID);
            nameToID[countID] = customer;
            cout << "\t" << customer << " joins the line" << endl;
            countID++;
        }

        // The customer at the end of the line deciding they don't want to wait and leaving
        prob = rand() % 100 + 1;
        if(prob <= 20 && !lineID.empty()) {
            int rearID = lineID.back();
            cout << "\t" << nameToID[rearID] << " (at the rear) left the line" << endl;
            storeLine.pop_back();
            lineID.pop_back();
        }

        // Any particular customer can decide they don't want to wait and leave the line
        prob = rand() % 100 + 1;
        if(prob <= 10 && !lineID.empty()) {
            int randomNum = rand() % lineID.size();
            int leaveID = lineID[randomNum];
            cout << "\t" << nameToID[leaveID] << " left the line" << endl;
            storeLine.delete_val(leaveID);
            lineID.erase(lineID.begin() + randomNum);
        }

        // A VIP (very important person) customer with a Coffee House Gold Card gets to skip the line and go straight to the counter and order
        prob = rand() % 100 + 1;
        if(prob <= 10 && !lineID.empty()) {
            int randomNum = rand() % names.size();
            string customer = names[randomNum];
            storeLine.push_front(countID);
            lineID.insert(lineID.begin(), countID);
            nameToID[countID] = customer;
            cout << "\t" << customer << " (VIP) joins the front of the line" << endl;
            countID++;
        }

        // Print customers in line
        cout << "\tResulting line:" << endl;
        if (lineID.empty()) {
            cout << "\t(empty)\n";
        } else {
            for(int id : lineID) {
                cout << "\t\t" << nameToID[id] << endl;
            }
        }
        
    }
    
    return 0;
}