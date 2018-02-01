"""
ID: math4to3
LANG: PYTHON3
TASK: gift1
"""

with open('gift1.in', 'r') as f:
    np = int(f.readline())
    people = [f.readline().strip() for _ in range(np)]
    accts = {person: 0 for person in people}
    for _ in range(np):
        giver = f.readline().strip()
        amnt, nreceivers = map(int, f.readline().split())
        if nreceivers:
            amnt_per_person = amnt // nreceivers
            for _ in range(nreceivers):
                receiver = f.readline().strip()
                accts[giver] -= amnt_per_person
                accts[receiver] += amnt_per_person

with open('gift1.out', 'w') as f:
    for person in people:
        f.write('{} {}\n'.format(person, accts[person]))
