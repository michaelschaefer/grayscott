# -*- coding: utf-8 -*-


from .parameterslider import ParameterSlider
from commandlineparser import predefined_coefficients
from PySide.QtCore import Signal  
from PySide.QtGui import QComboBox, QVBoxLayout, QWidget


class ParameterBox(QWidget):
    '''ParameterBox is a widget to select certain parameters for the numerical
    simulation.
    
    Arguments
    ---------
    parent : QWidget
        the parent widget        
    '''
        
    def __init__(self, parent):
        super(ParameterBox, self).__init__(parent)
        
        
        '''
        sliders
        '''

        self.sliders = {}

        slider = ParameterSlider(self, 'F')        
        slider.set_range(0, 0.1, 0.001)
        slider.set_value(0.035)
        self.sliders['F'] = slider            

        slider = ParameterSlider(self, 'k')        
        slider.set_range(0, 0.1, 0.001)
        slider.set_value(0.06)
        self.sliders['k'] = slider
        
        slider = ParameterSlider(self, 'timesteps')
        slider.set_format('.0f')
        slider.set_range(0, 100000, 1000)
        slider.set_value(10000)        
        self.sliders['timesteps'] = slider
        
        slider = ParameterSlider(self, 'keyframe distance')
        slider.set_format('.0f')
        slider.set_range(1, 100, 1)
        slider.set_value(10)
        self.sliders['keyframe_distance'] = slider
        
        slider = ParameterSlider(self, 'size')        
        slider.set_format('.0f')
        slider.set_range(32, 256, 32)
        slider.set_value(128)
        self.sliders['size'] = slider
        
        
        '''
        Combo box for default settings
        '''        
        
        self.default_coefficients = QComboBox()
        self.default_coefficients.setEditable(False)            
        for key in sorted([key for key in predefined_coefficients]):
            self.default_coefficients.addItem(key)        
        self.default_coefficients.activated.connect(self._load_predefined)
        self.default_coefficients.setCurrentIndex(0)
        self._load_predefined(0)        
        
        
        '''
        create layout
        '''        

        box = QVBoxLayout()
        box.addWidget(self.default_coefficients)
        box.addWidget(self.sliders['F'])
        box.addWidget(self.sliders['k'])
        box.addWidget(self.sliders['timesteps'])
        box.addWidget(self.sliders['keyframe_distance'])
        box.addWidget(self.sliders['size'])
        self.setLayout(box)
        
    def _load_predefined(self, index):
        key = self.default_coefficients.itemText(index)
        if key in predefined_coefficients:            
            coefficients = predefined_coefficients[key]            
            self.sliders['F'].set_value(coefficients['F'])
            self.sliders['k'].set_value(coefficients['k'])            

    def parameters(self):                
        coefficients = {}
        coefficients['Du'] = 0.16
        coefficients['Dv'] = 0.08
        coefficients['F'] = self.sliders['F'].value()
        coefficients['k'] = self.sliders['k'].value()
        
        params = {}
        params['coefficients'] = coefficients
        params['keyframe_distance'] = int(self.sliders['keyframe_distance'].value())
        params['size'] = int(self.sliders['size'].value())
        params['timesteps'] = int(self.sliders['timesteps'].value())                          
        
        return params
    