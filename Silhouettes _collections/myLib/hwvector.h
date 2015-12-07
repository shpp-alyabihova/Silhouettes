#ifndef HWVECTOR
#define HWVECTOR
#include <string>
#include <iostream>


template<class T>
class HWVector{
private:
    void resize(){
        vecCapacity *= 2;
        T * newStart = new T [vecCapacity];
        for (int i = 0; i < vecSize; ++i){
            newStart[i] = start[i];
        }
        delete [] start;
        start = newStart;
    }

    bool isFull(){
        return vecSize == vecCapacity;
    }

    T * start;
    int vecSize;
    int vecCapacity;

public:
    class c_iterator{
    private:
        const T * ptr;

    public:
        c_iterator(){}
        c_iterator(T * ptrVec) : ptr(ptrVec){}

        T operator*(){
            return *ptr;
        }

        T * operator->(){
            return ptr;
        }

        c_iterator & operator++(){
            ++ptr;
            return *this;
        }

        c_iterator & operator++(int){
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        c_iterator & operator--(){
            --ptr;
            return *this;
        }

        c_iterator & operator--(int){
            iterator tmp = *this;
            --ptr;
            return tmp;
        }

        bool operator==(c_iterator const it){
            return ptr == it.ptr;
        }

        bool operator!=(c_iterator const it){
            return ptr != it.ptr;
        }

    };


    class iterator{
    private:
        T * ptr;

    public:
        iterator(){}
        iterator(T * ptrVec) : ptr(ptrVec){}

        T & operator*(){
            return *ptr;
        }

        T * operator->(){
            return ptr;
        }

        iterator & operator++(){
            ++ptr;
            return *this;
        }

        iterator & operator++(int){
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        iterator & operator--(){
            --ptr;
            return *this;
        }

        iterator & operator--(int){
            iterator tmp = *this;
            --ptr;
            return tmp;
        }

        bool operator==(iterator it){
            return ptr == it.ptr;
        }

        bool operator!=(iterator it){
            return ptr != it.ptr;
        }

    };

    // Constructors ======================================================================================================
    HWVector()
        : vecCapacity(1), vecSize(0){
        start = new T[vecCapacity];
    }
    HWVector(int size)
        : vecSize(size), vecCapacity(size){
        start = new T[vecCapacity];
    }
    HWVector(int size, T value)
        : vecSize(size), vecCapacity(size){
        start = new T[vecCapacity];
        for(int i = 0; i < size; ++i){
            start[i] = value;
        }
    }

    ~HWVector(){
        delete [] start;
    }

    // Copy constructor ==================================================================================================
    HWVector (HWVector const & vector)
        : vecSize(vector.vecSize), vecCapacity(vector.vecCapacity), start(new T[vecCapacity]){
        for (int i = 0; i < vecSize; ++i){
            start[i] = vector.start[i];
        }
    }


    //Operator=
    HWVector & operator=(HWVector const & vector){
        if (start != vector.start){
            delete [] start;
            vecSize = vector.vecSize;
            vecCapacity = vector.vecCapacity;
            start = new T[vecCapacity];
            for(int i = 0; i < vecSize; ++i){
                start[i] = vector.start[i];
            }
        }
        return *this;
    }

    //SubVector
    HWVector & subList(int index, int length){
        HWVector<T> *subVec = new HWVector<T>(length);
        for (int i = 0; i < length; ++i){
            subVec->start[i] = start[index];
            ++index;
        }
        return *subVec;
    }


    // adding elements ====================================================================================================
    void add(T value){
        if(isFull()){
            resize();
        }
        start[vecSize] = value;
        ++vecSize;

    }

    //insert method
    void insert(int index, T value){
        if(isFull()){
            resize();
        }
        int tmpSize = vecSize - index;
        T * tmp = new T [tmpSize];
        for(int i = 0, j = index; i < tmpSize; ++i, ++j){
            tmp[i] = start[j];
        }
        start[index] = value;
        ++vecSize;
        ++index;
        for(int i = index, j = 0; i < vecSize; ++i, ++j){
            start[i] = tmp[j];
        }
        delete [] tmp;

    }


    // deleting elements ===================================================================================================
    void pop_back(){
        --vecSize;
    }

    void clear(){
        vecSize = 0;
    }

    void remove(int index){
        int j = index;
        for(int i = ++index; i < vecSize; ++i){
            start[j] = start[i];
            ++j;
        }
        --vecSize;
    }

    // Determination of vector parameters ==================================================================================
    int size(){
        return vecSize;
    }

    int capacity(){
        return vecCapacity;
    }

    // isEqual
    bool isEqual(HWVector const & vector){
        if(vecSize != vector.vecSize)
            return false;
        for(int i = 0, j = 0; i < vecSize; ++i){
            if(start[i] != vector.start[j])
                return false;
        }
        return true;
    }

    //isEmpty
    bool isEmpty(){
        return !vecSize;
    }

    //Operators=================================================================================================
    T & operator[](int i){
        return start[i];
    }

    HWVector & operator+=(HWVector const & vec){
        for(c_iterator it = vec.begin(); it != vec.end(); ++it){
            add(*it);
        }
        return *this;
    }

    HWVector & operator+=(T value){
        add(value);
        return *this;
    }


    HWVector & operator+(HWVector const & vec){
        for(c_iterator it = vec.begin(); it != vec.end(); ++it){
            add(*it);
        }
        return *this;
    }


    bool operator==(HWVector const & vec){
        return isEqual(vec);
    }

    bool operator!=(HWVector const & vec){
        return !isEqual(vec);
    }

    //Iterators================================================================================================
    iterator begin(){
        return iterator(start);
    }

    iterator end(){
        return iterator(start + vecSize);
    }

    c_iterator begin() const{
        return c_iterator(start);
    }

    c_iterator end() const{
        return c_iterator(start + vecSize);
    }

    //========================================================================================================

};

template<class T>
std::istream& operator>>(std::istream & stream, HWVector<T> & vec){
    for (int i = 0; i < vec.size(); ++i){
        stream >> vec[i];
    }
    return stream;
}

template<class T>
std::ostream& operator<<(std::ostream & stream, HWVector<T> & vec){
    stream << "[";
    for (int i = 0; i < vec.size() - 1; ++i){
        stream << vec[i] << ", ";
    }
    stream << vec[vec.size() - 1] << "]";
    return stream;
}

/*
//??????????????????????????????????????????????????????????????????????????????
template<class T>
HWVector<T> operator+(HWVector<T> vec1, HWVector<T> const & vec2){
    return vec1 += vec2;
}
*/

#endif // HWVECTOR

