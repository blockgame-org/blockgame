import os
import sys

INCLUDE_DIR = "include/"
SRC_DIR = "src/"

with open("makefile-template") as f:
    MAKE_TEMPLATE = f.read()


def main():
    gcc_mm_output = os.popen("gcc -MM -IBlockGame BlockGame/*.c BlockGame/Input/*.c BlockGame/Graphics/*.c BlockGame/Basic/*.c").read()
    # print(MAKE_TEMPLATE.format(GCC_MM_OUTPUT=gcc_mm_output))

    with open("makefile", 'w') as f:
        f.write(MAKE_TEMPLATE.format(GCC_MM_OUTPUT=gcc_mm_output))

    os.system("make build")


if __name__ == "__main__":
    main()
