import os

"""Take a file as an input. Split it by linelength to new files. Names of the new files
are of the form filename_x_ply. Those files must not exist before this script is run,
since it appends to files with these name.
"""

if __name__ == "__main__":

    file_to_read_name = input("File to read: ")

    for ply_level in range(1, 43):
        output_file_name = file_to_read_name + "_" + str(ply_level) + "_ply"
        os.system("grep -x '.\{" + str(ply_level) + "\}' " +
                  file_to_read_name + " >> " + output_file_name)
