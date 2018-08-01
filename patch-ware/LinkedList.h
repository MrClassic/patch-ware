/* ************************************************************************
 * File:   LinkedList.h
 * Author: Nate Gallegos
 *
 * Log:
 *      1/17/18
 *      File Created.
 *      Basic list functionality implemented.
 *      2/28/18
 *      Implemented size member and accessor.
 ************************************************************************** */

#ifndef NULL
#define NULL 0x0
#endif

#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

template <typename T>
class LinkedList{
    public:
    //default constructor    
    LinkedList(){
        head = new Node;
        head->data = NULL;
        head->next = NULL;
        tail = head;
        size = 0;
    }
    
    //initializing constructor
    LinkedList(T *data){
        head = new Node;
        head->data = NULL;
        head->next = new Node;
        tail = head->next;
        tail->data = data;
        size = 1;
    }
    
    //shallow copy
    LinkedList(const LinkedList<T> &rhs){
        head = tail = NULL;
        copy(rhs);
    }
    
    //shallow delete
    ~LinkedList(){
        clear();
    }
    
    //clears list. Boolean flag for deleting data pointed to.
    void clear(bool deleteData = false){
        Node *curr = head;
        while(curr != NULL){
            Node *n = curr;
            curr = curr->next;
            if(deleteData){
                delete n->data;
            }
            delete n;
            n = NULL;
        }
        head = NULL;
        tail = NULL;
    }
    
    //copy list, deepCopy boolean for deep copying
    void copy(const LinkedList &list, bool deepCopy = false){
        if(head != NULL){
            clear();
        }
        head = new Node;
        head->data = NULL;
        head->next = NULL;
        tail = head;
        Node *theirs = list.head->next;
        while(theirs != NULL){
            tail->next = new Node;
            tail = tail->next;
            tail->next = NULL;
            if(!deepCopy){
                tail->data = theirs->data;
            }
            theirs = theirs->next;
        }
        size = list.getSize();
    }

	//remove the parameter from the list
	bool remove(T* const data, bool deepRemove = false) {
		Node* temp = head;
		while (temp->next != NULL) {
			if (temp->next->data == data) {
				Node* remove = temp->next;
				temp->next = remove->next;
				if (remove == tail) {
					tail = temp;
				}
				if (deepRemove) {
					delete remove->data;
				}
				delete remove;
				return true;
			}
			temp = temp->next;
		}
		return false;
	}

	//removes all elements from the parameter list from this list.
	//returns true if any removes were made. False if none were made
	bool remove(LinkedList<T> &list, bool deepRemove = false) {
		bool result = false;
		Node* temp = head;
		while (temp->next != NULL) {
			LinkedList<T> copy = list;
			while (!copy.isEmpty()) {
				T* pop = copy.pop_front();
				if (pop == NULL) {
					continue;
				}
				if (temp->next->data == pop) {
					Node* remove = temp->next;
					temp->next = remove->next;
					if (remove == tail) {
						tail = temp;
					}
					if (deepRemove) {
						delete remove->data;
					}
					delete remove;
					result = true;
				}
				temp = temp->next;
			}
		}
		return result;
	}
    
    //add to the front of the list
    void push_front(T * const data){
        Node *temp = head->next;
        head->next = new Node;
        head->next->next = temp;
        temp = head->next;
        temp->data = data;
        if(head == tail){
            tail = head->next;
        }
        size++;
    }
    
    //add to end of the list
    void push_back(T * const data){
        tail->next = new Node;
        tail = tail->next;
        tail->data = data;
        tail->next = NULL;
        size++;
    }
    
    //pop first element in list
    T *pop_front(){
        if(isEmpty()){
            return NULL;
        }
        if(tail == head->next){
            tail = head;
        }
        Node *temp = head->next->next;
        T* out = head->next->data;
        delete head->next;
        head->next = temp;
        size--;
        return out;
    }
    
    //pop last element of list
    T* pop_back(){
        if(isEmpty()){
            return NULL;
        }
        Node *curr = head->next;
        if(curr == tail){
            tail = head;
            T* out = curr->data;
            head->next = NULL;
            delete curr;
            return out;
        }
        while(curr->next != tail){
            curr = curr->next;
        }
        curr->next = NULL;
        T *out = tail->data;
        delete tail;
        tail = curr;
        size--;
        return out;
    }
    
    //access "empty" property
    bool isEmpty() const {
        return head->next == NULL;
    }
    
    //apply function to data until function returns false
    bool apply(bool function(T* data, void *arg), void *arg) const {
        Node *curr = head->next;
        while(curr != NULL){
            if(!function(curr->data, arg)){
                return false;
            }
            curr = curr->next;
        }
        return true;
    }
    
    //accessor for List size
    int getSize() const{
        return size;
    }
    
    //assignment operator
    LinkedList &operator=(const LinkedList &rhs){
        if(this != &rhs){
            copy(rhs);
        }
        return *this;
    }
    
    //returns a sum of both lists
    LinkedList operator+(const LinkedList &rhs) const{
        LinkedList<T> out;
        out.copy(*this);
        Node *curr = rhs.head->next;
        while(curr != NULL){
            out.push_back(curr->data);
            curr = curr->next;
        }
        return out;
    }
    
private:
    struct Node{
        Node *next;
        T *data;
    };
    
    Node* head;
    Node* tail;
    int size;
};


#endif	/* LINKEDLIST_H */

