
# This program take a file of sequences and make new shorter sequences from it and
# write them together with the old sequences to a new file.

if __name__ == "__main__":

    file_to_read = open(input("File to read: "))
    file_to_write = open(input("File to write to: "), 'w')

    for line in file_to_read:
        for n in range(1, len(line)):
            file_to_write.write(line[0:n] + '\n')

    file_to_read.close()
    file_to_write.close()
