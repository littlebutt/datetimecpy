from setuptools import setup, Extension

module1 = Extension('datetimecpy',
                    sources = ['datetimecpy.c'])

setup (name='datetimecpy',
       version='1.0',
       author='littlebutt',
       author_email='luogan1996@icloud.com',
       license='MIT',
       description = "The datetime API",
       url='https://github.com/littlebutt/datetimecpy',
       ext_modules=[module1])