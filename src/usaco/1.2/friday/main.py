"""
ID: math4to3
LANG: PYTHON3
TASK: friday
"""


def is_leap(year):
    return year % 400 == 0 or year % 4 == 0 and not year % 100 == 0


def days_in_year(year):
    return 366 if is_leap(year) else 365


def days_in_month(year, month):
    if month in (9, 4, 6, 11):
        return 30
    elif month != 2:
        return 31
    else:
        return 28 + is_leap(year)


number_of_days_every_400_years = sum(days_in_year(year) for year in range(400))
number_of_days_to_year_cycle = [0] * 400
for year in range(1, 400):
    number_of_days_to_year_cycle[year] = (
        days_in_year(year - 1) + number_of_days_to_year_cycle[year - 1])
number_of_days_to_year_cycle = tuple(number_of_days_to_year_cycle)


def days_from_zero_to_year(year):
    return (year // 400 * number_of_days_every_400_years +
            number_of_days_to_year_cycle[year % 400])


def days_from_zero_to_date(year, month, day):
    return (days_from_zero_to_year(year) +
            sum(days_in_month(year, m) for m in range(1, month)) + day - 1)


class Date(object):
    def __init__(self, year, month, day):
        self.year = year
        self.month = month
        self.day = day

    @property
    def id(self):
        return days_from_zero_to_date(self.year, self.month, self.day)

    def __sub__(self, other):
        return self.id - other.id

    @property
    def day_of_week(self):
        # The reference date given is Monday, which falls under
        # index 2 in the desired output format.
        return (2 + (self - Date(1900, 1, 1))) % 7


assert Date(1900, 1, 2).day_of_week == 3

with open('friday.in') as f:
    n = int(f.readline())

totals = [0] * 7
for year in range(1900, 1900 + n):
    for month in range(1, 13):
        totals[Date(year, month, 13).day_of_week] += 1

with open('friday.out', 'w') as f:
    f.write('{}\n'.format(' '.join(map(str, totals))))
