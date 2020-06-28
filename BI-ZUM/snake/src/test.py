from src.snake import Snake
import time


def test(args, env):
    # load the snake
    s = Snake(filename=args.load_file)
    # play the game
    obs, score, done = env.reset()
    while not done:
        time.sleep(1/args.fps)
        env.render()
        action = s.action(obs)
        obs, score, done = env.step(action)
    env.render()
