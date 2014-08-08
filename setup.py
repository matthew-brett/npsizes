from distutils.core import setup, Extension

import numpy as np

npsizes = Extension('npsizes',
                    sources = ['npsizes.c'],
                    include_dirs=[np.get_include()])


setup (name = 'smalls',
       version = '1.0',
       description = 'Checks size of numpy objects',
       ext_modules = [npsizes])
