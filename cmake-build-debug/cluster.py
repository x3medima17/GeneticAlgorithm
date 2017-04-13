import random
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE
import numpy as np
import sys
import pylab

class ContinuousFilter:
	class KalmanFilter(object):

	    def __init__(self, process_variance, estimated_measurement_variance):
	        self.process_variance = process_variance
	        self.estimated_measurement_variance = estimated_measurement_variance
	        self.posteri_estimate = 0.0
	        self.posteri_error_estimate = 1.0

	    def input_latest_noisy_measurement(self, measurement):
	        priori_estimate = self.posteri_estimate
	        priori_error_estimate = self.posteri_error_estimate + self.process_variance

	        blending_factor = priori_error_estimate / (priori_error_estimate + self.estimated_measurement_variance)
	        self.posteri_estimate = priori_estimate + blending_factor * (measurement - priori_estimate)
	        self.posteri_error_estimate = (1 - blending_factor) * priori_error_estimate

	    def get_latest_estimated_measurement(self):
	        return self.posteri_estimate

	def __init__(self):
		self.data = []
		self.filtered = []


	@staticmethod
	def filter(noise):
		Y = noise
		measurement_standard_deviation = np.std(Y)

		# The smaller this number, the fewer fluctuations, but can also venture off
		# course..
		process_variance = 1e-1
		estimated_measurement_variance = measurement_standard_deviation ** 2  # 0.05 ** 2
		kalman_filter = ContinuousFilter.KalmanFilter(process_variance, estimated_measurement_variance)
		posteri_estimate_graph = []

		for iteration in Y:
		    kalman_filter.input_latest_noisy_measurement(iteration)
		    posteri_estimate_graph.append(kalman_filter.get_latest_estimated_measurement())
		return posteri_estimate_graph

	def push(self,val):
		self.data.append(val)
		if len(self.data) > 100:
			curr = self.filter(self.data[-100:])[-1]
			self.filtered.append(curr)


def random_waker(N):
	val = 1 
	y = [val]
	eps = .05
	for i in range(N-1):
		# val *= random.uniform(1-eps,1+eps)
		val += random.uniform(-1,1)
		y.append(val)
	return y



iteration_count = 1000

Y = random_waker(iteration_count)

obj = ContinuousFilter()

for item in Y:
	obj.push(item)


out = ContinuousFilter.filter(Y[:100]) + obj.filtered

est = ContinuousFilter.filter(Y)
pylab.figure()
pylab.plot(Y, color='r', label='noisy measurements', alpha=.5)
pylab.plot(out, 'b-', label='continuous estimate')
pylab.plot(est, 'g', label='discrete estimate')
pylab.legend()
pylab.xlabel('Iteration')
pylab.ylabel('Voltage')
pylab.show()

sys.exit()

def f(x):
	return (x-2)**9

def fp(x):
	return x**9 - 18*x**8 + 144*x**7 - 672*x**6 + 2016*x**5 - 4032*x**4 + 5376*x**3 - 4608*x**2 + 2304*x - 512


print(f(5), fp(5))

X = np.arange(1.92, 2.08,0.001)
Y = [f(x) for x in X]
yp = [fp(x) for x in X]
yp2 = [fp(x) for x in X]

print(yp == yp2)

plt.plot(X,yp)
plt.plot(X,Y)
plt.show()
print(x)

centers = [(0,0), (7,10), (50,50), (55,45)]
eps = 3

N = 200

vals = []

for item in centers:
	for i in range(N):
		axes = []
		for _ in item:
			axes.append(random.uniform(item[0]-eps, item[0]+eps))
		vals.append(axes)



x,y = zip(*vals)
ax = plt.subplot(121)
ax.scatter(x,y, alpha=.5)

model = TSNE(n_components=2, random_state=0)
np.set_printoptions(suppress=True)

Q = model.fit_transform(vals) 

x,y = zip(*Q)
ax = plt.subplot(122)

ax.scatter(x,y, color='r', alpha=.5)
plt.tight_layout()
plt.show()

