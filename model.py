# -*- coding: utf-8 -*-


import numpy as np


class GrayScottProblem(object):
    '''Discretizer for the Gray-Scott model. The model consists of two coupled
    2d reaction-diffusion equations. The spacial discretization of the Laplacian
    is done by finite differences using the usual five-point stencil. For time
    integration we use the explicit Euler method.
    
    Arguments
    ---------
    n : int
        Number of spacial point in each direction
    coefficients : tuple
        A tuple of four values specifying the diffusion for the two species, the
        feed rate and the rate constant of the second equation (default: (0.16, 
        0.08, 0.035, 0.06))
    '''

    def __init__(self, n, coefficients=(0.16, 0.08, 0.035, 0.06)):
        self.n = n
        (self.Du, self.Dv, self.F, self.k) = coefficients

        # initialize system variables
        self.u = np.zeros((self.n, self.n))
        self.v = np.zeros((self.n, self.n))
        self._set_initial_values()

    def _laplacian(self, u):
        Lu = np.zeros(u.shape)
        Lu[0, 1:-1] = u[-1, 1:-1] + u[1, 1:-1] + u[0, 0:-2] + u[0, 2:]
        Lu[0,0] = u[-1,0] + u[1,0] + u[0,-1] + u[0,1]
        Lu[0,-1] = u[-1,-1] + u[1,-1] + u[0,0] + u[0,-2]
        Lu[-1, 1:-1] = u[-2, 1:-1] + u[0, 1:-1] + u[-1, 0:-2] + u[-1, 2:]
        Lu[-1,0] = u[-2,0] + u[0,0] + u[-1,-1] + u[-1,1]
        Lu[-1,-1] = u[-2,-1] + u[0,-1] + u[-1,-2] + u[-1,0]
        Lu[1:-1, 0] = u[1:-1, -1] + u[1:-1, 1] + u[0:-2, 0] + u[2:, 0]
        Lu[1:-1, -1] = u[1:-1, -2] + u[1:-1, 0] + u[0:-2,-1] + u[2:, -1]
        Lu[1:-1, 1:-1] = u[0:-2, 1:-1] + u[2:, 1:-1] + u[1:-1, 0:-2] + u[1:-1,2:]
        return Lu - 4*u

    def _set_initial_values(self, r=20, noise=0.01):        
        # set basic level: u=1, v=0
        self.u[...] = 1.0
        self.v[...] = 0.0

        # set rectangular disturbances
        self.u[self.n//2-r:self.n//2+r, self.n//2-r:self.n//2+r] = 0.5
        self.v[self.n//2-r:self.n//2+r, self.n//2-r:self.n//2+r] = 0.25

        # add random noise
        self.u += noise * np.random.random(self.u.shape)
        self.v += noise * np.random.random(self.v.shape)

    def evolve(self):
        Lu = self._laplacian(self.u)
        Lv = self._laplacian(self.v)
        uvv = self.u * self.v**2
        self.u += self.Du * Lu - uvv + self.F * (1 - self.u)
        self.v += self.Dv * Lv + uvv - (self.F + self.k) * self.v

    def problem_size(self):
        return (self.n, self.n)
