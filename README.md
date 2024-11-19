About cscript
=============
cscript is a simple tool that enables the user to run c source code like a script file. Just add a proper shebang line, usually #!/usr/local/script as the first line of the c-file.
If necessary, add a second line starting with #gcc to provide additional command line arguments to gcc.

Then just make the c-file executable and you can run it from the command line like any script file. I also propose to change the extension to .cscript to avoid confusion with normal c source code.

When a c-script is called for the first time, cscript will compile the c-code with gcc and will store the output in a cache directory: ~/.cscript/cache/{hash of c file}/{c file}.bin.

It also saves the hash of the c script so it will do a new compilation only when the c source has changed.

Only one-file sources can be used, but libraries can be linked through the command line arguments provided in the aforementioned #gcc line.

## Example
A c-script that generates a time based uuid using libuuid.

```c++
#!/usr/local/bin/cscript
#gcc -luuid
//The actual c ciode starts here:
#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>

int main() {
    uuid_t uuid;
    char uuid_str[37];
    uuid_generate_time_safe(uuid);
    uuid_unparse_lower(uuid, uuid_str);
    printf("uuid=%s\n", uuid_str);
    return 0;
}
```
If you like this little tool and want to give something back, please send bug-reports or add PRs with bug fixes.

<b>Please note: This is a hobby project, created just for fun, so do not expect the reaction speed of a full-time development team.</b>

Building and Installing cscript
===============================

## How To Build
I provided a simple CMakeLists.txt file as well as a makefile.

### CMake

1. Create a build directory somewhere (.e.g. ~/build/cscript).
2. cd into that directory.
3. Call cmake {path-to-the-source-code}
4. Call make
5. Call make install with root access

### make

1. Create a build directory somewhere (.e.g. ~/build/cscript).
2. Copy the source files (.c, .h and the makefile) in to that directory. 
3. cd into that directory.
4. Call make
5. Call make install with root access

After the installation the executable will be install to /usr/local/bin by default.

License
=======
The tool is licensed under GPL v2.0, see the file LICENSE for the full license.
