#include "Intersection.hpp"
#include "AvlTree.hpp"
#include "BTree.hpp"
#include "Segment.hpp"

using namespace std;

int main() {
  int size = 5;

  Segment segments[] = {{{1, 5}, {4, 5}},
                  {{2, 5}, {10, 1}},
                  {{3, 2}, {10, 3}},
                  {{6, 4}, {9, 4}},
                  {{7, 1}, {8, 1}}};

    cout << IntersectNaive(segments, size) << endl << endl;
    cout << AVLIntersect(segments, size) << endl << endl;
    cout << BTreeIntersect(segments, size) << endl << endl;

  return 0;
}