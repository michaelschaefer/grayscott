#! /usr/bin/env python
# -*- coding: utf-8 -*-


from grayscottproblem import GrayScottProblem
from PySide import QtCore, QtGui
import sys


class GrayScottGui(QtGui.QMainWindow):
    def __init__(self):
        super(GrayScottGui, self).__init__()
        self.setWindowTitle('Gray-Scott model Gui')
        self.panel = Panel(self)
        self.setCentralWidget(self.panel)


class Panel(QtGui.QWidget):
    def __init__(self, parent):
        super(Panel, self).__init__(parent)

        self.param_panel = ParameterWidget(self)
        self.vis_panel = VisualizationWidget(self)
        
        box = QtGui.QVBoxLayout()
        box.addWidget(self.param_panel)
        box.addWidget(self.vis_panel)
        self.setLayout(box)


class ParameterWidget(QtGui.QWidget):
    def __init__(self, parent):
        super(ParameterWidget, self).__init__(parent)
        
        box = QtGui.QGridLayout()

        self.sliders = [Slider(self, 'F'), Slider(self, 'k')]
        for i in xrange(len(self.sliders)):
            box.addWidget(self.sliders[i], i, 0)

        self.button_simulate = QtGui.QPushButton('simulate')
        self.button_simulate.clicked.connect(self._simulate)        
        box.addWidget(self.button_simulate, len(self.sliders), 0)

        self.setLayout(box)

    def _simulate(self):
        F = self.sliders[0].value() / 1000.0
        k = self.sliders[1].value() / 1000.0
        problem = GrayScottProblem(128, { 'Du': 0.16, 'Dv': 0.08, 'F': F, 'k': k })
        for step in xrange(100):
            problem.evolve()
        print('finished')


class Slider(QtGui.QWidget):    
    def __init__(self, parent, name):
        super(Slider, self).__init__(parent)
        self.name = name
        
        box = QtGui.QGridLayout()
        self.label = QtGui.QLabel()
        self.slide = QtGui.QSlider(QtCore.Qt.Orientation.Horizontal)
        self.slide.valueChanged.connect(self._update_label)
        self.slide.setMinimum(0)
        self.slide.setMaximum(100)
        self.slide.setSingleStep(1)
        self.slide.setValue(35)        
        box.addWidget(self.label, 0, 0)
        box.addWidget(self.slide, 0, 1)
        self.setLayout(box)

    def value(self):
        return self.slide.value()

    def _update_label(self, value):
        self.label.setText('{} = {:.3f}'.format(self.name, value / 1000.0))


class VisualizationWidget(QtGui.QWidget):
    def __init__(self, parent):
        super(VisualizationWidget, self).__init__(parent)


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    win = GrayScottGui()
    win.show()
    sys.exit(app.exec_())
