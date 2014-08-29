# -*- coding: utf-8 -*-


import argparse


predefined_coefficients = {
    'bacteria1' : { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.065 },
    'unstable' : { 'Du': 0.16, 'Dv': 0.08, 'F': 0.02, 'k': 0.055 },
    'zebra': { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.06 }
}


options = {
    '-coefficients': {
        'choices': sorted([key for key in predefined_coefficients]),
        'default': 'zebra',
        'help': 'predefined coefficient setting to be used',
        'metavar': '<string>',
        'type': str
    },

    '-Du': {
        'default': 0.16,
        'help': 'diffusion coefficient for species u',
        'metavar': '<float>',
        'type': float
    },

    '-Dv': {
        'default': 0.08,
        'help': 'diffusion coefficient for species v',
        'metavar': '<float>',
        'type': float
    },  

    '-export': {
        'default': None,
        'help': 'export png images to <path>',
        'metavar': '<path>',
        'type': str
    },

    '-F': {
        'default': 0.035,
        'help': 'feed rate',
        'metavar': '<float>',
        'type': float
    },

    '-k': {
        'default': 0.035,
        'help': 'rate constant for species v',
        'metavar': '<float>',
        'type': float
    },

    '-keepalive': {
        'action': 'store_true',
        'default': False,
        'help': 'do not close plot window'        
    },

    '-noshow': {
        'action': 'store_true',
        'default': False,
        'help': 'do not show plots'
    },

    '-outputs': {
        'default': 100,
        'help': 'number of images to show and/or export',
        'metavar': '<int>',
        'type': int
    },

    '-size': {
        'default': 128,
        'help': 'number of grid cells in each dimension',
        'metavar': '<int>',
        'type': int
    },

    '-timesteps': {
        'default': 1000,
        'help': 'number of timestep to evolve the model',
        'metavar': '<int>',
        'type': int
    },

    '-verbose': {
        'action': 'store_true',
        'default': False,
        'help': 'give some information about the calculation process'
    }
    
}


def argument_parser():
    parser = argparse.ArgumentParser()

    for key in options:        
        parser.add_argument(key, **options[key])

    return parser
