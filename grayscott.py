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


import numpy as np
import sys


from commandlineparser import CommandLineParser
from grayscottproblem import GrayScottProblem
from visualizer import Visualizer


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
    # parse options and get corresponding settings dictionary
    settings = CommandLineParser.parse(sys.argv[1:])
    if settings == None:
        sys.exit(0)
        
    # create problem
    problem = GrayScottProblem(settings['size'], \
                               coefficients=settings['coefficients'])
    
    # create and init visualizer
    visualizer = Visualizer(problem.problem_size(), \
                            export=settings['export'], \
                            keepalive=settings['keepalive'], \
                            show=settings['show'])
    visualizer.update(problem.v)

    # create step generator
    step_generator = snapshot_generator(settings['timesteps'], \
                                        settings['outputsteps'], mode='linear')

    # evolution loop
    next_step = next(step_generator)
    for step in range(settings['timesteps']):
        problem.evolve()
        progress = 100 * step / settings['timesteps']
        print('{:.0f}% finished'.format(progress), end='\r')
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
    
        
        
    

