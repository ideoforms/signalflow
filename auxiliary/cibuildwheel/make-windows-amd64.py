#!/usr/bin/env python

import os
import re
import glob
import subprocess

os.environ["CIBW_BUILD"] = "cp*-win_amd64"
os.environ["CIBW_ARCHS_WINDOWS"] = "AMD64"

rv = subprocess.call(["python", "-m", "cibuildwheel"])
if rv == 0:
    os.chdir("wheelhouse")
    wheels = glob.glob("*.whl")
    for wheel in wheels:
        print(" - %s" % wheel)
        # NOTE: cibuildwheel seemed to be formerly creating wheels labelled
        # as -win32, this conditional is to catch and correct this.
        if re.search("-win32.whl", wheel):
            wheel_corrected = re.sub("-win32.whl", "-win_amd64.whl", wheel)
            os.rename(wheel, wheel_corrected)
            wheel = wheel_corrected
            print("   - renamed to %s" % wheel)
        print("   - running delvewheel")
        subprocess.call(["python", "-m", "delvewheel", "repair", wheel, "--add-path", "dlls"])
else:
    print("Failed to run python -m cibuildwheel")