
""" Take a file where each row first has a transposition and at the end a time and possibly
something in between. Write transpositions
with a times larger than some fixed time to a new file.
"""

if __name__ == "__main__":

    file_to_read = open(input("File to read: "))
    file_to_write = open(input("File to write to: "), 'w')

    for line in file_to_read:
        line_parts = line.split()
        move_sequence_string = line_parts[0]
        time = int(line_parts[-1])
        if time > 100:
            file_to_write.write(move_sequence_string + '\n')

    file_to_read.close()
    file_to_write.close()

