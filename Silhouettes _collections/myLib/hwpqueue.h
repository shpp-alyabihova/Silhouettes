#ifndef HWPQUEUE
#define HWPQUEUE


#include <iostream>
#include <stdlib.h>
#include <string>


template<class T>
class HWPriorityQueue{
    //Data of Class List
private:
    struct Node{
        T value;
        int priority;
        Node *next;
        Node *previous;

        Node(T value, int priority)
            :value(value), priority(priority), next(0), previous(0){}
    };

    Node* head;
    Node* tail;
    int count;
public:

    //Constructor
    HWPriorityQueue(){
        head = 0;
        tail = 0;
        count = 0;
    }


    //Destructor
    ~HWPriorityQueue(){
        clear();
    }



    class c_iterator{
    private:
        const Node * itPtr;
    public:
        c_iterator()
            : itPtr(0){}
        c_iterator (Node *n)
            : itPtr(n){}


        const Node * get(int index = 0){
            for (int i = 0; i < index; ++i){
                if (itPtr->next != 0){
                    itPtr = itPtr->next;
                }
                else
                    return itPtr->next;
            }
            return itPtr;
        }


        T operator*(){
            return itPtr->value;
        }

        c_iterator & operator++(){
            itPtr = itPtr->next;
            return *this;
        }

        c_iterator & operator++(int){
            c_iterator tmp = *this;
            itPtr = itPtr->next;
            return tmp;
        }

        c_iterator & operator--(){
            itPtr = itPtr->previous;
            return *this;
        }

        c_iterator & operator--(int){
            c_iterator tmp = *this;
            itPtr = itPtr->previous;
            return tmp;
        }

        bool operator==(c_iterator const & it){
            return itPtr == it.itPtr;
        }

        bool operator!=(c_iterator const & it){
            return itPtr != it.itPtr;
        }
    };



    class iterator{
    private:
        Node * itPtr;
    public:
        iterator()
            : itPtr(0){}
        iterator(Node * n)
            : itPtr(n){}


        Node * get(int index = 0){
            for (int i = 0; i < index; ++i){
                if (itPtr->next != 0){
                    itPtr = itPtr->next;
                }
                else
                    return itPtr->next;
            }
            return itPtr;
        }


        T & operator*(){
            return itPtr->value;
        }

        T * operator->(){
            return itPtr->value;
        }

        iterator & operator++(){
            itPtr = itPtr->next;
            return *this;
        }

        iterator & operator++(int){
            iterator tmp = *this;
            itPtr = itPtr->next;
            return tmp;
        }

        iterator & operator--(){
            itPtr = itPtr->previous;
            return *this;
        }

        iterator & operator--(int){
            iterator tmp = *this;
            itPtr = itPtr->previous;
            return tmp;
        }

        bool operator==(iterator const & it){
            return itPtr == it.itPtr;
        }

        bool operator!=(iterator const & it){
            return itPtr != it.itPtr;
        }
    };

    //Adding element===============================================================================================

    void enqueue(T value, int priority){
          Node* n = new Node(value, priority);
          ++count;

          if (head == 0) {
              head = n;
              tail = n;
          }
          else{
              n->previous = tail;
              tail->next = n;
              tail = n;
          }
      }



    //Copy
    HWPriorityQueue(HWPriorityQueue const & list){
        head = 0;
        tail = 0;
        count = 0;
        Node * current = list.head;
        for(int i = 0; i < list.count; ++i){
            enqueue(current->value, current->priority);
            current = current->next;
        }
    }

    //Operator=
    HWPriorityQueue & operator=(HWPriorityQueue const & list){
        if(this != &list){
            clear();
            head = 0;
            tail = 0;
            count = 0;
            Node * current = list.head;
            for(int i = 0; i < list.count; ++i){
                enqueue(current->value, current->priority);
                current = current->next;
            }

        }
        return *this;
    }

    //Method isEqual
    bool isEqual(HWPriorityQueue const & list){
        if(this->count != list.count)
            return false;

        for(c_iterator lIt = c_begin(), rIt = list.c_begin(); lIt != c_end(); ++lIt, ++rIt){
            if((*lIt != *rIt) || (lIt.get()->priority != rIt.get()->priority))
                return false;
        }
        return true;
    }


    //get method
    T & front(){
        return head->value;
    }

    T & peek(){
        int index = highestPriority();
        iterator it = begin();
        it.get(index);
        return *it;
    }

    int peekPriority(){
        return head->priority;
    }
    //Deleting element==================================================================================================
    T & dequeue(){
        if(count == 0){
            std::cerr << "You try delete nonexistent element. The program will be clossed";
            exit (EXIT_FAILURE);
        }

        return remove(highestPriority());
    }



    void clear(){
        while (count > 0) {
            pop_back();
        }
    }
    //Determination  size=================================================================================================
    int size(){
        return count;
    }


    bool isEmpty(){
        return count == 0;
    }

    //Iterators ============================================================================================================

    iterator begin(){
        return iterator(head);
    }

    iterator end() {
        return iterator(tail->next);
    }

    c_iterator c_begin() const{
        return c_iterator(head);
    }

    c_iterator c_end() const{
        return c_iterator(tail->next);
    }

    //get method
    T & get(int index){
        iterator it = begin();
        if (it.get(index) == 0){
            std::cerr << "You try get nonexistent element. The program will be clossed";
            exit (EXIT_FAILURE);
        }
        return *it;
    }



    T & operator[](int i){
        return get(i);
    }

    bool operator==(HWPriorityQueue const & list){
        return isEqual(list);
    }

    bool operator!=(HWPriorityQueue const & list){
        return !isEqual(list);
    }


private:
    int highestPriority(){
        Node * current = head;
        iterator i = begin();
        int priority = i.get()->priority;
        for (; i != end(); ++i){
            if (i.get()->priority < priority){
                current = i.get();
                priority = i.get()->priority;
            }
        }
        int index = getIndex(current);
        return index;
    }



    //method sort
    HWPriorityQueue & sort(){
        iterator it = begin();
        ++it;
        for(iterator j = it; j != end(); ++j){
            iterator sortIt = j;
            iterator i = j;
            --i;
            if (i.get()->priority <= sortIt.get()->priority){
                continue;
            }
            while ((i != begin()) && (i.get()->priority > sortIt.get()->priority)) {
                --i;
            }
            if ((i == begin()) && (i.get()->priority < sortIt.get()->priority)){
                ++i;
            }
            T value = *sortIt;
            int priority = sortIt.get()->priority;
            remove(getIndex(sortIt.get()));
            insert(getIndex(i.get()), value, priority);

        }
        return *this;
    }

    void pop_top(){
            if(count == 0){
                std::cerr << "You try delete nonexistent element. The program will be clossed";
                exit (EXIT_FAILURE);
            }
            if(count == 1){
                delete head;
                tail = 0;
                head = 0;
            }
            else{
                Node *temp = head;
                head = head->next;
                head->previous = 0;
                delete temp;
            }
            --count;
        }

    void pop_back(){
        if(count == 0){
            std::cerr << "You try delete nonexistent element. The program will be clossed";
            exit (EXIT_FAILURE);
        }
        if(count == 1){
            delete head;
            tail = 0;
            head = 0;
        }
        else{
            Node *temp = tail;
            tail = tail->previous;
            tail->next = 0;
            delete temp;
        }
        --count;
    }


    //insert method
    void insert(int index, T value, int priority){
        if (index == 0){
            add_top(value, priority);
            return;
        }
        if (index >= (count - 1)){
            add_back(value, priority);
            return;
        }

        iterator it = begin();
        Node * current = it.get(index);
        Node * insertNode = new Node(value, priority);
        insertNode->previous = current->previous;
        current->previous->next = insertNode;
        current->previous = insertNode;
        insertNode->next = current;
        ++count;
    }


    void add_top(T value, int priority){
            Node* n = new Node(value, priority);
            if (head == 0) {
                head = n;
                tail = n;
            }
            else{
                n->next = head;
                head->previous = n;
                head = n;
            }

            ++count;
        }

    void add_back(T value, int priority){
        Node* n = new Node(value, priority);
         ++count;
        if (head == 0) {
            head = n;
            tail = n;
        }
        else{
            n->previous = tail;
            tail->next = n;
            tail = n;
        }



    }



    int getIndex(Node * ptr){
        int index = 0;
        while (ptr->previous != 0) {
            ptr = ptr->previous;
            ++index;
        }
        return index;
    }

    //method remove
    T & remove(int index){
        T value;
        if (index == 0){
            value = head->value;
            pop_top();
            return value;
        }
        if (index == (count - 1)){
            value = tail->value;
            pop_back();
            return value;
        }
        iterator it = begin();
        Node * current = it.get(index);
        value = current->value;
        if (current != 0){
            current->previous->next = current->next;
            current->next->previous = current->previous;
            delete  current;
            --count;
        }
     return value;
    }

};


    template<class T>
    std::istream& operator>>(std::istream & stream, HWPriorityQueue<T> & list){
        for (int i = 0; i < list.size(); ++i){
            stream >> list.get(i);
        }
        return stream;
    }

    template<class T>
    std::ostream& operator<<(std::ostream & stream, HWPriorityQueue<T> & list){
        stream << "(";
        for (int i = 0; i < list.size() - 1; ++i){
            stream << list.get(i) << ", ";
        }
        stream << list.get(list.size() - 1) << ")";
        return stream;
    }



#endif // HWPQUEUE

