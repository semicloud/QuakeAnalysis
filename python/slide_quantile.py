# 滑动窗口的4分位法，Demo
import numpy as np

np.random.seed(42)
w_len = 5
C = 121
W = 56
H = 72
data = np.random.randint(0, 101, (C, H, W), dtype=int)

x = np.array([6, 47, 49, 15, 42, 41, 7, 39, 43, 40, 36])
np.sort(x)
q = np.quantile(x, [0.25, 0.5, 0.75], interpolation='nearest')
print(q)


