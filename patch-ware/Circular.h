/* ************************************************************************
 * File:   Circular.h
 * Author: Nate Gallegos
 *
 * Log:
 *      2/27/18
 *      File Created.
 *      Basic list functionality implemented.
 *		5/3/19
 *		Happy Birthday Micah!!!
 *		Revised the interface for popping (queue and stack)
 *		Fixed bugs involved with popping out of order.
 *		Re-did the functionality of operators = and << (for both)
 *		Added count variable for tracking size,
 *			Now the queue and stack will not pop if emptied.
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
		_count = 0;
	}

	circular_stack(int size) {
		data = new T[size];
		_size = size;
		start = 0;
		_count = 0;
	}

	circular_stack(const circular_stack<T> &orig) {
		_size = orig._size;
		data = new T[_size];
		start = 0;
		for (int i = 0; i < _size; i++) {
			data[i] = orig[i];
		}
		_count = orig._count;
	}

	~circular_stack() {
		delete[] data;
	}

	void push(T t) {
		start--;
		forceIndex(start);
		data[start] = t;
		if (_count < _size) {
			_count++;
		}
	}

	bool pop(T &t) {
		if (_count > 0) {
			t = data[start++];
			forceIndex(start);
			_count--;
			return true;
		}
		else {
			return false;
		}
	}

	int size() const {
		return _size;
	}

	int count() const {
		return _count;
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
			data[i] = rhs.data[i];
		}
		start = rhs.start;
		_count = rhs._count;
		return *this;
	}

	circular_stack& operator<<(const circular_stack &rhs) {
		for (int i = 0; i < rhs.count(); i++) {
			push(rhs[0 - (i + 1)]);
		}
		return *this;
	}

private:
	int _size;
	T* data;
	int start;
	int _count;

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
		_count = 0;
	}

	circular_queue(int size) {
		data = new T[size];
		_size = size;
		start = 0;
		filler = 0;
		_count = 0;
	}

	circular_queue(int size, T init) {
		data = new T[size];
		_size = size;
		start = 0;
		filler = 0;
		_count = 0;
		for (int i = 0; i < size; i++)data[i] = init;
	};

	~circular_queue() {
		delete[] data;
	}

	void push(T t) {
		data[filler] = t;
		filler++;
		forceIndex(filler);
		if (_count < _size) {
			++_count;
		}
		else {
			start++;
			forceIndex(start);
		}
	}

	bool pop(T &t) {
		if (_count > 0) {
			t = data[start++];
			forceIndex(start);
			_count--;
			return true;
		}
		else {
			return false;
		}
	}

	int size() const {
		return _size;
	}

	int count() const {
		return _count;
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
			data[i] = rhs.data[i];
		}
		filler = rhs.filler;
		start = rhs.start;
		_count = rhs._count;
		return *this;

	}

	circular_queue& operator<<(const circular_queue &rhs) {
		for (int i = 0; i < rhs.count(); i++) {
			push(rhs[i]);
		}
		return *this;
	}

private:
	int _size;
	T* data;
	int start;
	int filler;
	int _count;

	void forceIndex(int &i) const {
		while (i < 0)
			i += _size;
		i = i % _size;
	}
};

#endif