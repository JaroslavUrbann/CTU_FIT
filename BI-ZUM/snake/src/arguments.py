import argparse


def parse_args():
	parser = argparse.ArgumentParser(description='Snake game using an evolutionary algorithm',
									formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument('action', choices=('test', 'train'))

	# train parameters
	train = parser.add_argument_group('Arguments for training')
	train.add_argument("--save_file", type=str, help="Save file for the best snake.")
	train.add_argument("--es_steps", type=int, default=400, help="The number of steps of the evolutionary algorithm")
	train.add_argument("--lr", type=float, default=0.02, help="The learning rate")
	train.add_argument("--std", type=float, default=0.1, help="The standard deviation")
	train.add_argument("--population_size", type=int, default=70, help="The population size")
	train.add_argument("--eval_games", type=int, default=1, help="The number of games that determines the fitness of the snake before selection")
	train.add_argument("--hidden_layers", type=int, default=1, help="The number of hidden layers")
	train.add_argument("--layer_size", type=int, default=20, help="The number of neurons in a hidden layer")
	train.add_argument("--fov", type=int, default=5, help="The field of view of the snake (NxN), should be an odd number")

	# test parameters
	test = parser.add_argument_group('Arguments for testing')
	test.add_argument("--load_file", type=str, help="File of the snake that is to be tested, for example: trained_snakes/snake1.npy")
	test.add_argument("--fps", type=int, default=20, help="Frames per second")

	# game parameters
	game = parser.add_argument_group('Arguments for the environment')
	game.add_argument("--board_width", type=int, default=25, help="The board width")
	game.add_argument("--board_height", type=int, default=15, help="The board height")
	game.add_argument("--max_steps", type=int, default=100, help="The length of the game")
	game.add_argument("--food_period", type=int, default=5, help="The period in which the new food spawns")
	game.add_argument("--max_food", type=int, default=10, help="The maximum number of spawned food that can be in the game at the same time")
	game.add_argument("--food_life", type=int, default=100, help="The number of game steps before food despawns")

	args = parser.parse_args()

	if args.board_width < 4 or args.board_height < 4:
		raise parser.error("board is too small")
	if args.fov % 2 == 0:
		raise parser.error("fov needs to be an odd number")

	return parser.parse_args()

