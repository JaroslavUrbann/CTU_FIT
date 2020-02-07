import gym
import time
import numpy as np
env = gym.make('FetchReach-v1')
# env = gym.wrappers.Monitor(env, "recording")
n_steps = 0
tim = time.time()
for i_episode in range(1):
    observation = env.reset()
    for t in range(50):
        # env.render()
        n_steps += 1
        # time.sleep(0.5)
        action = env.action_space.sample()
        action = np.zeros(4)
        action[0] = 0.1
        observation, reward, done, info = env.step(action)
        print(observation["observation"][6])
        if done:
            print("Episode finished after {} timesteps".format(t+1))
            break
env.close()
print(n_steps, time.time() - tim)
