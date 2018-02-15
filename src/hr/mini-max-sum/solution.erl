-module(solution).
-export([main/0, test/0]).


main() ->
  List = read(5),
  {Min, Max} = solve(List),
  io:fwrite("~w ~w\n", [Min, Max]).

read(N) ->
  if
    N > 0 ->
      {ok, [Value]} = io:fread("", "~d"),
      [Value | read(N - 1)];
    true ->
      []
  end.

solve(List) ->
  Values = [
    lists:sum(lists:append(slice(List, 0, I), slice(List, I + 1))) ||
      I <- seq(0, length(List) - 1)],
  {lists:min(Values), lists:max(Values)}.

slice(List, Start, End) ->
  case List of
    [] ->
      [];
    [Head | Tail] ->
      if
        Start >= End ->
          [];
        Start > 0 ->
          slice(Tail, Start - 1, End - 1);
        true ->
          [Head | slice(Tail, 0, End - 1)]
      end
  end.

slice(List, Start) ->
  slice(List, Start, length(List)).

seq(Start, End) -> lists:seq(Start, End).

test() ->
  [1, 2, 3] = slice(seq(1, 10), 0, 3),
  [2, 3, 4] = slice(seq(1, 10), 1, 4),
  [] = slice(seq(1, 10), 1, 1),
  {10, 14} = solve([1, 2, 3, 4, 5]),
  true.
