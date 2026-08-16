# Patcth Energia framework files to fix compilation issues with C++17 and newer compilers.
# root errors 
# 

import os
import re
from os.path import join, isfile

Import("env")

FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-energiativa")

def patch_file(rel_path, pattern, replacement):
    if not FRAMEWORK_DIR:
        return
    file_path = join(FRAMEWORK_DIR, rel_path)
    if not isfile(file_path):
        return
    with open(file_path, "r") as f:
        content = f.read()
    if re.search(pattern, content):
        patched = re.sub(pattern, replacement, content)
        with open(file_path, "w") as f:
            f.write(patched)
        print(f"[Energia Patch] Successfully patched {rel_path}")

# 1. Patch random.c (srandom signature)
patch_file(
    join("cores", "tivac", "random.c"),
    r"srandom\s*\(\s*unsigned\s+long\s+seed\s*\)",
    "srandom(unsigned int seed)"
)

# 2. Patch WMath.cpp (redundant extern prototypes)
patch_file(
    join("cores", "tivac", "WMath.cpp"),
    r"extern\s+void\s+srandom\(unsigned long __seed\);",
    "// patched: extern void srandom(unsigned long __seed);"
)

# 3. Patch itoa.h (utoa duplicate prototype)
patch_file(
    join("cores", "tivac", "itoa.h"),
    r"extern\s+char\*\s+utoa\(\s*unsigned long value,\s*char \*string,\s*int radix\s*\)\s*;",
    "// patched: extern char* utoa(unsigned long, char*, int);"
)