def process(line):
    daystr, name, diff = line.split()
    return int(daystr), name, int(diff)


def winners(output):
    return sorted(key for key in output if output[key] == max(output.values()))


with open('measurement.in') as f:
    N = int(f.readline())
    output = {
        'Bessie': 7,
        'Elsie': 7,
        'Mildred': 7,
    }

    update_count = 0

    for _, name, diff in sorted(process(f.readline()) for _ in range(N)):
        last = winners(output)
        output[name] += diff
        now = winners(output)
        if last != now:
            update_count += 1

with open('measurement.out', 'w') as f:
    f.write('{}\n'.format(update_count))
