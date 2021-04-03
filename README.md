# The Fury
> Fan-made game based on the Fury of The Furries

## Tech
Haaf's Game Engine (HGE) v1.81 (included as subrepository)
Box2D physics engine v1.2.1 (included as subrepository)
pugixml (included in main repository as subproject)

## Build
### Requirements
MSVC 2019 (older versions are not tested)
Installed DirectX SDK (https://www.microsoft.com/en-us/download/details.aspx?id=6812)
CMake 3.11 or later

### Script
```bat
git clone --recurse-submodules https://github.com/Necrys/Fury.git
cd Fury
mkdir build
cd build
cmake .. -DDXSDK_PATH='f:/programming/dx9sdk' -A Win32
cmake --build . --config Release
```

## Credits
### Basic idea:
Alexander Kulinich (necrysgd@gmail.com)
Andrey Kostrov (fur.sapfil@gmail.com)

### Programming:
Alexander Kulinich (necrysgd@gmail.com)
Andrey Kostrov (fur.sapfil@gmail.com)

### Graphic art:
Andrey Kostrov (fur.sapfil@gmail.com)
