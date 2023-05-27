import numpy as np

p = []

p.append(0.4825)

for i in range(0, 3):

    p.append(0.2126 * pow(0.5893, i))


p[0] += (1 - sum(p))


for i in range(1, len(p)):
    p[i] += p[i-1]


def leaf_generate():

    r = np.random.random()
    for i in range(len(p)):
        if r < p[i]:
            return i

n = int(input("number of generations:"))



all_count = [0] * 5

for i in range(10000):
    # initialize the count array with zeros with length of 4
    count = [0] * (n + 1)
    count[0] = 1

    for j in range(1, n + 1):

        for k in range(count[j - 1]):

            val = leaf_generate()

            count[j] += val

    if count[n] == 0:
        all_count[0] += 1
    elif count[n] == 1:
        all_count[1] += 1
    elif count[n] == 2:
        all_count[2] += 1
    elif count[n] == 3:
        all_count[3] += 1
    elif count[n] == 4:
        all_count[4] += 1


for i in range(len(all_count)):

    all_count[i] = all_count[i] / 10000

print("Number of generations:", n)
print("Probability of 0 neutron:", all_count[0])
print("Probability of 1 neutron:", all_count[1])
print("Probability of 2 neutrons:", all_count[2])
print("Probability of 3 neutrons:", all_count[3])
print("Probability of 4 neutrons:", all_count[4])

