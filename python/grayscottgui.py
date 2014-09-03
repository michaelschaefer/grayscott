#! /usr/bin/env python
# -*- coding: utf-8 -*-


import gui
from grayscottproblem import GrayScottProblem
import numpy as np
from PySide import QtCore, QtGui
import sys


class GrayScottGui(QtGui.QMainWindow):
    def __init__(self, title='Gray-Scott model GUI'):
        super(GrayScottGui, self).__init__()
        self.title = title
        self.setWindowTitle(self.title)

        self.parameterbox = gui.ParameterBox(self)                            
        self.simulationviewer = gui.SimulationViewer(self, 256)
        
        self.buttonbox = gui.ButtonBox(self)
        self.buttonbox.pause.connect(self._simulation_pause)
        self.buttonbox.resume.connect(self._simulation_resume)
        self.buttonbox.start.connect(self._simulation_start)
        self.buttonbox.stop.connect(self._simulation_interrupt)
        
        box = QtGui.QGridLayout()
        box.addWidget(self.parameterbox, 0, 0)
        box.addWidget(self.buttonbox, 1, 0)
        box.addWidget(self.simulationviewer, 0, 1, 2, 1, alignment=QtCore.Qt.AlignmentFlag.AlignVCenter)

        panel = QtGui.QWidget(self)
        panel.setLayout(box)

        self.setCentralWidget(panel)        
        
        self.simulator = gui.Simulator()
        self.simulator.done.connect(lambda: self._simulation_ended('simulation finished'))
        self.simulator.interrupted.connect(lambda: self._simulation_ended('simulation interrupted'))        
        self.simulator.update_image.connect(self.simulationviewer.update_image)
        self.simulator.update_status.connect(self.simulationviewer.update_status)
        
    def closeEvent(self, event):
        if self.simulator.isRunning() == True:
            self.simulator.interrupt()
            self.simulator.wait()                            

    def _simulation_ended(self, msg):                    
        self.simulationviewer.update_status(msg)
        self.buttonbox.toggle_buttons(gui.Mode.Start)

    def _simulation_interrupt(self):
        self.simulator.interrupt() 
        
    def _simulation_pause(self):
        self.buttonbox.toggle_buttons(gui.Mode.Resume | gui.Mode.Stop)
        self.simulator.pause()
        
    def _simulation_resume(self):
        self.buttonbox.toggle_buttons(gui.Mode.Pause | gui.Mode.Stop)
        self.simulator.resume()       

    def _simulation_start(self):
        self.buttonbox.toggle_buttons(gui.Mode.Pause | gui.Mode.Stop)
        parameters = self.parameterbox.parameters()
        self.simulator.set_parameters(parameters)                
        self.simulator.start()
        

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    win = GrayScottGui()
    win.show()
    sys.exit(app.exec_())
