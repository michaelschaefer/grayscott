# -*- coding: utf-8 -*-


from PySide.QtCore import Signal
from PySide.QtGui import QHBoxLayout, QPushButton, QWidget


class Mode(object):
    Start = 1
    Pause = 2
    Resume = 4
    Stop = 8


class ButtonBox(QWidget):
    '''A widget for the control buttons of the program.
    
    Arguments
    ---------
    parent : QWidget
        the parent widget
    mode : Mode
        select the buttons that should be enabled
    '''
        
    pause = Signal()
    resume = Signal()
    start = Signal()    
    stop = Signal()
    
    def __init__(self, parent, mode=Mode.Start):
        QWidget.__init__(self, parent)
        
        self.buttons = {}
        
        button = QPushButton('start')
        button.clicked.connect(lambda: self.start.emit())
        self.buttons['start'] = button
        
        button = QPushButton('pause')
        button.clicked.connect(lambda: self.pause.emit())
        self.buttons['pause'] = button
        
        button = QPushButton('resume')
        button.clicked.connect(lambda: self.resume.emit())
        self.buttons['resume'] = button
        
        button = QPushButton('stop')
        button.clicked.connect(lambda: self.stop.emit())
        self.buttons['stop'] = button        
        
        box = QHBoxLayout()
        box.addWidget(self.buttons['start'])
        box.addWidget(self.buttons['pause'])
        box.addWidget(self.buttons['resume'])
        box.addWidget(self.buttons['stop'])
        self.setLayout(box)
        
        self.toggle_buttons(mode)
        
    def toggle_buttons(self, mode):
        self.buttons['start'].setEnabled(mode & Mode.Start)
        self.buttons['pause'].setEnabled(mode & Mode.Pause)
        self.buttons['resume'].setEnabled(mode & Mode.Resume)
        self.buttons['stop'].setEnabled(mode & Mode.Stop)
        