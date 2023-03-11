import numpy as np
from matplotlib import pyplot as plt

if __name__ == '__main__':
    w, h = 1000, 1000
    n = np.random.rand(w, h)
    plt.imsave('noise.png', n, cmap='gray')
