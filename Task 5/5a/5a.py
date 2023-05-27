# implement optimal stopping marriage problem

import random
import matplotlib.pyplot as plt

n = 100
# s = 1, 3, 5, 10
s = 10

# initialize array of 100 size with random uniform random distribution

def init_array(n):
    array = []
    for i in range(0, n):
        array.append(random.uniform(0, 1))
    return array



# select best s number of elements from population

success = []
m_array = []
# population = init_array(n)
# best_s = sorted(population, reverse=True)[:s]

for m in range(0, n):

    # choose m number of random number from population
    sudo_success = 0
    for i in range(0, 10000):
        population = init_array(n)
        best_s = sorted(population, reverse=True)[:s]
        
        sample_m = random.sample(population, m)
        # choose the best one from sample
        best_sample = 0
        if(len(sample_m) != 0):
            best_sample = max(sample_m)
        # choose other elements that are not in sample_m
        sample_n_m = [x for x in population if x not in sample_m]
        # choose the best one from sample_n_m
        temp = 0
        for j in range(0, len(sample_n_m)):
            # print(i, j, sample_n_m[j], best_sample)
            if sample_n_m[j] > best_sample:
                temp = sample_n_m[j]
                break

        if(temp == 0):
            temp = (sample_n_m[len(sample_n_m)-1])
        
        # if sudo success is in best_s, then success

        if temp in best_s:
            sudo_success += 1

    m_array.append(m)
    success.append((sudo_success/10000) * 100)

print(success)

plt.plot(m_array, success)

# Adding labels to the x and y axis
plt.xlabel('m')
plt.ylabel('Success rate')

# Adding a title to the graph
plt.title('Optimal Stopping Marriage Problem for n = 100 and s = 10')

# Show the graph
plt.show()
        



