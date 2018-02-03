#include <stdio.h>

void inverse_apply(int *map, int N, int *arr) {
  int out[101], i;

  for (i = 1; i <= N; i++) {
    out[i] = arr[map[i]];
  }

  for (i = 1; i <= N; i++) {
    arr[i] = out[i];
  }
}

int main() {
  int N, i, map[101], arr[101];
  FILE *fin = fopen("shuffle.in", "r");
  FILE *fout = fopen("shuffle.out", "w");

  fscanf(fin, "%d", &N);
  for (i = 1; i <= N; i++) {
    fscanf(fin, "%d", map + i);
  }
  for (i = 1; i <= N; i++) {
    fscanf(fin, "%d", arr + i);
  }
  inverse_apply(map, N, arr);
  inverse_apply(map, N, arr);
  inverse_apply(map, N, arr);
  for (i = 1; i <= N; i++) {
    fprintf(fout, "%d\n", arr[i]);
  }
  fclose(fout);
}
