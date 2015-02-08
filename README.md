# grayscott

Python implementation (and a C++ port of the program) of the Gray-Scott model for pattern formation. For details about the mathematical aspects see http://www.sciencemag.org/content/261/5118/189.full.pdf.

## Usage

### Python version

The Python version is located in the `Python/` subfolder. For it to run you need a [Python 2.7](http://www.python.org) installation together with the modules [NumPy](http://www.numpy.org) and [matplotlib](http://www.matplotlib.org). The program itself can then be run simply from the command line via ./grayscott.py (you may need to make that script executable via `chmod +x grayscott.py`). **Please to not execute the `grayscott.py` from the root directory of this repository since this has a different purpose!**

### C++ version

The C++ version is implemented with the Qt framework. If you are under a Linux environment you may be able to compile the program directly via a `make` in the root directory of the repository. You will need a recent verion of the Qt framework with header files to do this. Running the resulting `grayscott` executable will give you the program GUI.
