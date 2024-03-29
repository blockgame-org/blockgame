import os
import sys

INCLUDE_DIR = "include/"
SRC_DIR = "src/"

with open("makefile-template") as f:
    MAKE_TEMPLATE = f.read()


def main():
    gcc_mm_output = os.popen("gcc -MM -Iinclude src/*.c").read()
    # print(MAKE_TEMPLATE.format(GCC_MM_OUTPUT=gcc_mm_output))

    with open("makefile", 'w') as f:
        f.write(MAKE_TEMPLATE.format(GCC_MM_OUTPUT=gcc_mm_output))

    if len(sys.argv) == 1:
        return

    os.system("make " + " ".join(sys.argv[1:]))


if __name__ == "__main__":
    main()
