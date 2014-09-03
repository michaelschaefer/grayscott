# -*- coding: utf-8 -*-


from PySide.QtGui import qRgb


class  ColorTable(object):
    '''ColorTable provides a single static method for creating a color table
    to use as indexed palette for QImage'''
    
    @staticmethod
    def create(color1, color2, color3, n=256):
        '''creates a new colortable for three given colors.
        
        Arguments
        ---------
        color1 : tuple
            color in the beginning
        color2 : tuple
            color in the middle
        color3 : tuple
            color in the end
        n : int
            length of the colortable
        '''
        
        colortable = []
        for index in xrange(n):
            color = [0, 0, 0]
            for k in xrange(3):
                if index < n/2:
                    color[k] = color1[k] + 2.0*index/n * (color2[k] - color1[k])
                else:
                    color[k] = color2[k] + (2.0*index/n-1) * (color3[k] - color2[k])
            colortable.append(qRgb(color[0], color[1], color[2]))
        return colortable


# some default color tables
Gray = ColorTable.create((0, 0, 0), (127, 127, 127), (255, 255, 255))
RedWhiteBlue = ColorTable.create((186, 61, 62), (246, 246, 246), (5, 48, 97))
BlueWhiteRed = ColorTable.create((5, 48, 97), (246, 246, 246), (186, 61, 62))