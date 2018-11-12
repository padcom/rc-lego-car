#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <Arduino.h>

template <typename Type> struct LinkedListEntry {
  String key;
  Type data;
  LinkedListEntry *next;
};

template <typename Type> class LinkedList {
  public:
    ~LinkedList() {
      LinkedListEntry<Type> *current = entries;
      while (current != 0) {
        LinkedListEntry<Type> *next = current->next;
        delete current;
        current = next;
      }
    }

    void append(String key, Type data) {
      LinkedListEntry<Type> **current = &entries;
      while (*current != 0) {
        current = &(*current)->next;
      }
      *current = new LinkedListEntry<Type>;
      (*current)->key = key;
      (*current)->data = data;
      (*current)->next = 0;
    }

    virtual Type get(String key, Type def) {
      LinkedListEntry<Type> *current = entries;
      while ((current != 0) && (current->key.compareTo(key) != 0)) {
        current = current->next;
      }
      return current != 0 ? current->data : def;
    }

    void forEach(void(*callback)(String key, Type data)) {
      LinkedListEntry<Type> *current = entries;
      while (current != 0) {
        callback(current->key, current->data);
        current = current->next;
      }
    }
  private:
    int index;
    LinkedListEntry<Type> *entries = 0;
};

#endif
