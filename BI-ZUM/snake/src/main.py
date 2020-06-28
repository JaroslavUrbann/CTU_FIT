from src.arguments import parse_args
from src.game import Game
from src.train import train
from src.test import test


def main():
	args = parse_args()
	env = Game(args.max_steps, [args.board_height, args.board_width], args.food_period, args.max_food, args.food_life)
	if args.action == "train":
		train(args, env)
	if args.action == "test": 
		test(args, env)
