#pragma once

namespace TwoThree {
template <class T>
struct node {
  int size;
  T key[3];
  node<T> *first;
  node<T> *second;
  node<T> *third;
  node<T> *fourth;
  node<T> *parent;

  bool find(T k) {
    for (int i = 0; i < size; ++i)
      if (key[i] == k) return true;
    return false;
  }

  int find_n(T k) {
    for (int i = 0; i < size; ++i)
      if (key[i] == k) return i;
    return -1;
  }

  void swap(T &x, T &y) {
    int r = x;
    x = y;
    y = r;
  }

  void sort2(T &x, T &y) {
    if (x > y) swap(x, y);
  }

  void sort3(T &x, T &y, T &z) {
    if (x > y) swap(x, y);
    if (x > z) swap(x, z);
    if (y > z) swap(y, z);
  }

  void sort() {
    if (size == 1) return;
    if (size == 2) sort2(key[0], key[1]);
    if (size == 3) sort3(key[0], key[1], key[2]);
  }

  void insert_to_node(T k) {
    key[size] = k;
    size++;
    sort();
  }

  void remove_from_node(T k) {
    if (size >= 1 && key[0] == k) {
      key[0] = key[1];
      key[1] = key[2];
      size--;
    } else if (size == 2 && key[1] == k) {
      key[1] = key[2];
      size--;
    }
  }

  void become_node2(T k, node<T> *first_,
                    node<T> *second_) {
    key[0] = k;
    first = first_;
    second = second_;
    third = nullptr;
    fourth = nullptr;
    parent = nullptr;
    size = 1;
  }

  bool is_leaf() {
    return (first == nullptr) && (second == nullptr) && (third == nullptr);
  }

  node(T k)
      : size(1),
        key{k, 0, 0},
        first(nullptr),
        second(nullptr),
        third(nullptr),
        fourth(nullptr),
        parent(nullptr) {}

  node(T k, node<T> *first_, node<T> *second_, node<T> *third_,
       node<T> *fourth_, node<T> *parent_)
      : size(1),
        key{k, 0, 0},
        first(first_),
        second(second_),
        third(third_),
        fourth(fourth_),
        parent(parent_) {}

  friend node<T> *split(
      node<T> *item);
  friend node<T> *insert(node<T> *p, T k);
  friend node<T> *search(node<T> *p, T k);
  friend node<T> *search_min(
      node<T> *p);
  friend node<T> *merge(node<T> *leaf);
  friend node<T> *redistribute(
      node<T> *leaf);
  friend node<T> *fix(
      node<T> *leaf);

  friend node<T> *remove(node<T> *p, T k);
};

template <class T>
node<T> *insert(node<T> *p,
                T k) {
  if (!p)
    return new node<T>(
        k);

  if (p->is_leaf())
    p->insert_to_node(k);
  else if (k <= p->key[0])
    insert(p->first, k);
  else if ((p->size == 1) || ((p->size == 2) && k <= p->key[1]))
    insert(p->second, k);
  else
    insert(p->third, k);

  return split(p);
}

template <class T>
node<T> *split(node<T> *item) {
  if (item->size < 3) return item;

  node<T> *x =
      new node<T>(item->key[0], item->first, item->second, nullptr, nullptr,
                  item->parent);
  node<T> *y =
      new node<T>(item->key[2], item->third, item->fourth, nullptr, nullptr,
                  item->parent);

  if (x->first)
    x->first->parent = x;
  if (x->second)
    x->second->parent =
        x;
  if (y->first)
    y->first->parent = y;
  if (y->second) y->second->parent = y;

  if (item->parent) {
    item->parent->insert_to_node(item->key[1]);

    if (item->parent->first == item)
      item->parent->first = nullptr;
    else if (item->parent->second == item)
      item->parent->second = nullptr;
    else if (item->parent->third == item)
      item->parent->third = nullptr;

    if (item->parent->first == nullptr) {
      item->parent->fourth = item->parent->third;
      item->parent->third = item->parent->second;
      item->parent->second = y;
      item->parent->first = x;
    } else if (item->parent->second == nullptr) {
      item->parent->fourth = item->parent->third;
      item->parent->third = y;
      item->parent->second = x;
    } else {
      item->parent->fourth = y;
      item->parent->third = x;
    }

    node<T> *tmp = item->parent;
    delete item;
    return tmp;
  } else {
    x->parent = item;
    y->parent =
        item;
    item->become_node2(item->key[1], x, y);
    return item;
  }
}

template <class T>
node<T> *search(node<T> *p, T k) {
  if (!p) return nullptr;

  if (p->find(k))
    return p;
  else if (k < p->key[0])
    return search(p->first, k);
  else if ((p->size == 2) && (k < p->key[1]) || (p->size == 1))
    return search(p->second, k);
  else if (p->size == 2)
    return search(p->third, k);
}

template <class T>
void findPreSuc(node<T> *root, node<T> *&pre, node<T> *&suc, T key) {
  if (root == NULL) return;

  if (root->key == key) {
    if (root->left != NULL) {
      node<T> *tmp = root->left;
      while (tmp->right) tmp = tmp->right;
      pre = tmp;
    }

    if (root->right != NULL) {
      node<T> *tmp = root->right;
      while (tmp->left) tmp = tmp->left;
      suc = tmp;
    }
    return;
  }

  if (root->key > key) {
    suc = root;
    findPreSuc(root->left, pre, suc, key);
  } else
  {
    pre = root;
    findPreSuc(root->right, pre, suc, key);
  }
}

template <class T>
node<T> *search_min(node<T> *p) {
  if (!p) return p;
  if (!(p->first))
    return p;
  else
    return search_min(p->first);
}

template <class T>
node<T> *remove(node<T> *p, T k) {
  node<T> *item = search(p, k);

  if (!item) return p;

  node<T> *min = nullptr;
  if (item->key[0] == k)
    min = search_min(item->second);
  else
    min = search_min(item->third);

  if (min) {
    int &z = (k == item->key[0] ? item->key[0] : item->key[1]);
    item->swap(z, min->key[0]);
    item = min;
  }

  item->remove_from_node(k);
  return fix(item);
}

template <class T>
node<T> *fix(node<T> *leaf) {
  if (leaf->size == 0 &&
      leaf->parent ==
          nullptr) {
    delete leaf;
    return nullptr;
  }
  if (leaf->size !=
      0) {
    if (leaf->parent)
      return fix(leaf->parent);
    else
      return leaf;
  }

  node<T> *parent = leaf->parent;
  if (parent->first->size == 2 || parent->second->size == 2 ||
      parent->size == 2)
    leaf = redistribute(
        leaf);
  else if (parent->size == 2 && parent->third->size == 2)
    leaf = redistribute(leaf);
  else
    leaf =
        merge(leaf);

  return fix(leaf);
}

template <class T>
node<T> *redistribute(node<T> *leaf) {
  node<T> *parent = leaf->parent;
  node<T> *first = parent->first;
  node<T> *second = parent->second;
  node<T> *third = parent->third;

  if ((parent->size == 2) && (first->size < 2) && (second->size < 2) &&
      (third->size < 2)) {
    if (first == leaf) {
      parent->first = parent->second;
      parent->second = parent->third;
      parent->third = nullptr;
      parent->first->insert_to_node(parent->key[0]);
      parent->first->third = parent->first->second;
      parent->first->second = parent->first->first;

      if (leaf->first != nullptr)
        parent->first->first = leaf->first;
      else if (leaf->second != nullptr)
        parent->first->first = leaf->second;

      if (parent->first->first != nullptr)
        parent->first->first->parent = parent->first;

      parent->remove_from_node(parent->key[0]);
      delete first;
    } else if (second == leaf) {
      first->insert_to_node(parent->key[0]);
      parent->remove_from_node(parent->key[0]);
      if (leaf->first != nullptr)
        first->third = leaf->first;
      else if (leaf->second != nullptr)
        first->third = leaf->second;

      if (first->third != nullptr) first->third->parent = first;

      parent->second = parent->third;
      parent->third = nullptr;

      delete second;
    } else if (third == leaf) {
      second->insert_to_node(parent->key[1]);
      parent->third = nullptr;
      parent->remove_from_node(parent->key[1]);
      if (leaf->first != nullptr)
        second->third = leaf->first;
      else if (leaf->second != nullptr)
        second->third = leaf->second;

      if (second->third != nullptr) second->third->parent = second;

      delete third;
    }
  } else if ((parent->size == 2) &&
             ((first->size == 2) || (second->size == 2) ||
              (third->size == 2))) {
    if (third == leaf) {
      if (leaf->first != nullptr) {
        leaf->second = leaf->first;
        leaf->first = nullptr;
      }

      leaf->insert_to_node(parent->key[1]);
      if (second->size == 2) {
        parent->key[1] = second->key[1];
        second->remove_from_node(second->key[1]);
        leaf->first = second->third;
        second->third = nullptr;
        if (leaf->first != nullptr) leaf->first->parent = leaf;
      } else if (first->size == 2) {
        parent->key[1] = second->key[0];
        leaf->first = second->second;
        second->second = second->first;
        if (leaf->first != nullptr) leaf->first->parent = leaf;

        second->key[0] = parent->key[0];
        parent->key[0] = first->key[1];
        first->remove_from_node(first->key[1]);
        second->first = first->third;
        if (second->first != nullptr) second->first->parent = second;
        first->third = nullptr;
      }
    } else if (second == leaf) {
      if (third->size == 2) {
        if (leaf->first == nullptr) {
          leaf->first = leaf->second;
          leaf->second = nullptr;
        }
        second->insert_to_node(parent->key[1]);
        parent->key[1] = third->key[0];
        third->remove_from_node(third->key[0]);
        second->second = third->first;
        if (second->second != nullptr) second->second->parent = second;
        third->first = third->second;
        third->second = third->third;
        third->third = nullptr;
      } else if (first->size == 2) {
        if (leaf->second == nullptr) {
          leaf->second = leaf->first;
          leaf->first = nullptr;
        }
        second->insert_to_node(parent->key[0]);
        parent->key[0] = first->key[1];
        first->remove_from_node(first->key[1]);
        second->first = first->third;
        if (second->first != nullptr) second->first->parent = second;
        first->third = nullptr;
      }
    } else if (first == leaf) {
      if (leaf->first == nullptr) {
        leaf->first = leaf->second;
        leaf->second = nullptr;
      }
      first->insert_to_node(parent->key[0]);
      if (second->size == 2) {
        parent->key[0] = second->key[0];
        second->remove_from_node(second->key[0]);
        first->second = second->first;
        if (first->second != nullptr) first->second->parent = first;
        second->first = second->second;
        second->second = second->third;
        second->third = nullptr;
      } else if (third->size == 2) {
        parent->key[0] = second->key[0];
        second->key[0] = parent->key[1];
        parent->key[1] = third->key[0];
        third->remove_from_node(third->key[0]);
        first->second = second->first;
        if (first->second != nullptr) first->second->parent = first;
        second->first = second->second;
        second->second = third->first;
        if (second->second != nullptr) second->second->parent = second;
        third->first = third->second;
        third->second = third->third;
        third->third = nullptr;
      }
    }
  } else if (parent->size == 1) {
    leaf->insert_to_node(parent->key[0]);

    if (first == leaf && second->size == 2) {
      parent->key[0] = second->key[0];
      second->remove_from_node(second->key[0]);

      if (leaf->first == nullptr) leaf->first = leaf->second;

      leaf->second = second->first;
      second->first = second->second;
      second->second = second->third;
      second->third = nullptr;
      if (leaf->second != nullptr) leaf->second->parent = leaf;
    } else if (second == leaf && first->size == 2) {
      parent->key[0] = first->key[1];
      first->remove_from_node(first->key[1]);

      if (leaf->second == nullptr) leaf->second = leaf->first;

      leaf->first = first->third;
      first->third = nullptr;
      if (leaf->first != nullptr) leaf->first->parent = leaf;
    }
  }
  return parent;
}

template <class T>
node<T> *merge(node<T> *leaf) {
  node<T> *parent = leaf->parent;

  if (parent->first == leaf) {
    parent->second->insert_to_node(parent->key[0]);
    parent->second->third = parent->second->second;
    parent->second->second = parent->second->first;

    if (leaf->first != nullptr)
      parent->second->first = leaf->first;
    else if (leaf->second != nullptr)
      parent->second->first = leaf->second;

    if (parent->second->first != nullptr)
      parent->second->first->parent = parent->second;

    parent->remove_from_node(parent->key[0]);
    delete parent->first;
    parent->first = nullptr;
  } else if (parent->second == leaf) {
    parent->first->insert_to_node(parent->key[0]);

    if (leaf->first != nullptr)
      parent->first->third = leaf->first;
    else if (leaf->second != nullptr)
      parent->first->third = leaf->second;

    if (parent->first->third != nullptr)
      parent->first->third->parent = parent->first;

    parent->remove_from_node(parent->key[0]);
    delete parent->second;
    parent->second = nullptr;
  }

  if (parent->parent == nullptr) {
    node *tmp = nullptr;
    if (parent->first != nullptr)
      tmp = parent->first;
    else
      tmp = parent->second;
    tmp->parent = nullptr;
    delete parent;
    return tmp;
  }
  return parent;
}
}  // namespace TwoThree