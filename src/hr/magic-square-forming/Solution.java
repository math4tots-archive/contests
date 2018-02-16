import java.io.*;
import java.util.*;


public class Solution {

  public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);
    int[] arr = new int[9];
    for (int i = 0; i < 9; i++) {
      arr[i] = sc.nextInt();
    }
    System.out.println(solve(arr));
  }

  private static final int magicSum = 15;

  private static boolean isMagic(int[] arr) {
    for (int i = 0; i < 3; i++) {
      if (arr[3*i] + arr[3*i + 1] + arr[3*i + 2] != magicSum) {
        return false;
      }
      if (arr[i] + arr[i + 3] + arr[i + 6] != magicSum) {
        return false;
      }
    }
    if (arr[0] + arr[4] + arr[8] != magicSum) {
      return false;
    }
    if (arr[2] + arr[4] + arr[6] != magicSum) {
      return false;
    }
    return true;
  }

  private static int diff(int[] xs, int[] ys) {
    int cost = 0;
    for (int i = 0; i < 9; i++) {
      cost += Math.abs(xs[i] - ys[i]);
    }
    return cost;
  }

  private static int solve(int[] arr) {
    Permutator p = new Permutator(range(1, 10));
    int cost = -1;
    while (p.hasNext()) {
      int[] magic = p.next();
      if (!isMagic(magic)) {
        continue;
      }
      if (cost == -1) {
        cost = diff(arr, magic);
      } else {
        cost = Math.min(cost, diff(arr, magic));
      }
    }
    return cost;
  }

  public static void printarr(int[] arr) {
    for (int i = 0; i < arr.length; i++) {
      System.out.print(arr[i]);
      System.out.print(" ");
    }
    System.out.println();
  }

  public static int[] range(int start, int end) {
    int[] ret = new int[end - start];
    for (int i = start; i < end; i++) {
      ret[i - start] = i;
    }
    return ret;
  }

  static int[] copy(int[] arr) {
    int[] ret = new int[arr.length];
    for (int i = 0; i < ret.length; i++) {
      ret[i] = arr[i];
    }
    return ret;
  }

  public static class Permutator {
    private final int[] arr;
    private boolean more;
    private final PermHelper helper;

    Permutator(int[] arr) {
      this.arr = copy(arr);
      more = true;
      helper = new PermHelper(this.arr, this.arr.length);
    }

    public boolean hasNext() {
      return more;
    }

    public int[] next() {
      int[] ret = copy(arr);
      if (more) {
        more = helper.step();
      }
      return ret;
    }
  }

  private static class PermHelper {
    private final int[] arr;
    private final int end;
    private PermHelper helper;
    private int lastLastIndex;
    public PermHelper(int[] ar, int e) {
      arr = ar;
      end = e;
      if (e > 1) {
        helper = new PermHelper(ar, e - 1);
      }
      lastLastIndex = end - 1;
    }

    public boolean step() {
      if (end == 0 || end == 1) {
        return false;
      }

      boolean ret = helper.step();

      if (ret) {
        return true;
      }

      // We should make sure to put the array back to the way it was
      // before moving on.
      swap(arr, lastLastIndex, end - 1);

      // If we're already putting the last element to the beginning,
      // we're done.
      if (lastLastIndex == 0) {
        return false;
      }

      // Move on to the next item to put at the end.
      lastLastIndex--;
      swap(arr, lastLastIndex, end - 1);
      helper = new PermHelper(arr, end - 1);
      return true;
    }
  }

  private static void swap(int[] arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}
