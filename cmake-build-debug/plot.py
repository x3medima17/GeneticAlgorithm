import numpy as np
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE
import sys
from pylab import rcParams



def split(M):
	out = []
	tmp = []
	gen = -1
	for row in M:
		if row[0] != gen:
			gen = row[0]
			if tmp != []:
				out.append(tmp)
				tmp = []
		tmp.append(row[1:-1])
	out.append(list(tmp))
	return out

 
def plot_gen(A):
	n_formations = len(A[0]) / 2
	formations = [[] for x in range(n_formations)]

	for curr in A:
		for i in range(n_formations):
			item = curr[i], curr[i+len(curr)/2]
			formations[i].append(item)

	res = []
	for item in formations:
		x,y = zip(*item)
		res.append((x,y))
	print(len(res))
	return res
	plt.scatter(x,y)
	plt.show()

M = np.loadtxt("stats_big.out");
A = split(M)

A = list(reversed(A))

GENS = 10#len(A)
HEIGHT = 8
WIDTH = HEIGHT * 5
HEIGHT *= GENS

rcParams['figure.figsize'] = WIDTH, HEIGHT

k = 0
F = 5
gen = -1
fig, axx = plt.subplots(GENS, F)

for item in A[:GENS]:
	gen += 1
	res = plot_gen(item)
	f = -1
	for i,form in enumerate(res):
		f+=1
		k+=1
		x,y = form
		axx[gen,f].scatter(x,y, marker='o', alpha=.4)
		fig.tight_layout()
		print(gen)
	# plt.pause(0.2)
	# f.clf()

fig.savefig("stats.png")
# plt.show()

# plot_gen(A[0])
# print(len(A))

# model = TSNE(n_components=2, random_state=0, perplexity=100, method='exact')
# np.set_printoptions(suppress=True)

# gen = -1

# Q = model.fit_transform(A[gen]) 
# print(Q)



# plt.figure()
# tmp = zip(*Q)
# plt.scatter(tmp[0], tmp[1], marker='o', alpha=.4)

# plt.figure()
# tmp = zip(*A[gen])
# plt.scatter(tmp[0], tmp[1], marker='o', alpha=.4)
# plt.show()

sys.exit()

plt.clf()
plt.axis()
plt.ion()
for item in A:
   d = zip(*item)
   plt.clf()
   plt.scatter(d[0],d[1], marker='o', alpha=.4)
   plt.pause(0.01)
