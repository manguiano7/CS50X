import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
    # TODO: Read database file into a variable persons
    persons = []
    with open(sys.argv[1], newline='') as csvfile:
        reader = csv.reader(csvfile)
        # this is the header, includes subsequences
        subsequences = next(reader)[1:]
        # does not include header in first list element
        for row in reader:
            for i in range(1, len(row)):
                # converts string elements to row elements
                row[i] = int(row[i])
                # fills up database of persons
            persons.append(row)
            # print(row)

    # print(type(persons[0]))
    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], newline='') as file:
        # dna sequence is long string of STRs
        dna_sequence = file.readline()
        # print(dna_sequence)

    # TODO: Find longest match of each STR in DNA sequence
    # subsequeces are from persons header

    # sequence to match to be filled
    sequence_to_match = []

    for subsequence in subsequences:
        # print(subsequence)
        # print(longest_match(dna_sequence, subsequence))
        # longest match returns number of times each subsequence is found in the dna sequence
        # each number of times is an element of the list sequence to match
        sequence_to_match.append(longest_match(dna_sequence, subsequence))

    # print(sequence_to_match)

    # TODO: Check database for matching profiles
    for person in persons:
        # print(person)
        if person[1:] == sequence_to_match:
            print(person[0])
            return
    # if no match (return doesn't work above)
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
