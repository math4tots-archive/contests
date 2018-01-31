// Counts number of ways n can be made from the given coin types
func nways(n: Int, coins: [Int]) -> Int {

  let cs = Set(coins).sorted()

  if cs.count == 0 {
    return n == 0 ? 1 : 0
  }

  var memo = cs.map({ (_) in
    return (0...n).map({ (_) in
      return 0
    })
  })

  // special case 1: if total = 0, there's always exactly 1 way.
  for (i, _) in coins.enumerated() {
    memo[i][0] = 1
  }

  // special case 2: using exactly 1 coin
  let firstCoin = coins.first!
  for total in stride(from: firstCoin, to: n + 1, by: firstCoin) {
    memo[0][total] = 1
  }

  // inductive step
  for (i, coin) in Array(coins.enumerated())[1...] {
    for total in 0...min(n, coin - 1) {
      memo[i][total] = memo[i-1][total]
    }

    if coin <= n {
      for total in coin...n {
        memo[i][total] = memo[i-1][total] + memo[i][total - coin]
      }
    }
  }

  return memo.last!.last!
}

assert(nways(n: 10, coins: [2, 7]) == 1)
assert(nways(n: 10, coins: [1, 7]) == 2)
assert(nways(n: 4, coins: [1, 2, 3]) == 4)
assert(nways(n: 10, coins: [2, 5, 3, 6]) == 5)
assert("abc def".split(separator: " ") == ["abc", "def"])

let n = readLine()!.split(separator: " ").map({ Int($0)! }).first!
let coins = readLine()!.split(separator: " ").map({ Int($0)! })

print(nways(n: n, coins: coins))
