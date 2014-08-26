# -*- coding: utf-8 -*-


predefined_coefficients = {
    'bacteria1' : { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.065 },
    'unstable' : { 'Du': 0.16, 'Dv': 0.08, 'F': 0.02, 'k': 0.055 },
    'zebra': { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.06 }
}


class CommandLineParser(object):
    '''Parser to convert command line arguments to a settings dictionary for
    the Gray-Scott problem.
    '''
    
    defaultsettings = {
        'coefficients': { 'Du': 0.16, 'Dv': 0.08, 'F': 0.035, 'k': 0.06 },
        'export': False,   
        'keepalive': True,
        'outputsteps': 100,
        'show': True,
        'size': 256,
        'timesteps': 10000
    }

    @staticmethod
    def parse(arguments):
        settings = CommandLineParser.defaultsettings.copy()
        try:
            for option in arguments:                                
                if option.lower() in ('help', '-help', '--help'):
                    CommandLineParser.usage()
                    return None
                key, value = option.split('=', 1)
                if key in ('Du', 'Dv', 'F', 'k'):
                    settings['coefficients'][key] = float(value)
                elif key == 'coefficients':                    
                    settings['coefficients'] = predefined_coefficients[value]
                elif key in ('outputsteps', 'size', 'timesteps'):
                    settings[key] = int(value)
                elif key in ('export', 'keepalive', 'show'):
                    settings[key] = True if value.lower() == 'yes' else False
                else:
                    raise ValueError                
        except:
            print('invalid syntax')
            CommandLineParser.usage()
            return None
        return settings

    @staticmethod
    def usage():
        print('''
        Solves the Gray-Scott model for pattern formation. The model consists of
        a system of two coupled reaction-diffusion equations for chemical species
        u and v, namely
        
            ∂u/∂t = Du·∇²u - u·v² + F·(1 - u),
            ∂v/∂t = Dv·∇²v + u·v² - (F + k)·v,      
        
        and is equipped with periodic boundary conditions and specially chosen 
        initial values.

            Usage: python grayscott.py [options]

        Options are specified in the form key=value.
        The following options are available:
    
        coefficients=<value> (default: zebra)
            use the predefined coefficient setting <value> (available settings
            are: bacteria1, unstable, zebra)
        Du=<value> (default: 0.16)
            set the diffusion coeffient for system variable u to <value>
        Dv=<value> (default: 0.08)
            set the diffusion coeffient for system variable v to <value>
        export=<yes/no> (default: no)
            (do not) export png images to ./temp/
        F=<value> (default: 0.035)
            set the feed rate to <value>
        help
            prints this message
        k=<value> (default: 0.06)
            set the rate constant for the second equation to <value>
        keepalive=<yes/no> (default: yes)
            (do not) keep the last intermediate image on screen (option is only
            effective if show=yes)        
        outputsteps=<value> (default: 100)
            set the number of images to show and/or or export to <value>
        show=<yes/no> (default: yes)
            (do not) show  intermediate results as images on screen
        size=<value> (default: 256)
            set the grid size to <value>
        timesteps=<value> (default: 10000)
            set the number of timesteps to calculate to <value>            
        

        The order of parameters Du, Dv, F, k and coefficients is important: 
        Specifying one of Du, Dv, F, k after placing the coefficients option
        will overwrite the corresponding value.
    ''')
