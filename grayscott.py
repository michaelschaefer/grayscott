#! /usr/bin/env python3
# -*- utf-8 -*-


'''
Solving the Gray-Scott-Model for pattern formation

The model
    The Gray-Scott model is a system of reaction-diffusion equations describing
    chemical reactions of two species. By choosing the right parameters this 
    system is able to produce stable patterns.

The numerics
    The system is discretized by finite differences on a regular 2d grid in space
    and by the explicit Euler method in time.
'''


import matplotlib.pyplot as plt
import numpy as np


def laplacian(Z):
    '''
    Calculate the Laplacian of Z

    Parameters
    ----------
    Z : numpy.ndarray
        Matrix of which to calculate the Laplacian

    Returns
    -------
    res : numpy.ndarray
        Laplacian of Z
    
    Remark
    ------
    Since the Laplacian is evaluated only in the interior of the domain, the
    boundary elements are set to 0    
    '''
    
    return Z[0:-2, 1:-1] + Z[2:, 1:-1] + Z[1:-1, 0:-2] + Z[1:-1,2:] - 4*Z[1:-1,1:-1]


def evolve(Z, N, param):
    '''
    Evolve the system for one timestep
    
    Parameters
    ----------
    Z : numpy.ndarray
        Matrix containing the system's current state
    N : int
        Number of timesteps
    param : dict
        Parameters of the system

    Yields
    ------
    res : numpy.ndarray
        Matrix containing the system's next state
    '''

    # get the interior of the system matrix
    u = Z['U'][1:-1, 1:-1]
    v = Z['V'][1:-1, 1:-1]

    i = 0
    while i < N:
        i += 1

        # yield data before evolving
        yield Z['V']

        # get Laplacian of U and V
        Lu = laplacian(Z['U'])
        Lv = laplacian(Z['V'])

        # evolve system        
        u += param['Du'] * Lu - u*v**2 + param['F'] * (1 - u)
        v += param['Dv'] * Lv + u*v**2 - (param['F'] + param['k']) * v


def set_initial_values(Z):
    '''
    Set the inital values for Z
    
    Parameters
    ----------
    Z : numpy.ndarray
        Matrix to set initial values to

    Returns
    -------
    res : numpy.ndarray
        Matrix filled with inital values
    '''

    # get the interior of the system
    u = Z['U'][1:-1, 1:-1]
    v = Z['V'][1:-1, 1:-1]

    # initially, u = 1 and v = 0
    u[...] = 1.0
    v[...] = 0.0

    # change u and v in a square region around the center
    n, m = u.shape
    r = 20
    u[n/2-r : n/2+r, m/2-r : m/2+r] = 0.5
    v[n/2-r : n/2+r, m/2-r : m/2+r] = 0.25

    # add 1% random noise
    u += 0.01 * np.random.random((n,m))
    v += 0.01 * np.random.random((n,m))

    return Z


def setup_plot(size, dpi=72.0):
    '''
    Create plot window

    Arguments
    ---------
    size : tuple
        Size of the image in pixel
    dip : float
        dots-per-inch resolution of the plot

    Returns
    -------
    image : matplotlib.image.AxesImage
        Image container for simulation result
    '''
    
    # setup figure
    figure_size = ([size[i]/float(dpi) for i in range(2)])
    figure = plt.figure(figsize=figure_size, dpi=dpi, facecolor='white')
    figure.add_axes([0, 0, 1, 1], frameon=False)
    
    # create image container
    image = plt.imshow(np.zeros(size), \
                       cmap=plt.cm.gray_r)

    # remove axes ticks
    plt.xticks([])
    plt.yticks([])

    return image


def update_image(image, data):
    '''
    Update a given image
    
    Arguments
    ---------
    image : matplotlib.image.AxesImage
        image to update
    data : numpy.ndarray
        data for the image
    '''

    image.set_data(data)
    image.set_clim(data.min(), data.max())
    plt.draw()


def main():
    # set parameters
    param = {
        'Du' : 0.16,
        'Dv' : 0.08,
        'F' : 0.035,
        'k' : 0.06
    }

    # set number of timesteps
    N_timesteps = 50000

    # create and initialize system variables
    n = 64
    Z = set_initial_values(np.zeros((n,n), [('U', np.double), ('V', np.double)]))

    # initialize plotting stuff    
    plt.ion()
    image = setup_plot(Z.shape)

    # evolve system
    step = 0
    for snapshot in evolve(Z, N_timesteps, param):        
        if step % 100 == 0:
            print('step {}'.format(step))
            update_image(image, snapshot)
            plt.savefig('./temp/temp-{:03d}.png'.format(step//100), dpi=72.0)
        step += 1
    else:
        print('Evolution finished')

    # stop interactive plot
    plt.ioff()

    # leave final image
    plt.show()


if __name__ == '__main__':
    main()
    
        
        
    

