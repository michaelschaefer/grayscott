# -*- coding: utf-8 -*-


from PySide import QtCore
from PySide.QtCore import Signal, Slot
from PySide.QtGui import QVBoxLayout, QLabel, QSlider, QWidget


class ParameterSlider(QWidget):
    '''ParameterSlider is a QSlider combined with a QLabel to display the 
    current value of the slider
        
    Arguments
    ---------
    parent : QWidget
        parent widget
    name : str
        name of the parameter the slider represents. Will be used as 'name = ' 
        prefix for the label 
    '''
    
    valueChanged = Signal()
    
    def __init__(self, parent=None, name=None):
        QWidget.__init__(self, parent)
        self.name = name
        self.set_format('.3f')

        # create widgets
        self.label = QLabel()
        self.slider = QSlider(QtCore.Qt.Orientation.Horizontal)        
        self.slider.valueChanged.connect(self._update_label)
        
        # set default values
        self.set_range(0, 100)
        self.set_value(50)        

        # create layout for the entire widget
        box = QVBoxLayout()
        box.addWidget(self.label)
        box.addWidget(self.slider)        
        self.setLayout(box)

    def _convert(self, value, inverse=False):
        if inverse == True:
            # slider value -> true value                
            return self.offset + self.delta * float(value) / self.ticks
        else:
            # true value -> slider value
            return (value - self.offset) * self.ticks / self.delta

    @Slot()
    def _update_label(self, value):
        # convert value to external range
        value = self._convert(value, inverse=True)
        
        if self.name is None:
            # no name given -> show just 'value'
            text = self.format_str.format(value)
        else:
            # name given -> show 'name = value'
            text = self.format_str.format(self.name, value)
                        
        self.label.setText(text)
        self.valueChanged.emit()

    def set_format(self, format_str):
        if self.name is None:
            self.format_str = '{:' + format_str + '}'
        else:
            self.format_str = '{} = {:' + format_str + '}'

    def set_range(self, minimum, maximum, step=1):
        # calculate settings
        self.offset = float(minimum)
        self.delta = float(maximum - minimum)
        self.ticks = int(self.delta / step)

        # forward settings to widget
        self.slider.setMinimum(0.0)
        self.slider.setMaximum(self.ticks)
        self.slider.setSingleStep(1.0)

    def set_value(self, value):
        # convert value to internal range
        value = self._convert(float(value))
        
        # set value and emit signal        
        self.slider.setValue(value)
        self.slider.valueChanged.emit(value)
        
    def value(self):
        return self._convert(self.slider.value(), inverse=True)
