find . -regex '.*\.\(c\|h\)' -exec clang-format --style=file -i {} \;
