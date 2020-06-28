import numpy as np
from math import sqrt
from src.game import BOARD, WALL


class Snake:

    def __init__(self, nn_shape=None, genotype=None, filename=None):
        if filename:
            self._load_genotype(filename)
        else:
            self.score = None
            self.weights = []
            self.biases = []
            self.fov, n_hidden, hidden_size, y_size = nn_shape
            # creates the neural network
            sz = 2 * self.fov ** 2
            for _ in range(n_hidden):
                self.weights.append(np.zeros((hidden_size, sz)))
                self.biases.append(np.zeros(hidden_size))
                sz = hidden_size
            self.weights.append(np.zeros((y_size, sz)))
            self.biases.append(np.zeros(y_size))

            if genotype is not None:
                self._set_genotype(genotype)

    # converts the neural network into a 1d vector
    def get_genotype(self):
        genotype = np.zeros(0)
        for i in range(len(self.weights)):
            genotype = np.concatenate((genotype, self.weights[i].flatten(), self.biases[i].flatten()))
        return genotype

    # converts a 1d vector (the genotype) to a neural network
    def _set_genotype(self, genotype):
        l = 0
        for i in range(len(self.weights)):
            r = l + np.prod(self.weights[i].shape)
            self.weights[i] = genotype[l:r].reshape(self.weights[i].shape)
            self.biases[i] = genotype[r:r + self.biases[i].shape[0]]
            l = r + self.biases[i].shape[0]

    # saves the parameters of the network
    def save_genotype(self, name):
        # [1] is for a special case with 0 hidden layers, otherwise it doesn't work
        np.save(name, np.array(self.weights + [1] + self.biases))

    # loads the parameters of the network
    def _load_genotype(self, name):
        params = np.load(name, allow_pickle=True)
        self.weights = list(params[:int(params.shape[0] / 2)])
        self.biases = list(params[int(params.shape[0] / 2) + 1:])
        self.fov = int(sqrt(self.weights[0].shape[1] / 2))

    # maps the output of the neural network to an action
    def _discretize(self, out):
        # 0 = up/down, 1 = right/left
        if abs(out[0]) > abs(out[1]) and out[0] > 0: return [1, 0]
        if abs(out[0]) > abs(out[1]) and out[0] < 0: return [-1, 0]
        if abs(out[0]) < abs(out[1]) and out[1] > 0: return [0, 1]
        return [0, -1]

    # cuts out a self.fov x self.fov square from the observation (with the snakes head in the middle)
    def _preprocess(self, obs):
        board, head = obs
        height, width = board.shape[1:]
        nn_input = np.zeros((2, self.fov, self.fov))
        top = max(0, head[0] - int(self.fov/2))
        left = max(0, head[1] - int(self.fov/2))
        down = min(height, head[0] + int(self.fov/2) + 1)
        right = min(width, head[1] + int(self.fov/2) + 1)
        top2 = int(self.fov/2) - (head[0] - top)
        down2 = top2 + down - top
        left2 = int(self.fov/2) - (head[1] - left)
        right2 = left2 + right - left
        nn_input[:, top2:down2, left2:right2] = board[:, top:down, left:right]
        return nn_input.flatten()

    # outputs an action for a given observation
    def action(self, obs):
        ln = len(self.weights) - 1
        out = self._preprocess(obs)
        # neural network prediction
        for i in range(ln):
            out = np.maximum(0, np.dot(self.weights[i], out) + self.biases[i])
        out = np.tanh(np.dot(self.weights[ln], out) + self.biases[ln])
        return self._discretize(out)

    # evaluates the snake in an environment
    def get_score(self, env, n_games):
        if self.score is None:
            self.score = 0
            for _ in range(n_games):
                obs, score, done = env.reset()
                while not done:
                    action = self.action(obs)
                    obs, score, done = env.step(action)
                self.score += score
            self.score /= n_games
        return self.score

