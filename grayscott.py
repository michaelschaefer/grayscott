#! /usr/bin/env python


from multiprocessing import Pool
from numpy import linspace
from os import system
from sys import argv


def simulate(params):
    system('./grayscott {0[0]} {0[1]} {0[2]} {0[3]}'.format(params))
    print('parameter F={0[0]}, k={0[1]} finished'.format(params))


def main():
    N = 128
    T = 10000

    workers = int(argv[1])
    pool = Pool(processes=workers)
    
    params = []
    for F in linspace(0, 0.1, 11, endpoint=True):
        for k in linspace(0, 0.1, 11, endpoint=True):
            params.append((F, k, N, T))

    pool.map(simulate, params, chunksize=workers)
    pool.close()
    pool.join()


if __name__ == '__main__':
    main()
