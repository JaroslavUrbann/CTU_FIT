from src.snake import Snake
from src.evolution import ES


def train(args, env):
    nn_shape = [args.fov, args.hidden_layers, args.layer_size, 2]  # 2 is the action space
    # the function that will be optimized
    F = lambda gen: Snake(nn_shape, genotype=gen).get_score(env, args.eval_games)
    # randomly initialized genotype theta
    theta = Snake(nn_shape).get_genotype()
    # instance of evolution strategies
    es = ES(args.lr, args.std, args.population_size, F, theta)
    # optimize theta
    for step in range(args.es_steps):
        score = es.step()
        print("Step: ", step+1, "/", args.es_steps, "Fitness:", "{:.2f}".format(score * 100))
    # save theta
    Snake(nn_shape, genotype=es.theta).save_genotype(args.save_file)
