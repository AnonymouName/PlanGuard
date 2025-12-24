import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# 数据
x = [1, 2, 3, 4, 5, 6, 7, 8, 10]
y1 = [2.2, 2.1, 2.3, 2.2, 2.0, 2.4]
y2 = [2.3, 2.6, 2.5, 3.0, 2.8, 2.2]
y3 = [4.0, 3.1, 3.1, 2.9, 3.1, 3.1]
y4 = [3.7, 3.8, 3.4, 3.3, 3.6, 3.4]
y5 = [4.6, 3.8, 3.4, 3.6, 4.2, 3.7]
y6 = [5.3, 4.4, 4.4, 4.4, 4.1, 4.0]
y7 = [4.7, 4.9, 4.9, 4.8, 4.5, 4.1]
y8 = [4.9, 5.0, 4.9, 5.0, 5.5, 4.8]
y10 = [5.2, 7.2, 6.0, 6.2, 5.8, 6.1]
avg = [sum(i) / len(i) for i in [y1, y2, y3, y4, y5, y6, y7, y8, y10]]
result = [avg[i] / x[i] for i in range(len(x))]
print(avg, result, sum(result) / len(result))
x1 = [1, 2, 3, 5, 10, 20]
z2 = [4.0, 3.9, 4.3, 4.9, 4.1, 4.1]
z3 = [6.0, 5.8, 5.5, 5.2, 5.7, 5.8]
z5 = [7.7, 6.7, 7.8, 6.8, 6.1, 8.0]
z10 = [15.3, 15.6, 13.0, 14.7, 13.9]
z20 = [22.9, 25.9, 24.9, 25.9, 25.5]
avg1 = [sum(i) / len(i) for i in [y3, z2, z3, z5, z10, z20]]
result1 = [avg1[i] / x1[i] for i in range(len(x1))]
print(avg1, result1, sum(result1) / len(result1))






data = pd.DataFrame({
    'Number': np.repeat(x, 6),
    'Action': np.concatenate([y1, y2, y3, y4, y5, y6, y7, y8, y10])
})

plt.rcParams.update({'font.size': 18})
fig, ax1 = plt.subplots(figsize=(10, 6))
#sns.violinplot(x='Number', y='Action', data=data, ax=ax1, color='r', alpha=0.2)
ax1.plot(x,avg, 'r-', label='Action or Condition')
ax1.plot(x1,avg1, 'b-', label='Rule')
ax1.set_xlabel('Number')
ax1.set_ylabel('Time(ms)')
plt.legend()
plt.show()


