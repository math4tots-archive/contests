-module(solution).
-export([main/0, test/0]).


main() ->
  {ok, [T]} = io:fread("", "~d"),
  write([X + 1 || X <- read(T)]),
  true.

read(T) ->
  if T > 0 ->
    {ok, [Value]} = io:fread("", "~d"),
    [Value | read(T - 1)];
  true ->
    []
  end.

write(XS) ->
  case XS of
    [] ->
      false;
    [H | T] ->
      io:fwrite("~w\n", [H]),
      write(T)
  end.

test() ->
  true.
