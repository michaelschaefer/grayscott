#! /usr/bin/env python


from multiprocessing import Pool, cpu_count
from numpy import linspace
from os import system
from sys import argv


def simulate(params):
    system('./grayscott {0[0]} {0[1]} {0[2]} {0[3]}'.format(params))
    print('parameter F={0[0]}, k={0[1]} finished'.format(params))


def main():
    aF = double(argv[1])
    bF = double(argv[2])
    nF = double(argv[3])
    ak = double(argv[4])
    bk = double(argv[5])
    nk = double(argv[6])
    N = double(argv[7])
    T = double(argv[8])

    pool = Pool(processes=cpu_count)
    
    params = []
    for F in linspace(aF, bF, nF, endpoint=True):
        for k in linspace(ak, bk, nk, endpoint=True):
            params.append((F, k, N, T))

    pool.map_async(simulate, params)
    pool.close()
    pool.join()


if __name__ == '__main__':
    main()
