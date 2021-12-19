#pragma once

namespace AVL {
template <class T>
struct node
{
  T key;
  unsigned char height;
  node<T>* left;
  node<T>* right;
  node(T k) {
    key = k;
    left = right = 0;
    height = 1;
  }
};

template <class T>
unsigned char height(node<T>* p) {
  return p ? p->height : 0;
}

template <class T>
int bfactor(node<T>* p) {
  return height(p->right) - height(p->left);
}

template <class T>
void fixheight(node<T>* p) {
  unsigned char hl = height(p->left);
  unsigned char hr = height(p->right);
  p->height = (hl > hr ? hl : hr) + 1;
}

template <class T>
node<T>* rotateright(node<T>* p)
{
  node<T>* q = p->left;
  p->left = q->right;
  q->right = p;
  fixheight(p);
  fixheight(q);
  return q;
}

template <class T>
node<T>* rotateleft(node<T>* q)
{
  node<T>* p = q->right;
  q->right = p->left;
  p->left = q;
  fixheight(q);
  fixheight(p);
  return p;
}

template <class T>
node<T>* balance(node<T>* p)
{
  fixheight(p);
  if (bfactor(p) == 2) {
    if (bfactor(p->right) < 0) p->right = rotateright(p->right);
    return rotateleft(p);
  }
  if (bfactor(p) == -2) {
    if (bfactor(p->left) > 0) p->left = rotateleft(p->left);
    return rotateright(p);
  }
  return p;
}

template <class T>
node<T>* insert(node<T>* p, T k)
{
  if (!p) return new node<T>(k);
  if (k < p->key)
    p->left = insert(p->left, k);
  else
    p->right = insert(p->right, k);
  return balance(p);
}

template <class T>
node<T>* findmin(node<T>* p)
{
  return p->left ? findmin(p->left) : p;
}

template <class T>
node<T>* removemin(
    node<T>* p)
{
  if (p->left == 0) return p->right;
  p->left = removemin(p->left);
  return balance(p);
}

template <class T>
node<T>* remove(node<T>* p, T k)
{
  if (!p) return 0;
  if (k < p->key)
    p->left = remove(p->left, k);
  else if (k > p->key)
    p->right = remove(p->right, k);
  else
  {
    node<T>* q = p->left;
    node<T>* r = p->right;
    delete p;
    if (!r) return q;
    node<T>* min = findmin(r);
    min->right = removemin(r);
    min->left = q;
    return balance(min);
  }
  return balance(p);
}

template <class T>
void findPreSuc(node<T>* root, node<T>*& pre, node<T>*& suc, T key) {
  if (root == NULL) return;

  if (root->key == key) {
    if (root->left != NULL) {
      node<T>* tmp = root->left;
      while (tmp->right) tmp = tmp->right;
      pre = tmp;
    }

    if (root->right != NULL) {
      node<T>* tmp = root->right;
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
node<T>* search(node<T>* root, T key) {
  if (root == NULL || root->key == key) return root;

  if (root->key < key) return search(root->right, key);

  return search(root->left, key);
}
}  // namespace AVL