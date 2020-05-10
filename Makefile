FLAGS=-O3

game_objects=four_in_a_row_command_line.o game_state.o engine_API.o
test_objects=test.o game_state.o engine_API.o test_game_state.o \
             test_engine_API.o

four_in_a_row_command_line: $(game_objects)
	g++ $(FLAGS) $(game_objects) -o four_in_a_row_command_line

test: $(test_objects)
	g++  $(FLAGS) $(test_objects) -o test

four_in_a_row_command_line.o: four_in_a_row_command_line.cpp
	g++ -c $(FLAGS) -c four_in_a_row_command_line.cpp

game_state.o: game_state.cpp
	g++ -c $(FLAGS) game_state.cpp

engine_API.o: engine_API.cpp
	g++ -c $(FLAGS) engine_API.cpp

test.o: test.cpp
	g++ -c $(FLAGS) test.cpp

test_game_state.o: test_game_state.cpp
	g++ -c $(FLAGS) test_game_state.cpp

test_engine_API.o: test_engine_API.cpp
	g++ -c $(FLAGS) test_engine_API.cpp

.PHONY: clean
clean:
	rm -v *.o
