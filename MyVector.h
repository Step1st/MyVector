
#ifndef MYVECTOR_MYVECTOR_H
#define MYVECTOR_MYVECTOR_H

#include <memory>
#include <iostream>
#include <algorithm>
#include <exception>


template<class T,class Allocator = std::allocator<T>>
        class MyVector {
public:
    typedef T* iterator;
    typedef  const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;

    MyVector();
    explicit MyVector(size_type n, const T& t = T{});
    explicit MyVector( size_type count );
    template <class InputIt>
    MyVector( InputIt first, InputIt last);
    MyVector(const MyVector& v);
    MyVector( std::initializer_list<T> init);
    ~MyVector();
    MyVector<T, Allocator>& operator=(const MyVector& v);
//    friend bool operator<( MyVector<T, Allocator> lhs, MyVector<T, Allocator> rhs );

    //Iterators:
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    //Capacity:
    size_type size() const { return avail_ptr - data_ptr; }
    size_type capacity() const { return limit_ptr - data_ptr; }
    size_type max_size() const {return (pow(2,64 - sizeof(value_type))) - 1; }
    void shrink_to_fit();
    void resize(size_type new_size);
    void resize(size_type new_size, const value_type& val);
    void reserve(size_type new_size);
    bool empty();

    //Element access:
    value_type& operator[](size_type i);
    const value_type& operator[](size_type i) const;
    value_type& at(size_type i);
    const value_type& at(size_type i) const;
    value_type& front();
    const value_type& front() const;
    value_type& back();
    const value_type& back() const;
    value_type* data();
    const value_type* data() const;

    //Modifiers:
    void push_back(const T& t);
    void pop_back();
    void assign(iterator first, iterator last);
    void assign (size_type n, const value_type& val);
    void assign (std::initializer_list<value_type> il);
    iterator insert (iterator position, value_type& val);
    iterator insert (iterator position, size_type n, const value_type& val);
    iterator insert (iterator position, iterator first, iterator last);
    iterator insert (iterator position, value_type&& val);
    iterator insert (const_iterator position, std::initializer_list<value_type> il);
    template<class... Args>
    iterator emplace (iterator position, Args&&... args);
    template<class... Args>
    void emplace_back (Args&&... args);
    iterator erase (iterator position);
    iterator erase (iterator first, iterator last);
    void swap (MyVector<T, Allocator>& x);
    void clear();


private:
    iterator data_ptr;
    iterator avail_ptr;
    iterator limit_ptr;

    Allocator alloc;

    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);
    void uncreate();
    void unchecked_append(const T& val);
    void unchecked_remove();
    void grow();
    void shrink();
    void re_size(size_type new_size);
    void new_size(size_type new_size);
};

//Public functions:

template<class T,class Allocator>
MyVector<T, Allocator>::MyVector() { create(); }

template<class T,class Allocator>
MyVector<T, Allocator>::MyVector(MyVector<T,Allocator>::size_type n, const T &t) { create(n, t); }

template<class T, class Allocator>
MyVector<T, Allocator>::MyVector(MyVector::size_type count) { create(count, T());}

template<class T, class Allocator>
template< class InputIt >
MyVector<T, Allocator>::MyVector( InputIt first, InputIt last) { create(first, last); }

template<class T,class Allocator>
MyVector<T, Allocator>::MyVector(const MyVector &v) { create(v.begin(), v.end()); }

template<class T,class Allocator>
MyVector<T, Allocator>::MyVector(std::initializer_list<T> init) { create(init.begin(), init.end()); }

template<class T,class Allocator>
MyVector<T, Allocator>::~MyVector() { uncreate(); }

template<class T,class Allocator>
MyVector<T, Allocator>& MyVector<T, Allocator>::operator=(const MyVector &v) {
    if (&v != this){
        uncreate();
        create(v.begin(), v.end());
    }
    return *this;
}

//iterators:
template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::begin() { return data_ptr; }

template<class T,class Allocator>
typename MyVector<T, Allocator>::const_iterator MyVector<T, Allocator>::begin() const { return data_ptr; }

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::end() { return avail_ptr; }

template<class T,class Allocator>
typename MyVector<T, Allocator>::const_iterator MyVector<T, Allocator>::end() const { return avail_ptr; }

//Capacity:
template<class T,class Allocator>
void MyVector<T, Allocator>::shrink_to_fit() {
    if(avail_ptr != limit_ptr)
        shrink();
}

template<class T,class Allocator>
void MyVector<T, Allocator>::resize(MyVector::size_type new_size) {
    if (size() != new_size){
        re_size(new_size);
    }
}

template<class T,class Allocator>
void MyVector<T, Allocator>::resize(MyVector::size_type new_size, const value_type &val) {
    if (size() != new_size){
        re_size(new_size);
        while(limit_ptr-avail_ptr)
        {
            unchecked_append(val);
        }
    }
}

template<class T,class Allocator>
void MyVector<T, Allocator>::reserve(MyVector::size_type new_size) {
    if (new_size > size())
        this->new_size(new_size);
}

template<class T,class Allocator>
bool MyVector<T, Allocator>::empty() {
    if (size())
        return false;
    else
        return true;
}

//Element access:

template<class T,class Allocator>
typename MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::operator[](MyVector::size_type i) { return data_ptr[i]; }

template<class T,class Allocator>
const typename MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::operator[](MyVector::size_type i) const { return data_ptr[i]; }

template<class T,class Allocator>
typename MyVector<T, Allocator>::value_type &MyVector<T, Allocator>::at(MyVector::size_type i) {
    if (i < this->size() && i > 0)
        return data_ptr[i];
    else
        throw std::out_of_range("element_out_of_range");
}

template<class T,class Allocator>
const typename  MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::at(MyVector::size_type i) const {
    if (i < this->size() && i > 0)
        return data_ptr[i];
    else
        throw std::out_of_range("element_out_of_range");
}
template<class T,class Allocator>
typename  MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::front(){ return *data_ptr; }

template<class T,class Allocator>
const typename  MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::front() const { return *data_ptr; }

template<class T,class Allocator>
typename  MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::back(){ return *(avail_ptr-1); }

template<class T,class Allocator>
const typename  MyVector<T, Allocator>::value_type& MyVector<T, Allocator>::back() const { return *(avail_ptr-1); }

template<class T,class Allocator>
typename  MyVector<T, Allocator>::value_type *MyVector<T, Allocator>::data() { return data_ptr; }

template<class T,class Allocator>
const typename  MyVector<T, Allocator>::value_type *MyVector<T, Allocator>::data() const { return data_ptr; }

//Modifiers:

template<class T,class Allocator>
void MyVector<T, Allocator>::push_back(const T &t) {
    if (avail_ptr == limit_ptr)
        grow();
    unchecked_append(t);
}

template<class T,class Allocator>
void MyVector<T, Allocator>::pop_back() {
    if (avail_ptr != data_ptr) {
        unchecked_remove();
    }
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::erase(MyVector::iterator position) {
    if (position == avail_ptr-1) {
        alloc.destroy(position);
        return --avail_ptr;
    }
    else {
        MyVector<T, Allocator> temp;
        int i = (position - data_ptr);
        std::copy(begin(), position, std::back_inserter(temp));
        std::copy(position + 1, avail_ptr, std::back_inserter(temp));
        size_type t = capacity();
        *this = temp;
        resize(t);
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::erase(MyVector::iterator first, MyVector::iterator last) {
    if (last == avail_ptr) {
        iterator fst = first;
        while (avail_ptr != fst)
            alloc.destroy(--avail_ptr);
        return avail_ptr;
    }
    else {
        MyVector<T, Allocator> temp;
        int i = (first - data_ptr);
        std::copy(begin(), first, std::back_inserter(temp));
        std::copy(last, avail_ptr, std::back_inserter(temp));
        size_type t = capacity();
        *this = temp;
        resize(t);
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
void MyVector<T, Allocator>::clear() {
        while (avail_ptr != data_ptr)
            alloc.destroy(--avail_ptr);
    }

template<class T,class Allocator>
void MyVector<T, Allocator>::swap(MyVector<T, Allocator> &x) {
    MyVector<T, Allocator> temp(std::move(x));
    x = std::move(*this);
    *this = std::move(temp);
}

template<class T,class Allocator>
void MyVector<T, Allocator>::assign(MyVector::iterator first, MyVector::iterator last) {
    if(data_ptr){
        while (avail_ptr != data_ptr)
            alloc.destroy(--avail_ptr);
        alloc.destroy(avail_ptr);
    }
    std::copy(first, last, std::back_inserter(*this));
}

template<class T,class Allocator>
void MyVector<T, Allocator>::assign(MyVector::size_type n, const value_type &val) {
    if(data_ptr){
        while (avail_ptr != data_ptr)
            alloc.destroy(--avail_ptr);
        alloc.destroy(avail_ptr);
    }
    for (int i = 0; i < n; i++) {
        this->push_back(val);
    }
}

template<class T,class Allocator>
void MyVector<T, Allocator>::assign(std::initializer_list<value_type> il) {
    if(data_ptr){
        while (avail_ptr != data_ptr)
            alloc.destroy(--avail_ptr);
        alloc.destroy(avail_ptr);
    }
    std::copy(il.begin(), il.end(), std::back_inserter(*this));
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::insert(MyVector::iterator position, value_type &val) {
    int i = (position - data_ptr);
    if (avail_ptr == limit_ptr){
        grow();
        position = data_ptr + i;
    }
    if (position == avail_ptr) {
        unchecked_append(val);
        return position;
    }
    else {
        MyVector<T, Allocator> temp;
        std::copy(begin(), position, std::back_inserter(temp));
        temp.push_back(val);
        std::copy(position, avail_ptr, std::back_inserter(temp));
        *this = temp;
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator
MyVector<T, Allocator>::insert(MyVector::iterator position, MyVector::size_type n, const value_type &val) {
    int i = (position - data_ptr);
    if (avail_ptr == limit_ptr){
        grow();
        position = data_ptr + i;
    }
    if (position == avail_ptr) {
        unchecked_append(val);
        return position;
    }
    else {
        MyVector<T, Allocator> temp;
        std::copy(begin(), position, std::back_inserter(temp));
        for (int j = 0; j < n; j++){
            temp.push_back(val);
        }
        std::copy(position, avail_ptr, std::back_inserter(temp));
        *this = temp;
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::insert(MyVector::iterator position, value_type &&val) {
    int i = (position - data_ptr);
    if (avail_ptr == limit_ptr){
        grow();
        position = data_ptr + i;
    }
    if (position == avail_ptr) {
        unchecked_append(val);
        return position;
    }
    else {
        MyVector<T, Allocator> temp;
        std::copy(begin(), position, std::back_inserter(temp));
        temp.push_back(val);
        std::copy(position, avail_ptr, std::back_inserter(temp));
        *this = temp;
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator
MyVector<T, Allocator>::insert(MyVector::iterator position, MyVector::iterator first, MyVector::iterator last) {
    int i = (position - data_ptr);
    if (avail_ptr == limit_ptr){
        grow();
        position = data_ptr + i;
    }
    if (position == avail_ptr) {
        std::copy(first, last, std::back_inserter(*this));
        return position;
    }
    else {
        MyVector<T, Allocator> temp;
        std::copy(begin(), position, std::back_inserter(temp));
        std::copy(first, last, std::back_inserter(temp));
        std::copy(position, avail_ptr, std::back_inserter(temp));
        *this = temp;
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::insert(MyVector::const_iterator position, std::initializer_list<value_type> il) {
    if (avail_ptr == limit_ptr)
        grow();
    if (position == avail_ptr) {
        std::copy(il.begin(), il.end(), std::back_inserter(*this));
        return position;
    }
    else {
        MyVector<T, Allocator> temp;
        int i = (position - data_ptr);
        std::copy(begin(), position, std::back_inserter(temp));
        std::copy(il.begin(), il.end(), std::back_inserter(temp));
        std::copy(position, avail_ptr, std::back_inserter(temp));
        *this = temp;
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
template<class... Args>
typename MyVector<T, Allocator>::iterator MyVector<T, Allocator>::emplace(MyVector::iterator position, Args &&... args) {
    int i = (position - data_ptr);
    if (avail_ptr == limit_ptr){
        grow();
        position = data_ptr + i;
    }
    if (position == avail_ptr) {
        alloc.construct(avail_ptr++, std::forward<Args>(args)...);
        return position;
    }
    else {
        MyVector<T, Allocator> temp;
        std::copy(begin(), position, std::back_inserter(temp));
        temp.alloc.construct(temp.end(),std::forward<Args>(args)...);
        temp.avail_ptr++;
        std::copy(position, avail_ptr, std::back_inserter(temp));
        *this = temp;
        temp.uncreate();
        iterator p = this->data_ptr + i;
        return p;
    }
}

template<class T,class Allocator>
template<class... Args>
void MyVector<T, Allocator>::emplace_back(Args &&... args) {
    if (avail_ptr == limit_ptr)
        grow();
    alloc.construct(avail_ptr++, std::forward<Args>(args)...);
}


// Private functions:

template<class T,class Allocator>
void MyVector<T, Allocator>::create() {
    data_ptr = avail_ptr = limit_ptr = nullptr;
}

template<class T,class Allocator>
void MyVector<T, Allocator>::create(MyVector::size_type n, const T &val) {
    data_ptr = alloc.allocate(n);
    limit_ptr = avail_ptr = data_ptr + n;
    std::uninitialized_fill(data_ptr, limit_ptr, val);
}

template<class T,class Allocator>
void MyVector<T, Allocator>::create(MyVector::const_iterator i, MyVector::const_iterator j) {
    data_ptr = alloc.allocate(j - i);
    limit_ptr = avail_ptr = std::uninitialized_copy(i, j, data_ptr);
}

template<class T,class Allocator>
void MyVector<T, Allocator>::uncreate() {
    if (data_ptr) {
        iterator it = avail_ptr;
        while (it != data_ptr)
            alloc.destroy(--it);

        alloc.deallocate(data_ptr, limit_ptr - data_ptr);
    }
    data_ptr =  limit_ptr  =  avail_ptr  =  nullptr;
}

template<class T,class Allocator>
void MyVector<T, Allocator>::unchecked_append(const T &val) {
    alloc.construct(avail_ptr++, val);
}

template<class T,class Allocator>
void MyVector<T, Allocator>::unchecked_remove() {
    iterator it = avail_ptr;
    alloc.destroy(--it);
    avail_ptr--;
}

template<class T,class Allocator>
void MyVector<T, Allocator>::grow() {
    size_type new_size = std::max(2 * (limit_ptr - data_ptr), ptrdiff_t(1));

    iterator new_data_ptr = alloc.allocate(new_size);
    iterator new_avail_ptr = std::uninitialized_copy(data_ptr, avail_ptr, new_data_ptr);

    uncreate();

    data_ptr = new_data_ptr;
    avail_ptr = new_avail_ptr;
    limit_ptr = data_ptr + new_size;
}

template<class T,class Allocator>
void MyVector<T, Allocator>::shrink() {
    size_type new_size = avail_ptr - data_ptr;

    iterator new_data_ptr = alloc.allocate(new_size);
    iterator new_avail_ptr = std::uninitialized_copy(data_ptr, avail_ptr, new_data_ptr);

    uncreate();

    data_ptr = new_data_ptr;
    avail_ptr = new_avail_ptr;
    limit_ptr = new_avail_ptr;
}

template<class T,class Allocator>
void MyVector<T, Allocator>::re_size(MyVector::size_type new_size) {
    iterator new_data_ptr = alloc.allocate(new_size);
    iterator new_avail_ptr = std::uninitialized_copy(data_ptr, std::min(data_ptr+size(),data_ptr+new_size), new_data_ptr);

    uncreate();

    data_ptr = new_data_ptr;
    avail_ptr = new_avail_ptr;
    limit_ptr = data_ptr + new_size;
}

template<class T,class Allocator>
void MyVector<T, Allocator>::new_size(MyVector::size_type new_size) {
    iterator new_data_ptr = alloc.allocate(new_size);
    iterator new_avail_ptr = std::uninitialized_copy(data_ptr, avail_ptr, new_data_ptr);

    uncreate();

    data_ptr = new_data_ptr;
    avail_ptr = new_avail_ptr;
    limit_ptr = data_ptr + new_size;
}

template<class T,class Allocator>
bool operator==( const MyVector<T, Allocator> lhs, const MyVector<T, Allocator> rhs ){
    if (lhs.size() != rhs.size())
        return false;
    for (int i = 0; i < lhs.size(); i++){
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template<class T,class Allocator>
bool operator!=( const MyVector<T, Allocator> lhs, const MyVector<T, Allocator> rhs ){
    if (lhs.size() != rhs.size())
        return true;
    for (int i = 0; i < lhs.size(); i++){
        if (lhs[i] != rhs[i])
            return true;
    }
    return false;
}

template<class T,class Allocator>
bool operator<(MyVector<T, Allocator> lhs, MyVector<T, Allocator> rhs ){
    return std::lexicographical_compare(lhs.begin(),lhs.end(), rhs.begin(), rhs.end());
}

template<class T,class Allocator>
bool operator<=(MyVector<T, Allocator> lhs, MyVector<T, Allocator> rhs ){
    return std::lexicographical_compare(lhs.begin(),lhs.end(), rhs.begin(), rhs.end()) || lhs == rhs;
}

template<class T,class Allocator>
bool operator>(MyVector<T, Allocator> lhs, MyVector<T, Allocator> rhs ){
    return std::lexicographical_compare(rhs.begin(), rhs.end() ,lhs.begin(),lhs.end());
}

template<class T,class Allocator>
bool operator>=(MyVector<T, Allocator> lhs, MyVector<T, Allocator> rhs ){
    return std::lexicographical_compare(rhs.begin(), rhs.end() ,lhs.begin(),lhs.end()) || lhs == rhs;
}


#endif //MYVECTOR_MYVECTOR_H
