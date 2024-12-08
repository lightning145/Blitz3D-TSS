# Extending Blitz3D TSS

Extending the engine is done by a "Userlib", which is a pair of files:

- A `.dll` file containing the userlib's code.
- A `.decls` file that tells BlitzBasic the module, and the exports of the module.

The `.decls` files are CRUCIAL. Without one, BlitzBasic won't be able to locate the functions you want to use from the DLL file.

## The .decls Format

Each `.decls` file should begin with a `.lib` directive, followed by a whitespace and the quoted name of the DLL file to be loaded. Example:

> .lib "mylib.dll"

Afterwards, you must define a list of functions you want to expose to BlitzBasic. There are a few exceptions:

- No default parameter values are allowed.
- If no function return type is specified, the function is a 'void' function - ie: it returns nothing.
- Instead of object parameters, you can only specify 'void*' parameters using a '*' type tag. Such
parameters can be assigned ANY object or bank, so BE CAREFUL!
- The declaration must be followed by the C decorated name. This takes form of a `:` followed by the quoted decorated name. Example:

```
MyFunction(text$):"_MyFunction@4"
VecDistance#(x1#, y1#, z1#, x2#, y2#, z2#):"_VecDistance@24"
```

To get the list of exports from a DLL file, please check [this article](https://docs.microsoft.com/en-us/cpp/build/reference/dash-exports?view=msvc-170).

## Writing a DLL

Here's a list of rules:

- Functions **MUST** use the `_stdcall` convention.
- Floats are passed and returned as per standard C/C++ conventions.
- Strings **MUST** be in C format (`const char*`), no `std::string` allowed.
- Banks and objects can be passed into functions. The value passed is a pointer to the first byte of storage. No information is passed about the type or size of the memory though, so **PROCEED WITH EXTREME CAUTION**.
- Banks and objects cannot be returned from functions, and arrays are not supported in any way.
- C++ name mangling is not allowed, you must use `extern "C"`.

Now HERE is how we would write a DLL:

1. Open up Visual Studio and create a "Dynamic-Link Library" project.
2. Delete all the files, and turn "precompiled headers" off in the project settings.
3. Write the code.
4. Build it.
5. Create the `.decls` file.
6. Place `MyLib.dll` and `MyLib.decls` in the `userlibs` folder of your Blitz3D TSS installation.
7. Done!

**ATTENTION!** When building a standalone executable for your game, you must place the userlib's DLL alongside the executable (you don't need the declarations file anymore, though).

### Throwing exceptions in DLLs

> [!IMPORTANT]
> This feature is available in Blitz3D TSS v1.334 or above only. Other Blitz3D or BlitzPlus will occur "Unknown runtime exception", no matter what type of exception you threw.

Blitz3D TSS is using [Structured Exception Handling](https://learn.microsoft.com/en-us/windows/win32/debug/about-structured-exception-handling) to receive your exceptions. There are two types of exceptions are available for throwing:

* `0xE0000001` : Throwing a runtime error and crashing the program immediately. Requires runtime error message.
* `0xE0000002` : Throwing an exception which can be shown in the customized MAV message. Requires function name and exception message.

To throw exceptions in DLLs, you need to use [RaiseException function](https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-raiseexception).

For example:
```cpp
ULONG_PTR args[1]{};
args[0] = reinterpret_cast<ULONG_PTR>("This is a runtime error!");
RaiseException(0xE0000001, 0, 1, args);
```
```cpp
ULONG_PTR args[2]{};
args[0] = reinterpret_cast<ULONG_PTR>("Function name");
args[1] = reinterpret_cast<ULONG_PTR>("Exception message");
RaiseException(0xE0000002, 0, 2, args);
```

**ATTENTION!** Make sure that your `const char*` is pointing to the memory which is still available after your function ends.

## Sample Code

**MyLib.h**

```cpp
#pragma once

#include <math.h>
#include <string.h>
#include <stdlib.h>

//Convenience macro!
#define BLITZ3D(x) extern "C" __declspec(dllexport) x _stdcall

BLITZ3D(float) VecDistance(float x1, float y1, float z1, float x2, float y2, float z2);
BLITZ3D(const char*) Ping();
BLITZ3D(void) ErrorFunction();
BLITZ3D(void) MyExceptionFunction();
```

**MyLib.cpp**

```cpp
#include "MyLib.h"

BLITZ3D(float) VecDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dx = x1 - x2
    float dy = y1 - y2
    float dz = z1 - z2;
  
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

BLITZ3D(const char*) Ping()
{
    return "Pong";
}

BLITZ3D(void) ErrorFunction()
{
    ULONG_PTR args[1]{};
    args[0] = reinterpret_cast<ULONG_PTR>("This is a runtime error!");
    RaiseException(0xE0000001, 0, 1, args);
}


BLITZ3D(void) MyExceptionFunction()
{
    ULONG_PTR args[2]{};
    args[0] = reinterpret_cast<ULONG_PTR>("MyExceptionFunction");
    args[1] = reinterpret_cast<ULONG_PTR>("Hello world!");
    RaiseException(0xE0000002, 0, 2, args);
}
```

**MyLib.decls**

```
.lib "MyLib.dll"

VecDistance#(x1#, y1#, z1#, x2#, y2#, z2#):"_VecDistance@24"
Ping$():"_Ping@0"
ErrorFunction():"_ErrorFunction@0"
MyExceptionFunction():"_MyExceptionFunction@0"
```

Hopefully this guide will help you.

If you find any issues with this guide, please open an issue.
