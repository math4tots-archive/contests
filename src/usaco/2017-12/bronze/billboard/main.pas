program billboard;


type
  Rect = record
    x1, y1, x2, y2: int64
  end;


function min(a, b: int64): int64;
var ret: int64;
begin
  if (a < b) then
    ret := a
  else
    ret := b;
  min := ret;
end;

function max(a, b: int64): int64;
var ret: int64;
begin
  if (a > b) then
    ret := a
  else
    ret := b;
  max := ret;
end;

function overlap(a, b: Rect): Rect;
var ret: Rect;
begin
  ret.x1 := max(min(a.x1, a.x2), min(b.x1, b.x2));
  ret.x2 := min(max(a.x1, a.x2), max(b.x1, b.x2));
  ret.y1 := max(min(a.y1, a.y2), min(b.y1, b.y2));
  ret.y2 := min(max(a.y1, a.y2), max(b.y1, b.y2));
  if (ret.x1 >= ret.x2) or (ret.y1 >= ret.y2) then begin
    ret.x1 := 0;
    ret.x2 := 0;
    ret.y1 := 0;
    ret.y2 := 0;
  end;
  overlap := ret;
end;

function areaof(r: Rect): int64;
var ret: int64;
begin
  ret := (max(r.x1, r.x2) - min(r.x1, r.x2)) *
         (max(r.y1, r.y2) - min(r.y1, r.y2));
  if ret < 0 then
    ret := 0;
  areaof := ret;
end;

function readrect(var fin: text): Rect;
var ret: Rect;
begin
  readln(fin, ret.x1, ret.y1, ret.x2, ret.y2);
  readrect := ret;
end;

procedure main();
var
  fin, fout: text;
  r1, r2, r3: Rect;
  area: int64;
begin
  assign(fin, 'billboard.in');
  reset(fin);

  assign(fout, 'billboard.out');
  rewrite(fout);

  r1 := readrect(fin);
  r2 := readrect(fin);
  r3 := readrect(fin);

  area :=
    areaof(r1) + areaof(r2)
      - areaof(overlap(r1, r3))
      - areaof(overlap(r2, r3));

  writeln(fout, area);
  close(fout);
end;

begin
  main();
end.
