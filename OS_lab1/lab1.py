import math
import matplotlib.pyplot as plt

def find_nice_interval(sizes):
  steps = 20
  interval = math.ceil(max(sizes) / steps)
  dict = {}
  for size in sizes:
    number_of_step = str(size // interval)
    if number_of_step in dict:
      dict[number_of_step].append(size)
    else:
      dict[number_of_step] = [size]
  largest_interval = max(dict.values(), key=len)
  if len(largest_interval) / len(sizes) > 0.7:
    return find_nice_interval(largest_interval)
  else:
    return dict, interval

# Read numbers from file
with open('sizes1.txt') as file:
  sizes = [int(line.strip()) for line in file]

dict, interval = find_nice_interval(sizes)
names = [str(interval * int(step)) for step in dict.keys()]
values = [len(l) for l in dict.values()]

# Generate histogram
plt.bar(names, values, tick_label = names[::-1])

# Display plot
plt.show()

