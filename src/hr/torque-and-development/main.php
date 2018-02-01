<?php

class UnionFind {
  private $map;
  private $size_map;

  function __construct(int $n) {
    $this->map = range(0, $n - 1);
    $this->size_map = array_fill(0, $n, 1);
    assert(sizeof($this->map) === sizeof($this->size_map));
  }

  function get(int $i) {
    $p = $this->map[$i];
    if ($this->map[$p] === $p) {
      return $p;
    }
    $path = [$i];
    while ($this->map[end($path)] !== end($path)) {
      $path[] = $this->map[end($path)];
    }
    return end($path);
  }

  function size_of(int $i) {
    return $this->size_map[$this->get($i)];
  }

  function join(int $a, int $b) {
    $a = $this->get($a);
    $b = $this->get($b);
    if ($a === $b) {
      return;
    }

    if ($this->size_map[$a] > $this->size_map[$b]) {
      $t = $a;
      $a = $b;
      $b = $t;
    }

    $this->map[$a] = $b;
    $this->size_map[$b] += $this->size_map[$a];
  }
}

$uf = new UnionFind(3);
assert($uf->size_of(0) === 1);
assert($uf->size_of(1) === 1);
$uf->join(0, 1);
assert($uf->size_of(0) === 2);
assert($uf->get(0) === $uf->get(1));
assert($uf->size_of(2) == 1);

$uf = new UnionFind(100);
for ($i = 0; $i < 100; $i++) {
  $uf->join(0, $i);
}
assert($uf->size_of(50) == 100);


class Graph {
  private $n;
  private $edges;

  function __construct(int $n) {
    $this->n = $n;
    $this->edges = [];
  }

  function join(int $a, int $b, $weight) {
    $this->edges[] = [$weight, $a, $b];
  }

  function mst() {
    $mst = [];

    $uf = new UnionFind($this->n);
    $edges = $this->edges;
    sort($edges);

    foreach($edges as $edge) {
      $w = $edge[0];
      $a = $edge[1];
      $b = $edge[2];
      if ($uf->get($a) !== $uf->get($b)) {
        $uf->join($a, $b);
        $mst[] = [$a, $b, $w];
      }
    }

    return $mst;
  }
}

assert([1, 2, 3] < [2, 3, 4]);


function solve(int $n, int $clib, int $croad, $edges) {
  $graph = new Graph($n + 1);

  for ($i = 1; $i <= $n; $i++) {
    $graph->join($i, 0, $clib);
  }

  foreach($edges as $edge) {
    $a = $edge[0];
    $b = $edge[1];
    $graph->join($a, $b, $croad);
  }

  $mst = $graph->mst();

  $total = 0;
  foreach($mst as $edge) {
    $weight = $edge[2];
    $total += $weight;
  }

  return $total;
}

assert(solve(3, 2, 1, [[1, 2], [3, 1], [2, 3]]) == 4);

$fin = fopen("php://stdin", "r");

fscanf($fin, "%i", $q);

for ($i = 0; $i < $q; $i++) {
  fscanf($fin, "%i%i%i%i", $n, $m, $clib, $croad);
  $edges = [];
  for ($j = 0; $j < $m; $j++) {
    fscanf($fin, "%i%i", $a, $b);
    $edges[] = [$a, $b];
  }
  echo solve($n, $clib, $croad, $edges) . "\n";
}

?>
