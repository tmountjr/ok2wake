template <class T>
class Node
{
public:
  T *data;
  Node *next;

  Node()
  {
    this->data = nullptr;
    this->next = nullptr;
  }
  Node(T *data)
  {
    this->data = data;
    this->next = nullptr;
  }
};

template <class T>
class LinkedList
{
public:
  Node<T> *head;
  Node<T> *tail;

  LinkedList()
  {
    this->head = nullptr;
    this->tail = nullptr;
  }

  void add(T *data)
  {
    Node<T> *toAdd = new Node<T>(data);
    toAdd->next = this->head;
    if (this->head == nullptr)
    {
      this->head = toAdd;
    }
    else
    {
      this->tail->next = toAdd;
    }
    this->tail = toAdd;
  }
};