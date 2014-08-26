#! /usr/bin/env python3
# -*- coding: utf-8 -*-


import matplotlib.pyplot as plt
import numpy as np
import sys


from grayscottproblem import GrayScottProblem


def export(data, F, k):
    figsize = tuple(max(200, s) / 72.0 for s in data.shape)
    fig = plt.figure(figsize=figsize, dpi=72.0, facecolor='white')
    fig.add_axes([0, 0, 1, 1], frameon=False)
    plt.xticks([])
    plt.yticks([])

    plt.imshow(data, cmap=plt.cm.RdBu_r, interpolation='bicubic')

    filename = './study/F{:03d}-k{:03d}.png'.format(int(1000*F), int(1000*k))
    plt.savefig(filename, dpi=72.0)


def main():
    size = 256
    timesteps = 20000
    nF, nk = sys.argv[1:]

    for F in np.linspace(0, 0.1, nF):
        for k in np.linspace(0, 0.1, nk):
            coefficients = { 'Du': 0.16, 'Dv': 0.08, 'F': F, 'k': k }
            problem = GrayScottProblem(size, coefficients=coefficients)
            for step in range(timesteps):
                problem.evolve()
            print('Parameter F={:3.0f}, k={:3.0f} finished'.format(1000*F, 1000*k))
            export(problem.v, F, k)


if __name__ == '__main__':
    main()
    
