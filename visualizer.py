# -*- coding: utf-8 -*-


import matplotlib.pyplot as plt
import numpy as np


class Visualizer(object):
    '''Framework to plot, update and export time-dependent image data'''
    
    def __init__(self, size, dpi=72.0, export=False, keep_alive=True, show=True):
        self.dpi = dpi
        self.export = export
        self.images_written = 0
        self.keep_alive = keep_alive
        self.show = show
        
        # create plot
        figsize = tuple(max(200, s) for s in size)
        fig = plt.figure(figsize=figsize, dpi=self.dpi, facecolor='white')
        fig.add_axes([0, 0, 1, 1], frameon=False)

        # make plot interactive
        if self.show == True:
            plt.ion()

        # configure plot        
        plt.xticks([])
        plt.yticks([])

    def close(self):
        # close interactive mode
        if self.show == True:
            plt.ioff()

        # keep last image on screen
        if self.keep_alive == True:
            plt.show()

    def update(self, data, cmap=plt.cm.RdBu_r, interpolation='bicubic'):
        # check if image exists
        axesImage = plt.gci()
        if axesImage is None:
            # create image
            axesImage = plt.imshow(data, cmap=cmap, interpolation=interpolation)
            
        # set new data
        axesImage.set_data(data)
        axesImage.set_clim(data.min(), data.max())

        # update plot in interactive mode
        if self.show == True:
            plt.draw()
            
        # write out new image
        if self.export == True:
            filename = './temp/temp-{:05d}.png'.format(self.images_written)            
            self.images_written += 1
            plt.savefig(filename, dpi=self.dpi)
        
        
        
        
