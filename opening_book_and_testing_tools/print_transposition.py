
def print_board(move_sequence_string):

    columns = [[], [], [], [], [], [], []]
    player = "○";

    for move in move_sequence_string:
        columns[int(move)].append(player)
        if player == "○":
            player = "●"
        else:
            player = "○"

    for col in columns:
        while len(col) < 6:
            col.append(" ")

    for row in range(6):
        for col in range(7):
            print("|", end="")
            print(columns[col][5-row], end="")
        print("|")
    print(" 0 1 2 3 4 5 6")

if __name__ == "__main__":

    print()
    move_sequence_string = input("Transposition: ")
    print()
    print_board(move_sequence_string)

    if len(move_sequence_string) < 42:
        if len(move_sequence_string) % 2:
            print("● to play")
        else:
            print("○ to play")

