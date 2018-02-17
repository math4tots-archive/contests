n, k = map(int, input().split())
a = list(map(int, input().split()))

m = [0] * k

for x in a:
    m[x % k] += 1

size = 0
for x in range(k):
    if (x + x) % k == 0:
        size += bool(m[x])
    else:
        y = (k - x) % k
        size += max(m[x], m[y])
        m[x] = m[y] = 0

print(size)
