#pragma once
using namespace std;
struct Point {
  int x, y;
};

struct Segment {
  Point left, right;
};

struct Event {
  int x, y;
  bool isLeft;
  int index;
  Event() {
    x = 0;
    y = 0;
    isLeft = false;
    index = 0;
  }
  Event(int x, int y, bool l, int i) : x(x), y(y), isLeft(l), index(i) {}

  bool operator<(const Event &e) const {
    if (y == e.y) return x < e.x;
    return y < e.y;
  }

  bool operator>(const Event &e) const {
    if (y == e.y) return x > e.x;
    return y > e.y;
  }

  bool operator==(const Event &e) const {
    if (x != e.x) return false;
    if (y != e.y) return false;
    if (isLeft != e.isLeft) return false;
    if (index != e.index) return false;
    return true;
  }

  bool operator!=(const Event &e) const {
    if (x == e.x) return false;
    if (y == e.y) return false;
    if (isLeft == e.isLeft) return false;
    if (index == e.index) return false;
    return true;
  }

  Event &operator=(const Event &right) {
    if (this == &right) {
      return *this;
    }
    x = right.x;
    y = right.y;
    isLeft = right.isLeft;
    index = right.index;
    return *this;
  }
};

bool onSegment(Point p, Point q, Point r) {
  if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) &&
      q.y >= std::min(p.y, r.y))
    return true;

  return false;
}

int orientation(Point p, Point q, Point r) {
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

  if (val == 0) return 0;

  return (val > 0) ? 1 : 2;
}

bool doIntersect(Segment s1, Segment s2) {
  Point p1 = s1.left, q1 = s1.right, p2 = s2.left, q2 = s2.right;

  int o1 = orientation(p1, q1, p2);
  int o2 = orientation(p1, q1, q2);
  int o3 = orientation(p2, q2, p1);
  int o4 = orientation(p2, q2, q1);

  if (o1 != o2 && o3 != o4) return true;

  if (o1 == 0 && onSegment(p1, p2, q1)) return true;
  if (o2 == 0 && onSegment(p1, q2, q1)) return true;
  if (o3 == 0 && onSegment(p2, p1, q2)) return true;
  if (o4 == 0 && onSegment(p2, q1, q2)) return true;

  return false;
}
