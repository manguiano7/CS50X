# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    #print("system file is" + sys.argv[1])
    teams = []
    # TODO: Read teams into memory from file
    with open(sys.argv[1]) as file:
        reader = csv.Reader(file)
        for row in reader:
            # converts ratings to integers
            row['rating'] = int(row['rating'])
            # appends rows of teams to list teams
            teams.append(row)
            
    # print(teams[0]['team'])
    # creates counts dictionary
    counts = {}
    for i in range(len(teams)):
        # initializes count for each team
        counts[teams[i]['team']] = 0

    # TODO: Simulate N tournaments and keep track of win counts
    for i in range(N):
        # winner of the tournament - last one to win
        winner = simulate_tournament(teams)
        print(winner)
        # counts increments by one for the winning team of the tournament
        counts[winner] = counts[winner] + 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    winners = teams
    # simulates rounds until there is only one winner
    while (len(winners) > 1):
        winners = simulate_round(winners)

    return winners[0]['team']


if __name__ == "__main__":
    main()
