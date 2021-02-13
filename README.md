## SWTC: The Standard Widget Toolkit for C/C++
SWTC is an open source widget toolkit for C/C++ designed to provide efficient, portable access to the user-interface facilities of the operating systems on which it is implemented.

## Usage
```c++
#include <swtp.h>
int main() {
	WApp app;
	WShell *shell=new WShell();
	shell->Create(W_SHELL_TRIM | W_DISPOSE_ON_CLOSE | W_DELETE_ON_DISPOSE);
	shell->Open();
	return app.Run();
}
```
More information can be found here:

 - [Help][1]
 - [Wiki][2]

## Building SWTC on Linux
To build swtc on your computer you will need these packages:

 - The GTK development packages: often called libgtk-3-dev or gtk3-devel, or similar
 - pkg-config (which usually comes with the GTK dev package)
 - The build-essential package (or the relevant bit of it: G++, Make etc)
 - CMake
 - Git.

On Ubuntu / Debian you can install all of the above by typing:

    sudo apt-get install libgtk-3-dev pkg-config build-essential git cmake

Git clone the sources:

    git clone https://github.com/elbassbasi/swtc.git

Run CMake and build swtc:

  ```bash
  cd swtc
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j 4
  sudo cmake --build . --target install
  ```
## Building SWTC on Windows
To build swtc on your computer you will need these packages:

 - The build-essential package (or the relevant bit of it: G++, Make etc)
 - CMake
 - Git.

Git clone the sources:

    git clone https://github.com/elbassbasi/swtc.git
    
Run CMake and build swtc:

  ```bash
  cd swtc
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j 4
  ```
----------

Building SWTC on macOS
----

Prerequisites:

 - CMake
 - Git
 - Xcode
 - Xcode command-line tools
 - Homebrew.

Preparation:
 - Install Xcode from Mac App Store
 - Install Xcode command-line tools: `xcode-select --install`
 - Install Homebrew:
```
 ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
 - Update Homebrew: `brew update`
 - (Optional) Upgrade Homebrew packages: `brew upgrade`
 - Install Git: `brew install git`
 - Install CMake: `brew install cmake`


Clone the repo (lets assume that you have a folder `/Users/YOU/src`)

  ```bash
  git clone https://github.com/elbassbasi/swtc.git
  ```

 To build swtc:

  ```bash
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j 4
  cmake --build . --target install
  ```
  [1]: https://elbassbasi.github.io/swtc
  [2]: https://github.com/elbassbasi/swtc/wiki
