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
import mpl_toolkits.mplot3d.axes3d
import numpy as np
import sys

from model import GrayScottProblem
from visualizer import Visualizer


# parameters for the image export
visualizer_param = {
    'colormap': plt.cm.RdBu_r,
    'dpi': 72.0,
    'facecolor': 'white',
    'interactive': True,
    'interpolation': 'bicubic',
    'visualization_steps': 100,
    'write_images': False
}

# parameters for the system
predefined_coefficients = {
    'bacteria1' : (0.16, 0.08, 0.035, 0.065),
    'unstable' : (0.16, 0.08, 0.02, 0.055),
    'zebra': (0.16, 0.08, 0.035, 0.06)
}

# parameters for the numerics
numerics_param = {
    'n': 256,
    'coefficients': predefined_coefficients['zebra'],
    'timesteps': 20000
}


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

    B = np.zeros(Z.shape)

    # upper
    B[0, 1:-1] = Z[-1, 1:-1] + Z[1, 1:-1] + Z[0, 0:-2] + Z[0, 2:]
    # upper left
    B[0,0] = Z[-1,0] + Z[1,0] + Z[0,-1] + Z[0,1]
    # upper right
    B[0,-1] = Z[-1,-1] + Z[1,-1] + Z[0,0] + Z[0,-2]

    # lower
    B[-1, 1:-1] = Z[-2, 1:-1] + Z[0, 1:-1] + Z[-1, 0:-2] + Z[-1, 2:]
    # lower left
    B[-1,0] = Z[-2,0] + Z[0,0] + Z[-1,-1] + Z[-1,1]
    # lower right
    B[-1,-1] = Z[-2,-1] + Z[0,-1] + Z[-1,-2] + Z[-1,0]

    # left
    B[1:-1, 0] = Z[1:-1, -1] + Z[1:-1, 1] + Z[0:-2, 0] + Z[2:, 0]

    # right
    B[1:-1, -1] = Z[1:-1, -2] + Z[1:-1, 0] + Z[0:-2,-1] + Z[2:, -1]            
    
    # interior
    B[1:-1, 1:-1] = Z[0:-2, 1:-1] + Z[2:, 1:-1] + Z[1:-1, 0:-2] + Z[1:-1,2:]

    return B - 4*Z


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
    u = Z['U']
    v = Z['V']

    i = 0
    while i <= N:
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
    u = Z['U']
    v = Z['V']

    # initially, u = 1 and v = 0
    u[...] = 1.0
    v[...] = 0.0

    # change u and v in a square region around the center
    n, m = u.shape
    r = 20
    u[n//2-r : n//2+r, m//4-r : m//4+r] = 0.5
    v[n//2-r : n//2+r, m//4-r : m//4+r] = 0.25

    # add 1% random noise
    u += 0.01 * np.random.random((n,m))
    v += 0.01 * np.random.random((n,m))

    return Z


def usage():
    '''
    Print usage information
    '''    

    print('''
    Usage: python grayscott.py [options]

    Options are specified in the form key=value.
    The following options are available:
    
    Du:
        Diffusion coeffient for variable u
    Dv:
        Diffusion coeffient for variable v
    k:
        Rate constant of the second equation (overwrites the value of a 
        predefined setting if one is specified)
    F:
        Feed rate (overwrites the value of a predefined setting if one is 
        specified)    
    n:
        Grid size (default: 256)
    noninteractive:
        if this option is set, calculations will not be visualized on the fly 
        (this parameter has no value).
    setting:
        Name of a parameter setting to use. Possible values are bacteria1 and
        zebra.    
    timesteps:
        Number of timesteps (default: 20000)
    visualization_steps:
        How many intermediate images are drawn and/or written out (default: 100).
        Must be at least 2
    write_images:
        if this option is set, intermediate images will be written out into
        ./temp/ (this parameter has no value).
        

    Be cautious that the order of the parameters Du,Dv,F,k and setting is 
    important since the latter definition overwrite the former.
    ''')


def process_user_parameters():
    '''
    Tries to evaluate the user-given parameters
    
    Returns
    -------
    res : bool
        True, if program evaluation can continue (that is, parameter evaluation
        was succesful AND the string help was no parameter)
    '''

    try:
        for element in sys.argv[1:]:
            if element.lower().endswith('help'):
                usage()
                return False
            elif element.lower() == 'noninteractive':
                visualizer_param['interactive'] = False
                continue
            elif element.lower() == 'write_images':
                visualizer_param['write_images'] = True
                continue
            
            key, value = element.split('=', 1)
            if key in ('n', 'timesteps'):
                numerics_param[key] = int(value)
            elif key in ('Du', 'Dv', 'F', 'k'):
                numerics_param['setting'][key] = float(value)
            elif key == 'setting':
                if value not in predefined_settings:
                    # unknown setting
                    raise ValueError
                else:
                    numerics_param['setting'] = predefined_settings[value]
            elif key == 'visualization_steps':
                if int(value) < 2:
                    raise ValueError
                visualizer_param['visualization_steps'] = int(value)
            else:
                # unknown parameter
                raise ValueError
    except:
        print('Invalid syntax')
        usage()
        return False

    return True


def snapshot_generator(timesteps, visualization_steps, mode='linear'):
    if mode == 'linear':
        points = np.linspace(0, timesteps, visualization_steps+1)
    elif mode == 'log':
        points = np.logspace(0, np.log10(timesteps+1), visualization_steps+1) - 1
    elif mode == 'sqrt':
        points = np.linspace(0, np.sqrt(timesteps+1), visualization_steps+1)**2
    else:
        raise ValueError('Unsupported mode')

    int_points = [ round(x) for x in points ]
    old_p = None
    for p in int_points:
        if old_p is None:
            yield p
            old_p = p
        else:
            if p <= old_p:
                yield old_p + 1
                old_p += 1
            else:
                yield p
                old_p = p
    

def main():
    # process user parameters
    if process_user_parameters() == False:
        sys.exit(0)

    # create helper variables    
    n = numerics_param['n']
    coefficients = numerics_param['coefficients']
    timesteps = numerics_param['timesteps']
    visualization_steps = visualizer_param['visualization_steps']
    
    # init objects
    problem = GrayScottProblem(n, coefficients=coefficients)
    visualizer = Visualizer(problem.problem_size())
    visualizer.update(problem.v)
    step_generator = snapshot_generator(timesteps, visualization_steps, mode='linear')

    # evolution loop
    next_step = next(step_generator)
    for step in range(timesteps):
        problem.evolve()
        print('{:.0f}% finished'.format(100 * step / timesteps), end='\r')        
        if step == next_step:
            visualizer.update(problem.v)
            try:
                next_step = next(step_generator)
            except StopIteration:
                pass
    else:
        print('Evolution finished')
        visualizer.close()


if __name__ == '__main__':
    main()
    
        
        
    

