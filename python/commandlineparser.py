# -*- coding: utf-8 -*-


from argparse import ArgumentParser


predefined_coefficients = {
    'bacteria1' : { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.065 },
    'unstable' : { 'Du': 0.16, 'Dv': 0.08, 'F': 0.02, 'k': 0.055 },
    'zebra': { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.06 }
}


options = {
    '-coefficients': {
        'choices': sorted([key for key in predefined_coefficients]),
        'default': None,
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
        'default': 0.06,
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
        'default': 10000,
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


class CommandLineParser(ArgumentParser):
    '''Handler for arguments given to the command line, derived from
    ArgumentParser

    The __init__ method is overwritten to include the options given in the 
    upper options dictionary.
    '''
    
    def __init__(self):
        description = '''
        This Python program simulates the Gray-Scott model for pattern 
        formation. The model consists of two nonlinear, coupled partial
        differerntial equations for the chemical species u and v, namely
        
            ∂u/∂t = Du·∇²u - u·v² + F·(1 - u) and
            ∂v/∂t = Dv·∇²v + u·v² - (F + k)·v,
        
        together with periodic boundary conditions and some initial values.
        Du, Dv are diffusion coefficients, F is the feed rate and k the rate
        constant of the second equation.
        '''
        epilog = '''
        Note that a given '-coefficients <string>' will overwrite any values
        given by -Du, -Dv, -F and -k.
        '''

        ArgumentParser.__init__(self, description=description, epilog=epilog)
        for key in options:
            self.add_argument(key, **options[key])

    def parse_args(self):
        settings = ArgumentParser.parse_args(self)
        if settings.coefficients is None:
            # no -coefficients <string> specified, so use other parameters
            coeffs = {
                'Du': settings.Du,
                'Dv': settings.Dv,
                'F': settings.F,
                'k': settings.k
            }
            settings.coefficients = coeffs
        else:
            # use given coefficients setting instead of -Du etc.
            coeffs = predefined_coefficients[settings.coefficients]
            settings.coefficients = coeffs
        return settings
