def main():
    text = input("Text: ")

    num_words = 1
    num_sent = 0
    num_letters = 0

    num_letters = count_letters(text)
    num_words = count_words(text)
    num_sent = count_sentences(text)

    # /*  printf("%i\n", num_letters)
    # print(num_words)
    # print(num_sent)
    avg_lett_words = num_letters/num_words * 100.0
    avg_sent_words = num_sent/num_words*100.0
    index = round(0.0588 * avg_lett_words - (0.296 * avg_sent_words) - 15.8)
    if (index < 1):
        print("Before Grade 1")
    elif (index > 15):
        print("Grade 16+")
    else:
        print("Grade " + str(index))


def count_letters(text):

    i = 0

    num_letters = 0

    while (True):
        # print(i)
        try:
            if (text[i] == '\0' or text[i] == '\n'):
                break
        # if the above causes error, except forces a break
        except:
            break
        # checks if a letter
        if (text[i].isalpha()):
            num_letters = num_letters + 1
        i += 1

    return num_letters


def count_words(text):

    i = 0
    # initializes to one to account for last word
    num_words = 1

    while(True):
        try:
            if (text[i] == '\0'):
                break
        except:
            break
        if (text[i] == ' '):
            num_words += 1
        i = i + 1

    return num_words


def count_sentences(text):

    i = 0

    num_sent = 0

    while (True):
        try:
            if (text[i] == '\0'):
                break
        except:
            break

        if (text[i] == '.' or text[i] == '!' or text[i] == '?'):
            num_sent += 1
        i += 1

    return num_sent


if __name__ == "__main__":
    main()
