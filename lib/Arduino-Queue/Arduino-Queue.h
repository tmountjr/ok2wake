#include <Arduino.h>

template <class T>
class ArduinoQueue
{
private:
  int _front, _back, _count;
  T *_data;
  int _maxitems;

public:
  ArduinoQueue(int maxitems = 256)
  {
    _front = 0;
    _back = 0;
    _count = 0;
    _maxitems = maxitems;
    _data = new T[maxitems + 1];
  }

  ~ArduinoQueue()
  {
    delete[] _data;
  }

  inline int count();
  inline int front();
  inline int back();

  void push(const T &item);
  T peek();
  T pop();
  T last();
  void clear();
};

template <class T>
inline int ArduinoQueue<T>::count()
{
  return _count;
}

template <class T>
inline int ArduinoQueue<T>::front()
{
  return _front;
}

template <class T>
inline int ArduinoQueue<T>::back()
{
  return _back;
}

template <class T>
void ArduinoQueue<T>::push(const T &item)
{
  if (_count < _maxitems)
  {
    _data[_back++] = item;
    ++_count;
    if (_back > _maxitems)
    {
      _back -= (_maxitems + 1);
    }
  }
}

template <class T>
T ArduinoQueue<T>::pop()
{
  if (_count <= 0)
    return T();
  else
  {
    T result = _data[_front];
    _front++;
    --_count;
    if (_front > _maxitems)
    {
      _front -= (_maxitems + 1);
    }
    return result;
  }
}

template <class T>
T ArduinoQueue<T>::last()
{
  if (_count <= 0)
    return T();
  else
    return _data[_back - 1];
}

template <class T>
T ArduinoQueue<T>::peek()
{
  if (_count <= 0)
    return T();
  else
    return _data[_front];
}

template <class T>
void ArduinoQueue<T>::clear()
{
  _front = _back;
  _count = 0;
}