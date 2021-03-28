#pragma once

namespace alloc
{
  template <typename TData>
  constexpr TData * address_of(
    TData & _value
  ) noexcept
  {
    return &_value;
  }

  template <typename TData>
  class allocator
  {
  public:
    static TData* allocate(size_t n);
    static void deallocate(TData* _pointer, size_t n);
    static void construct(TData* _pointer, const TData & _value);
    static void destroy(TData* _first, TData* _last);
  };

  template <typename TData>
  TData* allocator<TData>::allocate(
    size_t n
  )
  {

    if (n == 0)
      return nullptr;

    return static_cast<TData*>(::operator new(n * sizeof(TData)));
  }

  template <typename TData>
  void allocator<TData>::deallocate(
    TData* _pointer, size_t n
  )
  {

    if (_pointer == nullptr)
      return;

    ::operator delete(_pointer);
  }

  template <typename TData>
  void allocator<TData>::destroy(
    TData* _first, TData* _last
  )
  {

    while (_first != _last)
      _first++->~TData();

  }

  template <typename TData>
  void allocator<TData>::construct(
    TData * _pointer, const TData & _value
  )
  {
    ::new ((void*)_pointer) TData(_value);
  }
}
