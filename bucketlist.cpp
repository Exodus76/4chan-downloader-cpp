#include "BucketList.h"
#include <iostream>
using namespace std;

Node::Node() {
	// IMPLEMENT ME
    next_ = nullptr;
}

Node::~Node() {
	// IMPLEMENT ME
}

BucketList::BucketList() : size_(0), cap_(INITIAL_CAP){
	buckets_ = new Node*[cap_]();
    // IMPLEMENT ME
    //called the constructor
    for (int i = 0; i < cap_; i++) {
        buckets_[i] = nullptr;
    }
}

BucketList::~BucketList() {
	// IMPLEMENT ME
    cout << "destructor called" <<endl;
    for (int i = 0; i < getCap(); i++) {
        Node* head = buckets_[i];
        if (buckets_[i] == nullptr){
            delete buckets_[i];
        } else {
            if (head->next_ == nullptr) {
                delete head;
                cout << i << " cleared" << endl;
            } else {
                while (head->next_ != nullptr) {
                    Node* wow = head->next_;
                    head->next_ = wow->next_;
                    delete wow; //deletes blue
                }
                if (head != nullptr ) {
                    delete head;
                }
            }
        }
    }
    delete [] buckets_;
    cout << "sucessfully cleared memory" << endl;
}

BucketList::BucketList(const BucketList& other) {
	// IMPLEMENT ME
    size_ = other.size_;
    cap_ = other.cap_;
    buckets_ = new Node*[cap_];
    for (int i = 0; i < size_ ;i++) {
        buckets_[i] = other.buckets_[i];
    }
}

//other is the one we are copying from
BucketList& BucketList::operator=(const BucketList& other) {
	// IMPLEMENT ME
    cout << "copy assignment called " << endl;
    if (this != &other) {
        delete [] buckets_;
        this->~BucketList();
        buckets_ = new Node*[cap_];
        for (int i = 0; i < cap_; i++) {
            Node* temp = other.buckets_[i];
            while (temp != nullptr) {
                string allo = temp->s_;
                cout << "inserting " << allo << endl;
                this->insert(allo);
                temp = temp->next_;
            }
        }
    }
    size_ = other.size_;
    cap_ = other.cap_;
    cout << "capacity is " << cap_ << "with size " << size_ << endl;
	return *this;
}

bool BucketList::contains(const string& s) const {

	// THIS IS DONE FOR YOU AS AN EXAMPLE
	// but you need to change something...

	int hc = h(s) % getCap(); // ...here.
    // cout << hc << endl;
	Node* temp = buckets_[hc];
    // if (temp == nullptr) cout << "huh?" <<endl;
	while (temp != nullptr) {
        // cout << "wtf" <<endl;
		if (temp->s_ == s) return true;
		temp = temp->next_;
	}
	return false;
}

void BucketList::insert(const string& s) {
	// IMPLEMENT ME
    // cout << h(s) << endl;
    int loc = h(s);
    // auto d = contains(s);
    if (!contains(s)) { // string is in do nothing
        loc = loc % getCap();
        Node* newString = new Node;
        Node* prev = buckets_[loc];
        // if ((double)getSize()/getCap() == MAX_LOAD_FACTOR) {
        // 	cap_ = cap_ * 2;
        //     //(s);
        // }
        // cout << "just before" << endl;
        if (prev == nullptr) {
            newString->s_ = s;
            newString->next_ = nullptr;
            buckets_[loc] = newString;
        } else {
            Node* current = buckets_[loc]->next_; //use current when it actually points to something
            while (current != nullptr && current->s_ < s) {
                prev = prev->next_;
                current = current->next_;
            }
            newString->s_ = s;
            newString->next_ = current;
            prev->next_ = newString;
        }
        size_++;
    }
}

void BucketList::remove(const string& s) {
	// IMPLEMENT ME
    if (!contains(s)) return;
    int loc = h(s) % getCap();
    Node* prev = buckets_[loc];
    Node* current = prev->next_;
    // cout << contains(s);

    if (prev->s_ == s ) {
        delete prev;
        buckets_[loc] = current;
        size_--;
        return;
    } else {
        while(current != nullptr) {
            if (current->s_ == s) {
                prev->next_ = current->next_;
                delete current;
                // current = nullptr;
                size_--;
                return;
            } else {
                current = current->next_;
                prev = prev->next_;
            }
        }
}
}

string BucketList::toString() const {
	// IMPLEMENT ME
    string s;
    for (int j = 0; j < cap_; j++) {
        Node* temp = buckets_[j];
        if (temp == nullptr) { 
            s.append(to_string(j));
        } else {
            s.append(to_string(j) + " ");
            while (temp != nullptr) {
                s.append(temp->s_);
                if (temp->next_ != nullptr) {
                    s.append(" ");
                }
                temp = temp->next_;
            }
        }
        if (j < cap_-1) s.append("\n");
    }
	return s; // dummy
}

int BucketList::h(const string& s) const {
	// IMPLEMENT ME
    int temp = 0;
    if ((int)s.size() == 0) { return 0;}
    for (int i=0; i < (int)s.size()  ;i++) {
        temp = ((int)pow(2, i))*((int)s[i]) + temp;
    }
	return temp; // dummy
}

int BucketList::getSize() const {
	// IMPLEMENT ME
	return size_; // dummy
}

int BucketList::getCap() const {
	// IMPLEMENT ME
	return cap_; // dummy
}
