/**
 * @file CircularBuffer.h
 * @author Daniel Kim
 * @brief Circular buffer 
 * @version 0.1
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022 Daniel Kim
 * 
 */

#ifndef CircularBuffer_h
#define CircularBuffer_h


#include <mutex>
#include <memory>
#include <thread>
#include <type_traits>

template <class T, bool is_static, size_t size>
class CircularBuffer
{
public:
    CircularBuffer()
	{
		if(!is_static)
		{
			buf_ = std::unique_ptr<T[]>(new T[size]);
		}
		max_size_ = size;
	}
    
    void insert(T item);
    T get();
    void reset();
    bool empty() const;
    bool full() const;
    size_t capacity() const;
    size_t get_size() const;

private:
	using buffer_type = std::conditional_t<is_static, T[size], std::unique_ptr<T[]>>;
	buffer_type buf_;

	size_t head_ = 0;
	size_t tail_ = 0;
	size_t max_size_;
	bool full_ = 0;
};


/**
 * @brief Resets the circular buffer
 * 
 * @tparam T type of the buffer
 */
template <class T, bool is_static, size_t size>
void CircularBuffer<T, is_static, size>::reset()
{
    head_ = tail_;
    full_ = false;
}

/**
 * @brief Empties the circular buffer
 * 
 * @tparam T data type
 * @return true the buffer is empty
 * @return false the buffer is not empty
 */
template <class T, bool is_static, size_t size>
bool CircularBuffer<T, is_static, size>::empty() const
{
	//if head and tail are equal, we are empty
	return (!full_ && (head_ == tail_));
}

/**
 * @brief Check to see if the buffer is full or not
 * 
 * @tparam T data type
 * @return true buffer is full
 * @return false buffer is not full
 */
template <class T, bool is_static, size_t size>
bool CircularBuffer<T, is_static, size>::full() const
{
	//If tail is ahead the head by 1, we are full
	return full_;
}

/**
 * @brief returns the capacity of the buffer
 * 
 * @tparam T data type
 * @return size_t capacity
 */
template <class T, bool is_static, size_t size>
size_t CircularBuffer<T, is_static, size>::capacity() const
{
	return max_size_;
}

/**
 * @brief current size of the buffer (different from capacity in that if an element
 * of the buffer is retrieved it returns the new size)
 * 
 * @tparam T data type
 * @return size_t size of the current buffer
 */
template <class T, bool is_static, size_t size>
size_t CircularBuffer<T, is_static, size>::get_size() const
{
	size_t size_ = max_size_;

	if(!full_)
	{
		if(head_ >= tail_)
		{
			size = head_ - tail_;
		}
		else
		{
			size = max_size_ + head_ - tail_;
		}
	}

	return size;
}

/**
 * @brief Inserts data into the buffer
 * 
 * @tparam T data type
 * @param item the data to be inserted
 */
template <class T, bool is_static, size_t size>
void CircularBuffer<T, is_static, size>::insert(T item)
{
	buf_[head_] = item;

	if(full_)
	{
		tail_ = (tail_ + 1) % max_size_;
	}

	head_ = (head_ + 1) % max_size_;

	full_ = head_ == tail_;
}

/**
 * @brief retrieves an element of the biffer
 * 
 * @tparam T class data type 
 * @return T data type within the buffer
 */
template <class T, bool is_static, size_t size>
T CircularBuffer<T, is_static, size>::get()
{
	if(empty())
	{
		return T();
	}

	//Read data and advance the tail (we now have a free space)
	auto val = buf_[tail_];
	full_ = false;
	tail_ = (tail_ + 1) % max_size_;

	return val;
}


#endif