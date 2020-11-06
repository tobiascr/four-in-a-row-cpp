
def transposition_string_012_format(move_sequence_string):
    """ Return a transposition string in the following format. 0 is a separator
    between columns 1 represent disks from the beginning player and 2 disks
    from the other player. Between the separators each column is described
    from below. For example the transposition

    0000000
    0000000
    0000000
    0000000
    0002000
    0001000

    is represented as

    "00012000".

    0000000
    0000000
    0002000
    0001000
    0102000
    0201021

    is represented as

    "02100121200201".
    """
    board = [[], [], [], [], [], [], []]
    player = "1";

    for move in move_sequence_string:
        board[int(move)].append(player)
        if player == "1":
            player = "2"
        else:
            player = "1"

    return "0".join(["".join(column) for column in board])

if __name__ == "__main__":

    file_to_read = open(input("File to read: "))
    file_to_write = open(input("File to write to: "), 'w')

    for line in file_to_read:
        [move_sequence_string, value] = line.split()
        file_to_write.write(transposition_string_012_format(move_sequence_string)
                            + ' ' + value + '\n')

    file_to_read.close()
    file_to_write.close()

