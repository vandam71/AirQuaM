# AirQuaM handler

__author__ = "Rafael Samorinha"
__version__ = "1.0.0"

import warnings
import os

warnings.filterwarnings("ignore")           # ignore messy numpy warnings
os.environ["TF_CPP_MIN_LOG_LEVEL"] = '2'    # tensorflow ignore CPU errors
