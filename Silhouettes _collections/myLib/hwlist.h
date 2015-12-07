#ifndef HWLIST_H
#define HWLIST_H
#include <iostream>
#include <stdlib.h>
#include <string>


template<class T>
class HWList{
    //Data of Class List
private:
    struct Node{
        T value;
        Node *next;
        Node *previous;

        Node(T value)
            :value(value), next(0), previous(0){}
    };

    Node* head;
    Node* tail;
    int count;

    //get index of list's node
    int getIndex(Node * ptr){
        int index = 0;
        while (ptr->previous != 0) {
            ptr = ptr->previous;
            ++index;
        }
        return index;
    }

public:

    //Constructor
    HWList(){
        head = 0;
        tail = 0;
        count = 0;
    }


    //Destructor
    ~HWList(){
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
    void add_top(T value){
        Node* n = new Node(value);
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

    void add_back(T value){
        Node* n = new Node(value);
        if (head == 0) {
            head = n;
            tail = n;
        }
        else{
            n->previous = tail;
            tail->next = n;
            tail = n;
        }

        ++count;

    }


    //Copy
    HWList(HWList const & list){
        head = 0;
        tail = 0;
        count = 0;
        Node * current = list.head;
        for(int i = 0; i < list.count; ++i){
            add_back(current->value);
            current = current->next;
        }
    }

    //Operator=
    HWList & operator=(HWList const & list){
        if(this != &list){
            clear();
            head = 0;
            tail = 0;
            count = 0;
            Node * current = list.head;
            for(int i = 0; i < list.count; ++i){
                add_back(current->value);
                current = current->next;
            }

        }
        return *this;
    }

    //Method isEqual
    bool isEqual(HWList const & list){
        if(this->count != list.count)
            return false;

        for(c_iterator lIt = c_begin(), rIt = list.c_begin(); lIt != c_end(); ++lIt, ++rIt){
            if(*lIt != *rIt)
                return false;
        }
        return true;
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


    //set method
    void set(int index, T value){
        iterator it = begin();
        Node * current = it.get(index);
        if (current == 0){
            std::cerr << "You try set value to nonexistent element. The program will be clossed";
            exit (EXIT_FAILURE);
        }
        current->value = value;
    }


    //insert method
    void insert(int index, T value){
        if (index == 0){
            add_top(value);
            return;
        }
        if (index >= (count - 1)){
            add_back(value);
            return;
        }
        iterator it = begin();
        Node * current = it.get(index);
        Node * insertNode = new Node(value);
        insertNode->previous = current->previous;
        current->previous->next = insertNode;
        current->previous = insertNode;
        insertNode->next = current;
        ++count;
    }

    //Deleting element==================================================================================================
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


    //method remove
    void remove(int index){
        if (index == 0){
            pop_top();
            return;
        }
        if (index == (count - 1)){
            pop_back();
            return;
        }
        iterator it = begin();
        Node * current = it.get(index);
        if (current != 0){
            current->previous->next = current->next;
            current->next->previous = current->previous;
            delete  current;
            --count;
        }
    }


    //deleting all elements
    void clear(){
        while (count > 0) {
            pop_back();
        }
    }

    //=========================================================================================================

    //Return sublist from list
    HWList & subList(int index, int length){
        if ((length + index) > count){
            length = count - index;
        }
        HWList<T> * subL = new HWList<T>;
        iterator it = begin();
        it.get(index);
        for (int i = 0; i < length; ++i){
            subL->add_back(it.get()->value);
            ++it;
        }
        return *subL;

    }



    //method sort
    HWList & sort(){
        iterator it = begin();
        ++it;
        for(iterator j = it; j != end(); ++j){
            iterator sortIt = j;
            iterator i = --j;
            if (*i <= *sortIt){
                continue;
            }
            while ((i != begin()) && (*i > *sortIt)) {
                --i;
            }
            if (*i < *sortIt){
                ++i;
            }
            T value = *sortIt;
            remove(getIndex(sortIt.get()));
            insert(getIndex(i.get()), value);

        }
        return *this;
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


    //Operators=================================================================================================================

    HWList & operator+=(HWList const & list){
        for(c_iterator it = list.c_begin(); it != list.c_end(); ++it){
            add_back(*it);
        }
        return *this;
    }

    HWList & operator+=(T value){
        add_back(value);
        return *this;
    }

    T & operator[](int i){
        return get(i);
    }

    bool operator==(HWList const & list){
        return isEqual(list);
    }

    bool operator!=(HWList const & list){
        return !isEqual(list);
    }

};

template<class T>
std::istream& operator>>(std::istream & stream, HWList<T> & list){
    for (int i = 0; i < list.size(); ++i){
        stream >> list.get(i);
    }
    return stream;
}

template<class T>
std::ostream& operator<<(std::ostream & stream, HWList<T> & list){
    stream << "(";
    for (int i = 0; i < list.size() - 1; ++i){
        stream << list.get(i) << ", ";
    }
    stream << list.get(list.size() - 1) << ")";
    return stream;
}

template<class T>
HWList<T> operator+(HWList<T> list1, HWList<T> const & list2){
    return list1 += list2;
}

#endif // HWLIST_H
