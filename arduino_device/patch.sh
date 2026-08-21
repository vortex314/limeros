CORE_DIR="$HOME/.platformio/packages/framework-energiativa/cores/tivac"

# 1. Fix srandom signature in WMath.cpp
sed -i '' 's/extern void srandom(unsigned long __seed);/extern void srandom(unsigned int __seed);/g' "$CORE_DIR/WMath.cpp"

# 2. Fix srandom signature in random.c
sed -i '' 's/srandom(unsigned long seed)/srandom(unsigned int seed)/g' "$CORE_DIR/random.c"

# 3. Add forward declarations for ltoa/ultoa in itoa.c
sed -i '' 's/extern char\* ltoa/char* ltoa/g' "$CORE_DIR/itoa.c"
sed -i '' 's/extern char\* ultoa/char* ultoa/g' "$CORE_DIR/itoa.c"

# 4. Ensure itoa/ltoa/ultoa declarations are in WString.h
if ! grep -q "char\* ltoa" "$CORE_DIR/WString.h"; then
  sed -i '' '1s/^/#ifdef __cplusplus\nextern "C" {\n#endif\nchar* itoa(int, char*, int);\nchar* ltoa(long, char*, int);\nchar* ultoa(unsigned long, char*, int);\n#ifdef __cplusplus\n}\n#endif\n/' "$CORE_DIR/WString.h"
fi