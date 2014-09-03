# -*- coding: utf-8 -*-


from . import colortable
from PySide import QtCore
from PySide.QtGui import qRgb, QFrame, QImage, QLabel, QPixmap, QVBoxLayout, QWidget 


class SimulationViewer(QWidget):
    def __init__(self, parent, size=256):
        super(SimulationViewer, self).__init__(parent)        
        self.size = size        

        self.pixmap = QLabel()        
        self.pixmap.setFixedSize(self.size, self.size)
        self.pixmap.setFrameStyle(QFrame.Panel | QFrame.Sunken)
        self.status_line = QLabel('no simulation running')

        box = QVBoxLayout()
        box.addWidget(self.pixmap, alignment=QtCore.Qt.AlignHCenter)
        box.addWidget(self.status_line, alignment=QtCore.Qt.AlignHCenter)
        self.setLayout(box)
        
        self.colortable = colortable.BlueWhiteRed 
        
    def resize(self, size):
        self.size = size
        self.pixmap.setFixedSize(self.size, self.size)   

    @QtCore.Slot(str)
    def update_status(self, status):
        self.status_line.setText(status)
        self.status_line.adjustSize()
    
    @QtCore.Slot(QImage)
    def update_image(self, image):
        image.setColorTable(self.colortable)        
        pixmap = QPixmap.fromImage(image)
        self.pixmap.setPixmap(pixmap.scaled(self.size, self.size))
        