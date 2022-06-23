# TODO
from cs50 import get_int


def main():

    while True:
        height = get_int("Height: ")
        if height >= 1 and height <= 8:
            break

    for r in range(1, height + 1):
        print(" " * (height - r) + "#" * r + "  " + "#" * r)

    # for r in range(1, height + 1):
    #     for s in range(0,height - r):
    #         print(" ", end="")
    #     for c in range(0, r):
    #         print("#", end="")
    #     print("  ", end="")
    #     for c in range(0, r):
    #         print("#", end="")
    #     print("")


if __name__ == "__main__":
    main()