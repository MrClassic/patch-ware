/* ************************************************************************
 * File:   Circular.h
 * Author: Nate Gallegos
 *
 * Log:
 *      2/27/18
 *      File Created.
 *      Basic list functionality implemented.
 ************************************************************************** */

#ifndef CIRCULAR_H
#define CIRCULAR_H

/* *************************************************************************
 *  Class:  Circular Stack
 *  Purpose:    Models a stack implemented ring buffer. The buffer is a fixed
 *              size upon initialization. Continuous pushes overwrite the old
 *              data.
 ************************************************************************* */
template<class T>
struct circular_stack {
public:

    circular_stack() {
        _size = 1;
        data = new T[1];
        start = 0;
    }

    circular_stack(int size) {
        data = new T[size];
        _size = size;
        start = 0;
    }

    circular_stack(const circular_stack<T> &orig) {
        _size = orig._size;
        data = new T[_size];
        start = 0;
        for (int i = 0; i < _size; i++) {
            data[i] = orig[i];
        }
    }

    ~circular_stack() {
        delete[] data;
    }

    void push(T t) {
        start--;
        forceIndex(start);
        data[start] = t;
    }

    T pop() {
        int index = start;
        start++;
        return data[index];
    }

    int size() const {
        return _size;
    }

    T& operator[](int index) const {
        index += start;
        forceIndex(index);
        return data[index];
    }

    circular_stack& operator=(const circular_stack &rhs) {
        if (data != NULL) {
            delete data;
        }
        _size = rhs._size;
        data = new T[_size];
        for (int i = 0; i < _size; i++) {
            data[i] = rhs[i];
        }
		return *this;
    }
private:
    int _size;
    T* data;
    int start;

    void forceIndex(int &i) const {
        while (i < 0)
            i += _size;
        i = i % _size;
    }
};

/* *************************************************************************
 *  Class:  Circular Queue
 *  Purpose:    Models a queue implemented ring buffer. The buffer is a fixed
 *              size upon initialization. Continuous pushes overwrite the old
 *              data.
 ************************************************************************* */
template<class T>
struct circular_queue {
public:

    circular_queue() {
        _size = 0;
        data = new T[1];
        start = 0;
        filler = 0;
    }

    circular_queue(int size) {
        data = new T[size];
        _size = size;
        start = 0;
        filler = 0;
    }

    circular_queue(int size, T init) {
        data = new T[size];
        _size = size;
        start = 0;
        filler = 0;
        for (int i = 0; i < size; i++)data[i] = init;
    };

    ~circular_queue() {
        delete[] data;
    }

    void push(T t) {
        filler++;
        forceIndex(filler);
        data[filler] = t;
        if (filler == start) {
            start++;
            forceIndex(start);
        }
    }

    T pop() {
        int index = start;
        start++;
        return data[index];
    }

    int size() const {
        return _size;
    }

    T& operator[](int index) const {
        index += start;
        forceIndex(index);
        return data[index];
    }

    circular_queue& operator=(const circular_queue &rhs) {
        if (data != NULL) {
            delete data;
        }
        _size = rhs._size;
        data = new T[_size];
        for (int i = 0; i < _size; i++) {
            data[i] = rhs[i];
        }
		return *this;
    }
private:
    int _size;
    T* data;
    int start;
    int filler;

    void forceIndex(int &i) const {
        while (i < 0)
            i += _size;
        i = i % _size;
    }
};

#endif