import numpy as np
from random import randrange
import os


BOARD = 0
WALL = -1
FOOD = 1


class Game:

    def __init__(self, max_steps=200, 
                       board_size=[10, 10],  
                       food_period=20,
                       max_food=4,
                       food_life=20):
        self.h = board_size[0]
        self.w = board_size[1]
        self.max_steps = max_steps
        self.food_period = food_period
        self.max_food = max_food
        self.food_life = food_life
        self.reset()

    def _in_wall(self, pos):
        return self.board[tuple([0]+pos)] == WALL

    def _in_food(self, pos):
        return self.board[tuple([1]+pos)] == FOOD

    def _add_wall(self, pos):
        self.board[tuple([0]+pos)] = WALL

    def _add_food(self, pos):
        self.food.append([pos, self.steps])
        self.board[tuple([1]+pos)] = FOOD

    def _eat_food(self):
        self.board[tuple([1]+self.snake[0])] = BOARD
        self.score += 1
        self.food = [f for f in self.food if f[0] != self.snake[0]]

    def _rand_coord(self):
        return [randrange(self.h), randrange(self.w)]

    def _get_state(self):
        return [[self.board, self.snake[0]], self.score, self.done]

    def reset(self):
        self.score = 0
        self.steps = 0
        self.done = False
        self.food = []
        self.last_action = []
        # put walls around the board
        self.board = np.full((2, self.h, self.w), BOARD)
        self.board[0, :, -1] = WALL
        self.board[0, :, 0] = WALL
        self.board[0, -1, :] = WALL
        self.board[0, 0, :] = WALL
        # init snake position
        self.snake = [[0, 0], [0, 0]]
        while self._in_wall(self.snake[0]) or self._in_wall(self.snake[1]):
            head = self._rand_coord()
            add = randrange(1)
            if randrange(1): tail = [head[0]+add, head[1]+1-add]
            else: tail = [head[0]-add, head[1]-1+add]
            self.snake = [head, tail]
        self._add_wall(self.snake[0])
        self._add_wall(self.snake[1])
        return self._get_state()

    # action: [0,1],[0,-1]...
    def step(self, action):
        if self.done: return
        self.last_action = action

        # small reward for surviving
        self.score += 1/self.max_steps

        # move snake
        self.snake.insert(0, [sum(x) for x in zip(self.snake[0], action)])
        if self._in_wall(self.snake[0]):
            self.snake.pop(0)
            self.done = True
            return self._get_state()

        # adds snakes head to the board
        self._add_wall(self.snake[0])

        # eats food or removes the last part of the tail (to move forward)
        if self._in_food(self.snake[0]): self._eat_food()
        else: self.board[tuple([0]+self.snake.pop())] = BOARD

        # adds new food to the board
        if len(self.food) < self.max_food and self.steps % self.food_period == 0:
            to = 10     # timeout for special cases
            newfood = self._rand_coord()
            while to > 0 and (self._in_wall(newfood) or self._in_food(newfood)):
                newfood = self._rand_coord()
                to -= 1
            if to: self._add_food(newfood)

        # removes food if its too old
        if self.food and self.food[0][1] + self.food_life < self.steps:
            self.board[tuple([1] + self.food[0][0])] = BOARD
            self.food.pop(0)

        # increments step
        self.steps += 1
        if self.steps == self.max_steps:
            self.done = True

        return self._get_state()

    def render(self):
        board = np.chararray(self.board.shape[1:])
        board[:] = ' '
        for y in range(self.h):
            for x in range(self.w):
                if self.board[0,y,x] == WALL: board[y,x] = '#'
                if self.board[1,y,x] == FOOD: board[y,x] = 'f'
        for s in self.snake: board[tuple(s)] = 'o'
        board[tuple(self.snake[0])] = '*'

        # clears the console and prints the board
        os.system('cls' if os.name=='nt' else 'clear')
        for row in board: print(row.tostring().decode())

        print("step:", self.steps, "/", self.max_steps)
        print("score:", "{:.2f}".format(self.score))
        if self.last_action == [1, 0]: print("last action: down")
        if self.last_action == [-1, 0]: print("last action: up")
        if self.last_action == [0, 1]: print("last action: right")
        if self.last_action == [0, -1]: print("last action: left")

