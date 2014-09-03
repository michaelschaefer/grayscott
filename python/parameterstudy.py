#! /usr/bin/env python
# -*- coding: utf-8 -*-


# import whole modules
import matplotlib.pyplot as plt
import numpy as np
import sys


# import parts of modules
from grayscottproblem import GrayScottProblem
from multiprocessing import Pool


# default configuration
settings = {
    'size': 128,
    'timesteps': 5000
}


def export(data, F, k):
    '''Write data to a png image
    
    Arguments
    ---------
    data : numpy.ndarray
        array containing the data to be written as png image
    F : float
        feed rate of the current configuration
    k : float
        rate constant of the current configuration
    '''
        
    figsize = tuple(s / 72.0 for s in data.shape)
    fig = plt.figure(figsize=figsize, dpi=72.0, facecolor='white')
    fig.add_axes([0, 0, 1, 1], frameon=False)
    plt.xticks([])
    plt.yticks([])

    plt.imshow(data, cmap=plt.cm.RdBu_r, interpolation='bicubic')
    plt.gci().set_clim(0, 1)

    filename = './study/F{:03d}-k{:03d}.png'.format(int(1000*F), int(1000*k))
    plt.savefig(filename, dpi=72.0)
    plt.close()


def evolution(Fk_tuple):
    F, k = Fk_tuple
    coefficients = { 'Du': 0.16, 'Dv': 0.08, 'F': F, 'k': k }        
    problem = GrayScottProblem(settings['size'], coefficients = coefficients)
    for step in range(settings['timesteps']):
        problem.evolve()
    print('Parameter F={:3.0f}, k={:3.0f} done'.format(1000*F, 1000*k))
    export(problem.v, F, k)
    return True


def main():
    '''Evolve system for certain parameters and save final state as png image'''

    # set configuration
    workers = int(sys.argv[1])
    nF, nk = sys.argv[2:]

    # create multiprocessing stuff
    pool = Pool(workers)
    range_F = np.linspace(0.000, 0.02, nF, endpoint=False)
    range_k = np.linspace(0.040, 0.070, nk)    
    for F in range_F:
        for k in range_k:
            pool.apply_async(evolution, [(F,k)])
    pool.close()
    pool.join()


if __name__ == '__main__':
    main()
    
