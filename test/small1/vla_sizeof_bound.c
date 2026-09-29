#include "testharness.h"

// An array length containing the sizeof of a variable-length array is not an
// integer constant expression, so the array is a variable-length array and is
// declared where it appears (C11 6.7.6.2p4).

int n = 1;
int calls;
int f(void) { calls++; return 0; }

int main(int argc, char **argv) {
  n = argc + 2;
  int vla[n];
  int a[sizeof(int[n])];
  int b[sizeof vla];
  int c[2][sizeof(int[sizeof vla])];
  int d[sizeof(int[4])];
  int e[__alignof__(int[n])];
  n = 1;

  if (sizeof a != (argc + 2) * sizeof(int) * sizeof(int)) E(1);
  if (sizeof b != sizeof a) E(2);
  if (sizeof c != 2 * sizeof(int) * sizeof(int) * sizeof vla) E(3);
  if (sizeof d != 4 * sizeof(int) * sizeof(int)) E(4);
  if (sizeof e != __alignof__(int) * sizeof(int)) E(5);

  // The rows of c are variable-length, so sizeof evaluates c[f()], but not
  // b[f()], which is an int.
  calls = 0;
  if (sizeof(b[f()]) != sizeof(int) || calls != 0) E(6);
  if (sizeof(c[f()]) != sizeof(int) * sizeof(int) * sizeof vla) E(7);
  if (calls != 1) E(8);

  // A type name whose length is the sizeof of a variable-length array is a
  // variable-length array type, so its length is evaluated.
  calls = 0;
  if (sizeof(int[sizeof(int[f() + n])]) != sizeof(int) * sizeof(int) * n) E(9);
  if (calls != 1) E(10);
  calls = 0;
  if (sizeof(int[sizeof(c[f()])]) != sizeof(int) * sizeof c[0]) E(11);
  if (calls != 1) E(12);

  int x[2][sizeof(int[4])];
  calls = 0;
  if (sizeof(x[f()]) != 4 * sizeof(int) * sizeof(int)) E(13);
  if (calls != 0) E(14);

  SUCCESS;
}
