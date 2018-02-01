{
ID: math4to3
TASK: ride
LANG: PASCAL
}
program ride;

procedure main();
var
    fin, fout: text;
    first, second: ansistring;
    fval, sval: integer;
    c: char;
begin
  assign(fin, 'ride.in');
  reset(fin);

  assign(fout, 'ride.out');
  rewrite(fout);

  readln(fin, first);
  readln(fin, second);

  fval := 1;
  sval := 1;

  for c in first do begin
    fval := fval * (ord(c) - ord('A') + 1) mod 47;
  end;

  for c in second do begin
    sval := sval * (ord(c) - ord('A') + 1) mod 47;
  end;

  if fval = sval then
    writeln(fout, 'GO')
  else
    writeln(fout, 'STAY');

  close(fout);
end;

begin
  main();
end.
