FLAGS=-O3

game_objects=four_in_a_row_command_line.o opening_book.o game_state.o engine_API.o \
             transposition_table.o
test_objects=test.o opening_book.o game_state.o engine_API.o test_game_state.o \
             test_engine_API.o transposition_table.o

four_in_a_row_command_line: $(game_objects)
	g++ $(FLAGS) $(game_objects) -o four_in_a_row_command_line

install:
	cp four_in_a_row_command_line /usr/local/bin
	mkdir -p /usr/local/share/four_in_a_row_opening_book
	cp opening_book/opening_book* /usr/local/share/four_in_a_row_opening_book

uninstall:
	rm /usr/local/bin/four_in_a_row_command_line
	rm -R /usr/local/share/four_in_a_row_opening_book

test: $(test_objects)
	g++  $(FLAGS) $(test_objects) -o test

four_in_a_row_command_line.o: four_in_a_row_command_line.cpp
	g++ -c $(FLAGS) four_in_a_row_command_line.cpp

opening_book.o: opening_book.cpp
	g++ -c $(FLAGS) opening_book.cpp

game_state.o: game_state.cpp
	g++ -c $(FLAGS) game_state.cpp

engine_API.o: engine_API.cpp
	g++ -c $(FLAGS) engine_API.cpp

transposition_table.o: transposition_table.cpp
	g++ -c $(FLAGS) transposition_table.cpp

test.o: ./testing/test.cpp
	g++ -c $(FLAGS) ./testing/test.cpp

test_game_state.o: ./testing/test_game_state.cpp
	g++ -c $(FLAGS) ./testing/test_game_state.cpp

test_engine_API.o: ./testing/test_engine_API.cpp
	g++ -c $(FLAGS) ./testing/test_engine_API.cpp

.PHONY: clean
clean:
	rm -v *.o
