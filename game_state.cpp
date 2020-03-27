
class GameState:

    def get_value(self, column, row):
        return self.board[10 + column + row * 9]

    def make_move(self, column):
        position = 10 + column + self.column_height[column] * 9
        self.board[position] = ("1", "2")[self.number_of_moves % 2]
        self.move_history[self.number_of_moves] = position
        self.column_height[column] += 1
        self.number_of_moves += 1

    def undo_last_move(self):
        self.number_of_moves -= 1
        position = self.move_history[self.number_of_moves]
        self.column_height[position % 9 - 1] -= 1
        self.board[position] = "0"

    def four_in_a_row(self):
        """True iff there is a four in a row that goes through the last made move."""
        position = self.move_history[self.number_of_moves - 1]
        player = self.board[position]

        # Columns
        if position // 9 >= 4:
            if self.board[position - 9] == player:
                if self.board[position - 18] == player:
                    if self.board[position - 27] == player:
                        return True

        # Rows
        in_row = 1
        if self.board[position - 1] == player:
            in_row += 1
            if self.board[position - 2] == player:
                in_row += 1
                if self.board[position - 3] == player:
                    in_row += 1
        if self.board[position + 1] == player:
            in_row += 1
            if self.board[position + 2] == player:
                in_row += 1
                if self.board[position + 3] == player:
                    in_row += 1
        if in_row >= 4:
            return True

        # Diagonals
        in_row = 1
        if self.board[position - 10] == player:
            in_row += 1
            if self.board[position - 20] == player:
                in_row += 1
                if self.board[position - 30] == player:
                    in_row += 1
        if self.board[position + 10] == player:
            in_row += 1
            if self.board[position + 20] == player:
                in_row += 1
                if self.board[position + 30] == player:
                    in_row += 1
        if in_row >= 4:
            return True

        in_row = 1
        if self.board[position - 8] == player:
            in_row += 1
            if self.board[position - 16] == player:
                in_row += 1
                if self.board[position - 24] == player:
                    in_row += 1
        if self.board[position + 8] == player:
            in_row += 1
            if self.board[position + 16] == player:
                in_row += 1
                if self.board[position + 24] == player:
                    in_row += 1
        if in_row >= 4:
            return True
