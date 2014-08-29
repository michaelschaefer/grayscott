#! /usr/bin/env python3
# -*- utf-8 -*-


'''
Solving the Gray-Scott model for pattern formation

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


from commandlineparser import *
from grayscottproblem import GrayScottProblem
from timestepper import TimeStepper
from visualizer import Visualizer
    

def main():
    # get settings from command line arguments
    settings = argument_parser().parse_args()
    settings.coefficients = predefined_coefficients[settings.coefficients]
        
    # create problem
    problem = GrayScottProblem(settings.size, coefficients=settings.coefficients)
    
    # create visualizer
    visualizer = Visualizer(problem.problem_size(), \
                            export=settings.export, \
                            keepalive=settings.keepalive, \
                            show=(not settings.noshow))

    # create step generator
    stop_point_generator = TimeStepper(settings.timesteps, \
                                       settings.outputs, mode='linear')

    # evolution loop
    stop_point = next(stop_point_generator)
    for step in range(settings.timesteps + 1):        
        # print progress message
        progress = 100 * step / settings.timesteps
        print('{:3.0f}% finished'.format(progress), end='\r')

        # trigger visualization
        if step == stop_point:
            visualizer.update(problem.v)
            try:
                stop_point = next(stop_point_generator)
            except StopIteration:
                pass

        # evolve problem
        problem.evolve()
    else:
        print('\nEvolution finished')
        visualizer.close()


if __name__ == '__main__':
    main()
    
        
        
    

