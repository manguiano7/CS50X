# TODO

while (True):
    # gets input until input is within range
    height = input("Height: ")
    # tries treating input as integer, if integer, it will break
    try:
        height = int(height)
        if (height <= 8 and height >= 1):
            break
    # if input is string (valuerror) do nothing and go back into loop
    except ValueError:
        1

# tracks row number
M = 1
for i in range(height):
    # for spaces
    for n in range(height - M):
        print(" ", end='')
    # for left pound signs
    for m in range(M):
        print("#", end='')
    # extra spaces in middle
    print("  ", end='')

    # right side pound signs
    for m in range(M):
        print("#", end='')
    M = M + 1
    # end of line character
    print()

