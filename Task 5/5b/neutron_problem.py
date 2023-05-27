import numpy as np 

p_1 = (0.2126)*((0.5893)**0)
p_2 = (0.2126)*((0.5893)**1)
p_3 = (0.2126)*((0.5893)**2)
p_0 = 1 - p_1 - p_2 - p_3

print("Probability of 0 neutron:", p_0)
print("Probability of 1 neutron:", p_1)
print("Probability of 2 neutrons:", p_2)
print("Probability of 3 neutrons:", p_3)

print("total probability: ", p_0+p_1+p_2+p_3)

# calculate cumulative probability
p_1 = p_1 + p_0
p_2 = p_2 + p_1
p_3 = p_3 + p_2

num_trials = 10000
num_generations = 3

neutron_counts_all_trials = np.zeros(4, dtype=int)

for trial in range(num_trials):

    # create an array of size num_generations with all values set to 0
    neutron_counts = np.zeros(num_generations+1, dtype=int)
    neutron_counts[0] = 1

    for i in range(1, num_generations+1):
        # print("in generation", i, "there are", neutron_counts[i-1], "neutrons")

        for num_neutron in range(neutron_counts[i-1]):

            # generate a random number between 0 and 1
            rand_num = np.random.random()

            # print("random number: ", rand_num)

            if rand_num < p_0:
                neutron_counts[i] += 0
            elif rand_num < p_1:
                neutron_counts[i] += 1
            elif rand_num < p_2:
                neutron_counts[i] += 2
            elif rand_num < p_3:
                neutron_counts[i] += 3

    # print the final neutron counts of last generation
    # print(neutron_counts[num_generations-1])
    if neutron_counts[num_generations] == 0:
        neutron_counts_all_trials[0] += 1
    elif neutron_counts[num_generations] == 1:
        neutron_counts_all_trials[1] += 1
    elif neutron_counts[num_generations] == 2:
        neutron_counts_all_trials[2] += 1
    elif neutron_counts[num_generations] == 3:
        neutron_counts_all_trials[3] += 1

print("Probability of 0 neutron:", neutron_counts_all_trials[0]/num_trials)
print("Probability of 1 neutron:", neutron_counts_all_trials[1]/num_trials)
print("Probability of 2 neutrons:", neutron_counts_all_trials[2]/num_trials)
print("Probability of 3 neutrons:", neutron_counts_all_trials[3]/num_trials)