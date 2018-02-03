/*
Use naive solution of actually painting a grid to find
how much of the original two billboards are still visible
*/
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

short grid[2001][2001];

typedef struct {
  int x1, y1, x2, y2;
} Rect;

void paint(int a, int b, short color) {
  grid[a + 1000][b + 1000] = color;
}

void fill(Rect r, short color) {
  int x, y;
  int lx = MIN(r.x1, r.x2);
  int ly = MIN(r.y1, r.y2);
  int ux = MAX(r.x1, r.x2);
  int uy = MAX(r.y1, r.y2);
  for (x = lx; x < ux; x++) {
    for (y = ly; y < uy; y++) {
      grid[x][y] = color;
    }
  }
}

int count() {
  int total = 0, x, y;
  for (x = 0; x <= 2000; x++) {
    for (y = 0; y <= 2000; y++) {
      total += grid[x][y];
    }
  }
  return total;
}

void fscanr(FILE *fin, Rect *r) {
  fscanf(fin, "%d%d%d%d", &r->x1, &r->y1, &r->x2, &r->y2);
  r->x1 += 1000;
  r->y1 += 1000;
  r->x2 += 1000;
  r->y2 += 1000;
}

int main() {
  Rect a, b, t;
  FILE *fin = fopen("billboard.in", "r");
  FILE *fout = fopen("billboard.out", "w");

  fscanr(fin, &a);
  fscanr(fin, &b);
  fscanr(fin, &t);

  fill(a, 1);
  fill(b, 1);
  fill(t, 0);
  fprintf(fout, "%d\n", count());
}
