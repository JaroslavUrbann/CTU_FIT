import numpy as np


class ES:

	def __init__(self, lr, std, population_size, F, theta):
		# the size of the step
		self.alpha = lr
		# standard deviation of the noise that gets added to theta
		self.sigma = std
		# the number of noise vectors to create
		self.population_size = population_size
		# the function that is going to be optimized
		self.F = F
		# the parameter vector that is being optimized
		self.theta = theta

	# returns average fitness
	def step(self):
		# gaussian distribution (noise vector)
		epsilon = np.random.randn(self.population_size, self.theta.shape[0])
		# reward vector
		R = np.zeros(self.population_size)
		# for each new individual, measure fitness
		for i, e in enumerate(epsilon):
			# adding gaussian with deviation sigma to the parameter vector and calculating reward
			R[i] = self.F(self.theta + e * self.sigma)
		# normalize rewards
		Ravg = np.mean(R)
		Rstd = np.std(R)
		R -= Ravg
		if Rstd: R /= Rstd
		# perform update:
		# multiply each noise vector by it's reward and normalize by sigma and population size
		self.theta += self.alpha * np.dot(epsilon.T, R) / (self.sigma * self.population_size)
		return Ravg

