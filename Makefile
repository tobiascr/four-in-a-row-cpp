four_in_a_row_command_line: four_in_a_row_command_line.o game_state.o \
            engine_API.o
	g++ four_in_a_row_command_line.o game_state.o engine_API.o \
            -o four_in_a_row_command_line

four_in_a_row_command_line.o: four_in_a_row_command_line.cpp
	g++ -O3 -c four_in_a_row_command_line.cpp

game_state.o: game_state.cpp
	g++ -O3 -c game_state.cpp

engine_API.o: engine_API.cpp
	g++ -O3 -c engine_API.cpp
