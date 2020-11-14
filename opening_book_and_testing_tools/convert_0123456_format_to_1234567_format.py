
if __name__ == "__main__":

    move_string = input("Transposition to convert: ")
    moves = [int(char) for char in move_string]
    converted_string = ""

    for move in moves:
        converted_string += str(move + 1)

    print(converted_string)
