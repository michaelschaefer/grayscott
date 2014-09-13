# -*- coding: utf-8 -*-


import numpy as np


class TimeStepper(object):
    '''Generator for calculation of intermediate points for visulization
    
    Arguments
    ---------
    total : int
        Number of points to walk over
    intervals : int
        Number of intervals to split range(0, total) into
    mode : string
        Determines how the intervals are distributed. Possible modes are 
        'linear', 'log' and 'sqrt', meaning that the points evenly distributed
        on a linear, logarithmic or squared axes.
    '''
    
    def __init__(self, total, intervals, mode='linear'):
        if mode not in ('linear', 'log', 'sqrt'):
            raise ValueError('unknown mode')
        if mode == 'linear':
            points = np.linspace(0, total, intervals + 1)
        elif mode == 'log':
            points = np.logspace(0, np.log10(total + 1), intervals + 1)-1
        elif mode == 'sqrt':
            points = np.linspace(0, np.sqrt(total + 1), intervals + 1)**2

        points = [ round(p) for p in points ]                        
        old_p = points[0]
        self.points = [ old_p ]
        for p in points[1:]:
            if p <= old_p:
                self.points.append(old_p + 1)
                old_p = old_p + 1
            else:
                self.points.append(p)
                old_p = p

        self.next_index = 0
        self.generator = self._generator_func(self.points)
            

    def __iter__(self):
        return self

    def __next__(self):        
        return next(self.generator)

    def next(self):
        if self.next_index < len(self.points):
            self.next_index += 1
            return self.points[self.next_index - 1]            
        else:
            raise StopIteration
        
    def _generator_func(self, points):
        old_p = points[0]
        yield old_p
        for p in points[1:]:
            if p <= old_p:
                yield old_p + 1
                old_p += 1
            else:
                yield p
                old_p = p
