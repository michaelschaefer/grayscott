# -*- coding: utf-8 -*-


from grayscottproblem import GrayScottProblem
import numpy as np
from PySide.QtCore import QThread, Signal, Slot
from PySide.QtGui import QImage


class Simulator(QThread):
    '''Simulator is the interface between the numerics (GrayScottProblem) and
    the UI (SimulationViewer). It is derived from QThread to allow for 
    executing a simulation without blocking the user interface.
    
    Arguments
    ---------
    parameters : dict
        the parameter dictionary for the simulation
    '''
    
    done = Signal()
    interrupted = Signal()
    update_image = Signal(QImage)
    update_status = Signal(str)
    
    def __init__(self, parameters=None):
        QThread.__init__(self)
        
        self.interrupt_flag = False
        self.pause_flag = False
        self.pause_time = 10
        
        if parameters is None:
            self.parameters = None
        else:
            self.set_parameters(parameters)
            
    def _rescale(self, data):
        minimum = data.min()
        delta = data.max() - minimum
        return (data - minimum) / delta
            
    @Slot()
    def interrupt(self):        
        self.interrupt_flag = True
        self.pause_flag = False
        
    @Slot()
    def pause(self):
        self.pause_flag = True        
        
    @Slot()
    def resume(self):
        self.pause_flag = False    
        
    def set_parameters(self, parameters):
        self.coefficients = parameters['coefficients']
        self.keyframe_distance = parameters['keyframe_distance']
        self.size = parameters['size']
        self.timesteps = parameters['timesteps']
        
    def run(self):
        problem = GrayScottProblem(self.size, self.coefficients)
        
        self.interrupt_flag = False
        self.pause_flag = False
        
        for step in xrange(self.timesteps + 1):                                                
            # pause? 
            while self.pause_flag == True:
                self.msleep(self.pause_time)
            
            # interrupt?
            if self.interrupt_flag == True:                
                self.interrupted.emit()
                return
            
            problem.evolve()
            if step % self.keyframe_distance == 0:
                # create image
                data = np.require(255*self._rescale(problem.v), np.uint8, 'C')
                n, m = data.shape     
                image = QImage(data.data, n, m, QImage.Format_Indexed8)                                                
                self.update_image.emit(image)
                
            progress = int(100.0 * step / self.timesteps)
            self.update_status.emit('{}% finished'.format(progress))                            
                
        self.done.emit()
                