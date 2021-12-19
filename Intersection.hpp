#pragma once
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "Segment.hpp"
#include "AvlTree.hpp"
#include "BTree.hpp"

using namespace std;

int IntersectNaive(Segment arr[], int n) {
  std::vector<std::string> s;

  int ans = 0;

  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (doIntersect(arr[i], arr[j])) {
        s.push_back(std::to_string(i + 1) + " " + std::to_string(j + 1));
        ans++;
      }
    }
  }

  for (auto &pr : s) {
    std::cout << pr << "\n";
  }
  return ans;
}

int AVLIntersect(Segment arr[], int n) {
  std::unordered_map<std::string, int> mp;
  std::vector<Event> e;
  for (int i = 0; i < n; ++i) {
    e.push_back(Event(arr[i].left.x, arr[i].left.y, true, i));
    e.push_back(Event(arr[i].right.x, arr[i].right.y, false, i));
  }

  sort(e.begin(), e.end(), [](Event &e1, Event &e2) { return e1.x < e2.x; });

  AVL::node<Event> *s = nullptr;
  int ans = 0;

  for (int i = 0; i < 2 * n; i++) {
    Event curr = e[i];
    int index = curr.index;

    if (curr.isLeft) {
      AVL::node<Event> *next = nullptr, *prev = nullptr;
      AVL::findPreSuc(s, prev, next, curr);

      bool flag = false;
      if (next != nullptr && doIntersect(arr[next->key.index], arr[index])) {
        std::string s = std::to_string(next->key.index + 1) + " " + std::to_string(index + 1);
        if (mp.count(s) == 0) {
          mp[s]++;
          ans++;
        }
      }
      if (prev != nullptr && doIntersect(arr[prev->key.index], arr[index])) {
        std::string s = std::to_string(prev->key.index + 1) + " " + to_string(index + 1);
        if (mp.count(s) == 0) {
          mp[s]++;
          ans++;
        }
      }
      if (prev != nullptr && next != nullptr &&
          next->key.index == prev->key.index)
        ans--;

      s = AVL::insert(s, curr);
    } else {
      auto it = AVL::search(
          s, Event(arr[index].left.x, arr[index].left.y, true, index));

      AVL::node<Event> *next = nullptr, *prev = nullptr;
      AVL::findPreSuc(s, prev, next, it->key);

      if (next != nullptr && prev != nullptr) {
        std::string s = std::to_string(next->key.index + 1) + " " +
                   std::to_string(prev->key.index + 1);
        std::string s1 = std::to_string(prev->key.index + 1) + " " +
                    std::to_string(next->key.index + 1);
        if (mp.count(s) == 0 && mp.count(s1) == 0 &&
            doIntersect(arr[prev->key.index], arr[next->key.index]))
          ans++;
        mp[s]++;
      }

      s = AVL::remove(s, it->key);
    }
  }

  for (auto &pr : mp) {
    std::cout << pr.first << "\n";
  }
  return ans;
}

std::set<Event>::iterator pred(std::set<Event> &s, std::set<Event>::iterator it) {
  return it == s.begin() ? s.end() : --it;
}

std::set<Event>::iterator succ(std::set<Event> &s, std::set<Event>::iterator it) {
  return ++it;
}

int BTreeIntersect(Segment *arr, int n) {
  std::unordered_map<string, int> mp;
  std::vector<Event> e;
  for (int i = 0; i < n; ++i) {
    e.push_back(Event(arr[i].left.x, arr[i].left.y, true, i));
    e.push_back(Event(arr[i].right.x, arr[i].right.y, false, i));
  }

  sort(e.begin(), e.end(), [](Event &e1, Event &e2) { return e1.x < e2.x; });

  std::set<Event> s;
  int ans = 0;

  for (int i = 0; i < 2 * n; i++) {
    Event curr = e[i];
    int index = curr.index;

    if (curr.isLeft) {
      auto next = s.lower_bound(curr);
      auto prev = pred(s, next);
      bool flag = false;
      if (next != s.end() && doIntersect(arr[next->index], arr[index])) {
        std::string s = std::to_string(next->index + 1) + " " + std::to_string(index + 1);
        if (mp.count(s) == 0) {
          mp[s]++;
          ans++;
        }
      }
      if (prev != s.end() && doIntersect(arr[prev->index], arr[index])) {
        std::string s = std::to_string(prev->index + 1) + " " + std::to_string(index + 1);
        if (mp.count(s) == 0) {
          mp[s]++;
          ans++;
        }
      }
      if (prev != s.end() && next != s.end() && next->index == prev->index)
        ans--;

      s.insert(curr);
    }

    else {
      auto it =
          s.find(Event(arr[index].left.x, arr[index].left.y, true, index));
      auto next = succ(s, it);
      auto prev = pred(s, it);

      if (next != s.end() && prev != s.end()) {
        std::string s =
            std::to_string(next->index + 1) + " " + std::to_string(prev->index + 1);
        std::string s1 =
            std::to_string(prev->index + 1) + " " + std::to_string(next->index + 1);
        if (mp.count(s) == 0 && mp.count(s1) == 0 &&
            doIntersect(arr[prev->index], arr[next->index]))
          ans++;
        mp[s]++;
      }

      s.erase(it);
    }
  }

  for (auto &pr : mp) {
    std::cout << pr.first << "\n";
  }
  return ans;
}