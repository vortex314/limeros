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

# 1. Patch WMath.cpp: srandom() signature must match newlib stdlib.h's
#    `void srandom(unsigned)`; Energia's `unsigned long` version conflicts.
patch_file(
    join("cores", "tivac", "WMath.cpp"),
    r"extern\s+void\s+srandom\(unsigned long __seed\);",
    "extern void srandom(unsigned int __seed);"
)

# 2. Patch random.c: keep its srandom() definition in sync with the header.
patch_file(
    join("cores", "tivac", "random.c"),
    r"srandom\s*\(\s*unsigned\s+long\s+seed\s*\)",
    "srandom(unsigned int seed)"
)

# 3. Provide a _fini stub in startup_gcc.c: the titiva platform links with
#    -nostartfiles, so crtn.o (which normally defines _fini) is omitted,
#    leaving an undefined reference from newlib's __libc_fini_array.
#    (_init is already defined by the framework's cores/tivac/main.cpp.)
startup = join("cores", "tivac", "startup_gcc.c")
if isfile(join(FRAMEWORK_DIR, startup)):
    spath = join(FRAMEWORK_DIR, startup)
    with open(spath, "r") as f:
        content = f.read()
    if "void _fini(void) {}" not in content:
        with open(spath, "a") as f:
            f.write(
                "\n/* Patched by patch.py: _fini stub (crtn.o not linked due to "
                "-nostartfiles) */\n"
                "void _fini(void) {}\n"
            )
        print(f"[Energia Patch] Added _fini stub to {startup}")

# Note: -D_ITOA_ was removed from build_flags because it pre-defines the
# include guard of cores/tivac/itoa.h, silently disabling that header and
# leaving ltoa()/ultoa() undeclared.