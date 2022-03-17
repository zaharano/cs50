# TODO

import cs50

while True:
    height = cs50.get_int("How high? (1-8): ")
    if height > 0 and height < 9:
        break
    print("Uh, that's not a valid input bub")

for i in range(1, height + 1):
    for j in range(height - i):
        print(" ", end="")
    for j in range(i):
        print("#", end="")
    print("  ", end="")
    for j in range(i):
        print("#", end="")
    print("")
