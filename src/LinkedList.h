#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <Arduino.h>

extern int ll_index;

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
        Serial.print("LinkedList.append - next");
        current = &(*current)->next;
      }
      Serial.println("LinkedList.append - adding new entry: " + key);
      *current = new LinkedListEntry<Type>;
      (*current)->key = key;
      (*current)->data = data;
      (*current)->next = 0;
    }

    virtual Type get(String key, Type def) {
      if (entries == 0) {
        Serial.println("No entries defined");
      } else {
        Serial.println("Entries have items defined");
      }
      LinkedListEntry<Type> *current = entries;
      while ((current != 0) && (current->key.compareTo(key) != 0)) {
        Serial.println("LinkedList.get: " + key + " not matched - next");
        current = current->next;
      }
      Serial.println("LinkedList.get: " + key + " end = " + (current == 0));
      return current != 0 ? current->data : def;
    }

    void forEach(void(*callback)(String key, Type data)) {
      LinkedListEntry<Type> *current = entries;
      while (current != 0) {
        Serial.println("LinkedList.forEach");
        callback(current->key, current->data);
        current = current->next;
      }
    }
  private:
    int index;
    LinkedListEntry<Type> *entries = 0;
};

#endif
