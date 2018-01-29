def adjust(grade):
    if grade < 38:
        return grade

    up_rounded = (grade + 4) // 5 * 5

    if up_rounded - grade < 3:
        return up_rounded

    return grade

n = int(input())
for grade in (int(input()) for _ in range(n)):
    print(adjust(grade))
