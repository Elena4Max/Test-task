#pragma once
#include "allocator.h"
#include <iostream>

template <typename TData>
class CArray
{
public:
  CArray();

  CArray(
    const CArray & _array
  );

  ~CArray();

  void push_back(
    const TData & _value
  );

  void insert(
    unsigned int  _index,
    const TData & _value
  );

  void erase(
    unsigned int _index
  );

  void clear();

  unsigned int size() const;

  TData & operator[](
    unsigned int _index
    );

  TData * begin();

  TData * end();

  void print();

protected:
  size_t _size;
  TData* _container;
  TData* _begin;
  TData* _end;
  typedef alloc::allocator<TData> data_allocator;
  void destruct(TData *from, TData *to);
  void append(const TData &_value);
};

template <typename TData>
CArray<TData>::CArray()
{
  try
  {
    _begin = data_allocator::allocate(16);
    _end = _begin;
    _size = 0;
  }
  catch (...)
  {
    _begin = nullptr;
    _end = nullptr;
  }
}

template <typename TData>
CArray<TData>::CArray(
  const CArray & _array
)
{
  _begin = data_allocator::allocate(16);
  _end = _begin;
  _size = 0;

  for (auto i = _array._begin; i != _array._end; i++)
  {
    this->push_back(*i);
  }

}

template <typename TData>
void CArray<TData>::push_back(
  const TData & _value
)
{
  append(_value);
}

template <typename TData>
void CArray<TData>::clear()
{
  data_allocator::destroy(_begin, _end);
  _end = _begin;
  _size = 0;
}

template <typename TData>
void CArray<TData>::append(
  const TData & _value
)
{
  auto new_begin = data_allocator::allocate(++_size);
  auto new_end = new_begin;
  const TData & value_copy = _value;

  try
  {
    new_end = std::move(_begin, _end, new_begin);
    data_allocator::construct(alloc::address_of(*new_end), value_copy);
    ++new_end;
  }
  catch (...)
  {
    data_allocator::deallocate(new_begin, _size--);
    throw;
  }

  data_allocator::destroy(_begin, _end);
  data_allocator::deallocate(_begin, _size);
  _begin = new_begin;
  _end = new_end;
}

template <typename TData>
TData * CArray<TData>::begin()
{
  return (_begin);
}

template <typename TData>
TData * CArray<TData>::end()
{
  return (_end);
}

template <typename TData>
unsigned int CArray<TData>::size() const
{
  return _size;
}

template <typename TData>
void CArray<TData>::insert(
  unsigned int  _index,
  const TData & _value
)
{
  auto new_begin = data_allocator::allocate(++_size);
  auto new_end = new_begin;
  const TData & value_copy = _value;
  auto pos = _begin + _index;
  try
  {
    new_end = std::move(_begin, pos, new_begin);
    data_allocator::construct(alloc::address_of(*new_end), value_copy);
    ++new_end;
    new_end = std::move(pos, _end, new_end);
  }
  catch (...)
  {
    data_allocator::deallocate(new_begin, _size--);
    throw;
  }
  data_allocator::destroy(_begin, _end);
  data_allocator::deallocate(_begin, _size);
  _begin = new_begin;
  _end = new_end;
}

template <typename TData>
TData & CArray<TData>::operator [](
  unsigned int _index
  )
{
  return this->_container[_index];
}

template <typename TData>
void CArray<TData>::destruct(
  TData *from, TData *to
)
{

  while (from != to)
  {
    from++->~TData();
  }

}

template <typename TData>
void CArray<TData>::erase(
  unsigned int _index
)
{
  auto xpos = _begin + _index;
  std::move(xpos + 1, _end, xpos);
  data_allocator::destroy(_end - 1, _end);
  --_end;
  --_size;
}

template <typename TData>
void CArray<TData>::print()
{
  for (auto i = this->begin(); i != this->end(); i++)
  {
    std::cout << *i << " ";
  }
  std::cout << "size = " << _size << std::endl;
}

template <typename TData>
CArray<TData>::~CArray()
{
  data_allocator::destroy(_begin, _end);
  data_allocator::deallocate(_begin, _size);
  _begin = _end = nullptr;
}