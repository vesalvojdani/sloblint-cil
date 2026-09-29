#include "testharness.h"
#include <stddef.h>

// sizeof evaluates its operand exactly when the operand's type is a
// variable-length array (C11 6.5.3.4p2); __alignof__ never evaluates it.

int calls;
int f(void) { calls++; return 0; }
typedef int T[2];

int main(int argc, char **argv) {
  int n = argc + 2;
  int vla[n][n];
  int vla3[n][n][n];
  int fixed[3][4];
  int (*pv)[n];
  int i = 0;
  int zero = 0;
  size_t s;
  pv = vla;

  // Variable-length operands: side effects happen.
  calls = 0; s = sizeof(vla[f()]);
  if (calls != 1 || s != n * sizeof(int)) E(1);
  calls = 0; s = sizeof(vla3[f()][f()]);
  if (calls != 2 || s != n * sizeof(int)) E(2);
  calls = 0; s = sizeof(pv[f()]);
  if (calls != 1 || s != n * sizeof(int)) E(3);
  i = 0; s = sizeof(vla[i++]);
  if (i != 1 || s != n * sizeof(int)) E(4);
  calls = 0; s = !zero ? sizeof(vla[f()]) : 1;
  if (calls != 1) E(5);
  calls = 0; s = sizeof(vla[f()]) + sizeof(vla[f()]);
  if (calls != 2) E(6);

  // Type names of variable-length array type: their lengths are evaluated.
  calls = 0; s = sizeof(int[f() + n]);
  if (calls != 1 || s != n * sizeof(int)) E(7);
  calls = 0; s = sizeof(int[n][f() + 1]);
  if (calls != 1 || s != n * sizeof(int)) E(8);
  calls = 0; s = sizeof(T[f() + n]);
  if (calls != 1 || s != n * sizeof(T)) E(9);
  i = 0; s = sizeof(int[i++ + n]);
  if (i != 1 || s != n * sizeof(int)) E(10);

  // Fixed-size operands, including a pointer to a variable-length array type,
  // and the operand of __alignof__: nothing is evaluated.
  calls = 0; s = sizeof(int (*)[f() + 1]);
  if (calls != 0 || s != sizeof(int *)) E(11);
  calls = 0; s = sizeof(vla[f()][0]);
  if (calls != 0 || s != sizeof(int)) E(12);
  calls = 0; s = sizeof(fixed[f()]);
  if (calls != 0 || s != sizeof(fixed[0])) E(13);
  i = 0; s = sizeof(fixed[i++]);
  if (i != 0) E(14);
  calls = 0; s = sizeof(calls++);
  if (calls != 0) E(15);
  calls = 0; s = __alignof__(vla[f()]);
  if (calls != 0) E(16);
  calls = 0; s = sizeof(sizeof(vla[f()]));
  if (calls != 0) E(17);
  calls = 0; s = zero && sizeof(vla[f()]);
  if (calls != 0) E(18);

  // The length of vla is fixed at its declaration.
  int m = n;
  n = 7;
  calls = 0; s = sizeof(vla[f()]);
  if (calls != 1 || s != m * sizeof(int)) E(19);

  SUCCESS;
}
