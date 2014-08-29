#! /usr/bin/env python


from multiprocessing import Pool, cpu_count
from numpy import linspace
from os import system
from sys import argv


def simulate(params):
    system('./grayscott {0[0]} {0[1]} {0[2]} {0[3]}'.format(params))
    print('parameter F={0[0]}, k={0[1]} finished'.format(params))


def main():
    aF = float(argv[1])
    bF = float(argv[2])
    nF = int(argv[3])
    ak = float(argv[4])
    bk = float(argv[5])
    nk = int(argv[6])
    N = int(argv[7])
    T = int(argv[8])

    pool = Pool(processes=cpu_count())
    
    params = []
    for F in linspace(aF, bF, nF, endpoint=True):
        for k in linspace(ak, bk, nk, endpoint=True):
            params.append((F, k, N, T))

    pool.map_async(simulate, params)
    pool.close()
    pool.join()


if __name__ == '__main__':
    main()
