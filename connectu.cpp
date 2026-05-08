/*
 * PROJECT: CONNECT-U (Starter Code)
 * Course: ECE367L Data Structures & Algorithms
 * 
 * SAFETY UPDATE:
 * The saveData() function call in main() is currently COMMENTED OUT.
 * This prevents you from accidentally wiping your 'posts.csv' file
 * if your Lab 1 implementation is incomplete.
 * * ONLY uncomment saveData() after you have verified Lab 1 works!
 */
////////Mina Girgis/////



#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

// ==========================================
// MODELS & DATA STRUCTURES
// ==========================================

struct Post {
    int postId;
    int userId;
    string content;
    int likes;
    long timestamp;
    Post* next; 

    Post(int pid, int uid, string txt, int lk, long time) 
        : postId(pid), userId(uid), content(txt), likes(lk), timestamp(time), next(nullptr) {}
        
    // TODO: LAB 3 - Implement Scoring Logic
   double getScore() {
    double hoursOld = (time(0) - timestamp) / 3600.0;
    return (likes * 10) + (1000 / (hoursOld + 1));
}
};

///////// LAB 1 - Linked List //////////////

class Timeline {
public:
    Post* head;
    Timeline() : head(nullptr) {}

    // Task: Add a new post to the FRONT of the list (O(1))
   void addPost(int pid, int uid, string content, int likes, long time) {
    // Create a new Post node
    Post* newPost = new Post(pid, uid, content, likes, time);

    // Connect the new post to the current first post
    newPost->next = head;

    // Move head to the new post (now it's at the front)
    head = newPost;
}


    

    void printTimeline() {
    Post* current = head;
    if (!current) { 
        cout << "  (No posts yet)" << endl; 
        return; 
    }

    while (current != nullptr) {

        cout << "  > [ID: " << current->postId << "] "
             << current->content 
             << " (" << current->likes << " likes)" << endl;

        current = current->next;
    }
}
};
////////////// END LAB 1////////////////

    
// Forward Declaration
class User;

// TODO: LAB 4 - Binary Search Tree
struct BSTNode {
    User* user;
    BSTNode* left;
    BSTNode* right;
    BSTNode(User* u) : user(u), left(nullptr), right(nullptr) {}
};

class FriendBST {
public:
    BSTNode* root;
    FriendBST() : root(nullptr) {}

    BSTNode* insert(BSTNode* node, User* u) ;

    void printInOrder(BSTNode* node);

    void addFriend(User* u) { root = insert(root, u); }
    

// LAB 6 - BST Deletion Functions
BSTNode* findMin(BSTNode* node);
BSTNode* deleteNode(BSTNode* node, string username);
void deleteFriend(string username);

    void printFriends() {
        if (root == nullptr) cout << "  (No friends yet)" << endl;
        else printInOrder(root);
    }
};

class User {
public:
    int userId;
    string username;
    int techScore, artScore, sportScore;
    
    Timeline timeline;       // Lab 1
    vector<User*> friends;   // Graph
    FriendBST friendTree;    // Lab 4

    User(int id, string name, int t, int a, int s) 
        : userId(id), username(name), techScore(t), artScore(a), sportScore(s) {}

    void addPost(int pid, string content, int likes, long time) {
        timeline.addPost(pid, userId, content, likes, time);
    }

    void addFriend(User* u) {
        friends.push_back(u);       
        friendTree.addFriend(u);    
    }
    
    vector<User*> getFriendsList() { return friends; }
};
// LAB 6 - Find the smallest user in a subtree
BSTNode* FriendBST::findMin(BSTNode* node) {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// LAB 6 - Delete a user from the BST
BSTNode* FriendBST::deleteNode(BSTNode* node, string username) {
    if (node == nullptr) return nullptr;

    if (username < node->user->username) {
        node->left = deleteNode(node->left, username);
    }
    else if (username > node->user->username) {
        node->right = deleteNode(node->right, username);
    }
    else {
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        BSTNode* temp = findMin(node->right);
        node->user = temp->user;
        node->right = deleteNode(node->right, temp->user->username);
    }

    return node;
}

// LAB 6 - Public delete function
void FriendBST::deleteFriend(string username) {
    root = deleteNode(root, username);
}
/* BST Implementation  lab 4 */
BSTNode* FriendBST::insert(BSTNode* node, User* u) {
    if (node == nullptr) {
        return new BSTNode(u);
    }

    if (u->username < node->user->username) {
        node->left = insert(node->left, u);
    }
    else if (u->username > node->user->username) {
        node->right = insert(node->right, u);
    }

    return node;
}

void FriendBST::printInOrder(BSTNode* node) {
    if (node == nullptr) return;

    printInOrder(node->left);
    cout << "  > @" << node->user->username << endl;
    printInOrder(node->right);
}
//end of lab 4

// TODO: LAB 3 - Max Heap
class FeedHeap {
private:
    Post* heap[1000];
    int size;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (heap[parent]->getScore() >= heap[index]->getScore())
                break;

            swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap[left]->getScore() > heap[largest]->getScore())
                largest = left;

            if (right < size && heap[right]->getScore() > heap[largest]->getScore())
                largest = right;

            if (largest == index)
                break;

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    FeedHeap() : size(0) {}

    void push(Post* p) {
        heap[size] = p;
        heapifyUp(size);
        size++;
    }

    Post* popMax() {
        if (size == 0)
            return nullptr;

        Post* top = heap[0];
        heap[0] = heap[size - 1];
        size--;

        heapifyDown(0);

        return top;
    }

    bool isEmpty() { return size == 0; }
};

vector<User*> allUsers;

// TODO: LAB 2 - Hash Map
struct HashNode {
    string key;
    User* value;
    HashNode* next;
    HashNode(string k, User* v) : key(k), value(v), next(nullptr) {}
};

class UserMap {
private:
    static const int TABLE_SIZE = 10007; 
    HashNode** table;

    // Polynomial Rolling Hash Function
    unsigned long hashFunction(string key) {
        unsigned long hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    UserMap() {
        table = new HashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    // Insert user into hash table using chaining
    void put(string key, User* user) {
        unsigned long index = hashFunction(key);

        HashNode* current = table[index];

        // Check if key already exists
        while (current != nullptr) {
            if (current->key == key) {
                current->value = user;
                return;
            }
            current = current->next;
        }

        // Insert new node at front (chaining)
        HashNode* newNode = new HashNode(key, user);
        newNode->next = table[index];
        table[index] = newNode;
    }

    // O(1) average lookup
    User* get(string key) {
        unsigned long index = hashFunction(key);

        HashNode* current = table[index];

        while (current != nullptr) {
            if (current->key == key)
                return current->value;
            current = current->next;
        }

        return nullptr;
    }
};

UserMap userMap;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

vector<string> split(string s) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    
    for (char c : s) {
        if (c == '"') {
            inQuotes = !inQuotes; // Toggle quote state
            continue; // Skip the quote character itself
        }
        
        if (c == ',' && !inQuotes) {
            // Found a delimiter outside of quotes -> New Token
            tokens.push_back(token);
            token.clear();
        } else {
            // Regular character or comma inside quotes
            token += c;
        }
    }
    tokens.push_back(token); // Add last token
    return tokens;
}


int GLOBAL_POST_ID_COUNTER = 1;

Post* findPostById(int id) {
    for (User* u : allUsers) {
        Post* curr = u->timeline.head;
        while (curr != nullptr) {
            if (curr->postId == id) return curr;
            curr = curr->next;
        }
    }
    return nullptr;
}

void createNewPost(User* author, string content) {
    int postId = GLOBAL_POST_ID_COUNTER++;
    long timestamp = time(0); 
    author->addPost(postId, content, 0, timestamp);
    cout << "\n[SUCCESS] Post saved to timeline." << endl;
}

void registerNewUser(string username, int tech, int art, int sport) {
    int newId = allUsers.size() + 1; 
    User* newUser = new User(newId, username, tech, art, sport);
    allUsers.push_back(newUser);
    userMap.put(username, newUser);
    cout << "\n[SUCCESS] Account created." << endl;
}

void addFriendship(User* requester, User* target) {
    requester->addFriend(target);
    target->addFriend(requester);
    cout << "\n[SUCCESS] You are now friends with @" << target->username << endl;
}
// LAB 6 - Remove Friend using BST Deletion
void removeFriend(User* currentUser) {
    string friendName;

    cout << "Enter username to remove: ";
    cin >> friendName;

    User* target = userMap.get(friendName);

    if (target == nullptr) {
        cout << "User not found." << endl;
        return;
    }

    bool found = false;

    // Remove target from current user's friends vector
    for (int i = 0; i < currentUser->friends.size(); i++) {
        if (currentUser->friends[i]->username == friendName) {
            currentUser->friends.erase(currentUser->friends.begin() + i);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "That user is not in your friends list." << endl;
        return;
    }

    // Remove current user from target user's friends vector
    for (int i = 0; i < target->friends.size(); i++) {
        if (target->friends[i]->username == currentUser->username) {
            target->friends.erase(target->friends.begin() + i);
            break;
        }
    }

    // Remove from both BSTs
    currentUser->friendTree.deleteFriend(friendName);
    target->friendTree.deleteFriend(currentUser->username);

    cout << "\n[SUCCESS] Removed @" << friendName << " from your friends list." << endl;
}

//TODO: LAB 5 - Breadth First Search
void recommendFriends(User* startUser) {
    cout << "\n[GRAPH ANALYSIS] Friend Recommendations:\n";

    set<int> visited;
    queue<User*> q;

    // Mark yourself as visited
    visited.insert(startUser->userId);

    // Add direct friends to visited (so we don't recommend them)
    for (User* f : startUser->friends) {
        visited.insert(f->userId);
        q.push(f); // start BFS from friends
    }

    set<string> recommendations;

    // BFS
    while (!q.empty()) {
        User* current = q.front();
        q.pop();

        for (User* neighbor : current->friends) {
            if (visited.find(neighbor->userId) == visited.end()) {
                // Not visited → recommend
                recommendations.insert(neighbor->username);
                visited.insert(neighbor->userId);
            }
        }
    }

    // Print results
    if (recommendations.empty()) {
        cout << "  No recommendations found.\n";
    } else {
        for (const string& name : recommendations) {
            cout << "  > @" << name << endl;
        }
    }
}

// ==========================================
// FILE I/O 
// ==========================================

void loadData() {
    cout << "Loading data from CSV files..." << endl;
    ifstream userFile("users.csv");
    string line;
    if (userFile.is_open()) {
        getline(userFile, line); 
        while (getline(userFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            User* newUser = new User(stoi(row[0]), row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]));
            allUsers.push_back(newUser);
            userMap.put(row[1], newUser); 
        }
        userFile.close();
    }
    ifstream relFile("relations.csv");
    if (relFile.is_open()) {
        getline(relFile, line); 
        while (getline(relFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 2) continue;
            int u1 = stoi(row[0]); int u2 = stoi(row[1]);
            if (u1 <= allUsers.size() && u2 <= allUsers.size()) {
                allUsers[u1-1]->addFriend(allUsers[u2-1]);
                allUsers[u2-1]->addFriend(allUsers[u1-1]);
            }
        }
        relFile.close();
    }
    ifstream postFile("posts.csv");
    if (postFile.is_open()) {
        getline(postFile, line); 
        while (getline(postFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            int pid = stoi(row[0]); int uid = stoi(row[1]);
            if (uid <= allUsers.size()) {
                allUsers[uid-1]->addPost(pid, row[2], stoi(row[3]), stol(row[4])); 
                if (pid >= GLOBAL_POST_ID_COUNTER) GLOBAL_POST_ID_COUNTER = pid + 1;
            }
        }
        postFile.close();
    }
}

void saveData() {
    // SAFETY CHECK: If no users exist, do not overwrite files!
    if (allUsers.empty()) {
        cout << "[SAFETY] No data in memory. Skipping save to prevent file wipe." << endl;
        return;
    }

    cout << "Saving data..." << endl;
    ofstream userFile("users.csv");
    userFile << "user_id,username,tech_score,art_score,sport_score\n";
    for (User* u : allUsers) {
        userFile << u->userId << "," << u->username << "," << u->techScore << "," << u->artScore << "," << u->sportScore << "\n";
    }
    userFile.close();

    ofstream relFile("relations.csv");
    relFile << "user_id_1,user_id_2\n";
    for (User* u : allUsers) {
        for (User* f : u->friends) {
            if (u->userId < f->userId) relFile << u->userId << "," << f->userId << "\n";
        }
    }
    relFile.close();

    ofstream postFile("posts.csv");
    postFile << "post_id,user_id,content,likes,timestamp\n";
    for (User* u : allUsers) {
        vector<Post*> temp;
        Post* curr = u->timeline.head;
        while (curr) { temp.push_back(curr); curr = curr->next; }
        for (int i = temp.size() - 1; i >= 0; i--) {
            Post* p = temp[i];
            string safeContent = p->content;
            if (safeContent.find(',') != string::npos) {
                safeContent = "\"" + safeContent + "\"";
            } 
            postFile << p->postId << "," << p->userId << "," << safeContent << "," << p->likes << "," << p->timestamp << "\n";
        }
    }
    postFile.close();
    cout << "Done." << endl;
}

// ==========================================
// MAIN MENU (UI)
// ==========================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showUserDashboard(User* currentUser) {
    int choice = 0;
    while (choice != 7) {
        cout << "\n--- Welcome, @" << currentUser->username << " ---" << endl;
        cout << "1. View My Post (Lab 1)" << endl;
        cout << "2. Create New Post (Lab 1)" << endl;
        cout << "3. Add Friend (Lab 2)" << endl;
        cout << "4. Algorithmic Feed (Lab 3)" << endl;
        cout << "5. View Friends Sorted (Lab 4)" << endl;
        cout << "6. Get Friend Recommendations (Lab 5)" << endl;
        cout << "7. Logout" << endl;
        // Lab 6
        cout << "8. Remove Friend (Lab 6)" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n[MY POST]" << endl;
            currentUser->timeline.printTimeline();
        }
        else if (choice == 2) {
            cout << "\nEnter post content: ";
            cin.ignore(); 
            string content;
            getline(cin, content);
            createNewPost(currentUser, content);
        }
        else if (choice == 3) {
            string friendName;
            cout << "Enter username to add: "; cin >> friendName;
            User* target = userMap.get(friendName);
            if(target && target != currentUser) addFriendship(currentUser, target);
            else cout << "Invalid user (or Hash Map not implemented)." << endl;
        }
        else if (choice == 4) {
            cout << "\n[ALGORITHMIC FEED]" << endl;
            FeedHeap feed;
            vector<User*> friends = currentUser->getFriendsList();
            for(User* f : friends) {
                Post* p = f->timeline.head;
                int limit = 0;
                while(p != nullptr && limit < 5) {
                    feed.push(p);
                    p = p->next;
                    limit++;
                }
            }
            int count = 0;
            while(!feed.isEmpty() && count < 10) {
                Post* top = feed.popMax();
                if(top)
                    cout << "  > [ID: " << top->postId << "] [Score: " << (int)top->getScore() << "] @" 
                         << allUsers[top->userId - 1]->username << ": " << top->content 
                         << " (" << top->likes << " likes)" << endl;
                count++;
            }
            if(count == 0) cout << "  No posts found." << endl;
            else {
                cout << "\nDo you want to like a post? (y/n): ";
                char resp; cin >> resp;
                if (resp == 'y' || resp == 'Y') {
                    int pid; cout << "Enter Post ID: "; cin >> pid;
                    Post* p = findPostById(pid);
                    if (p) { p->likes++; cout << "Liked!" << endl; }
                }
            }
        }
        else if (choice == 5) {
            cout << "\n[MY FRIENDS]" << endl;
            currentUser->friendTree.printFriends();
        }
        else if (choice == 6) {
             recommendFriends(currentUser);
        }
        // Lab 6 
        else if (choice == 8) {
             removeFriend(currentUser);
        }   
        else if (choice == 7) {
            cout << "Logging out..." << endl;
        }
    }
}

void showMainMenu() {
    int choice = 0;
    while (choice != 3) {
        cout << "\n=== CONNECT-U ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit & Save" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            string username;
            cout << "Username: "; cin >> username;
            User* user = userMap.get(username);
            if (user) showUserDashboard(user);
            else cout << "User not found." << endl;
        } 
        else if (choice == 2) {
            string username;
            int t, a, s;
            cout << "Username: "; cin >> username;
            cout << "Tech/Art/Sport (1-10): "; cin >> t >> a >> s;
            registerNewUser(username, t, a, s);
        }
        else if (choice == 3) {
            // SAFETY: Commented out to prevent data loss on initial run.
            // Students must uncomment this ONLY when Lab 1 is complete.
             saveData(); 
            cout << "Goodbye! " << endl;
        }
    }
}

int main() {
    loadData(); 
    clearScreen();
    showMainMenu();
    return 0;
}
