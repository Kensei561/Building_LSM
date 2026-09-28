I installed CMake through its official site. 
 
To understand what CMake is, I had to understand what build files and build systems are. This is what I have learnt so far: 
When the no. of files of a program as well as the density of the web of their mutual dependencies increase, 
1. manually writing the commands for their compilation becomes very tedious (as the no. of files can go as high as tens of thousands), 
2. changes made in even one of those files will inevitably and unnecessarily require all the files to be compiled again.

A build system helps with many other problems, including the aforementioned. What you now do, is describe relationships between targets rather than spelling out every compiler and linker action yourself; such low level actions will now be taken care of by the build system. 
Different OS have distinct build syntax corresponding to each of the OS which increases overhead for the build script writer. What CMake adds to this is that it eliminates the syntactical overhead. 

Here are the keywords I encountered, and their deciphered meanings: 
1. cmake_minimum_required(VERSION 3.16): Tells the system the minimum version of CMake needed to parse this file so it doesn't try to use outdated build rules
2. project: Specifies the name of the project.
3. ${PROJECT_NAME}: puts the name of the project in whichever line this placeholder is used. This way of syntax(${..}) is how you declare all the other variables in CMakeLists. 
4. add_executable(<name of the executable> <src files>): Specifies the name and the constitution of an executable.
5. add_library(<name of the library> <src files>): Specifies the name and the constitution of a library.
6. target_link_libraries(<target> <name of the library>): Declares the dependency of the target on the written library and should be linked with it when built. 
7. add_subdirectory(<name of the directory>): Prompts CMake to read the CMakeList in another folder and make the targets in that CMakeList a part of the current build. Here, the keyword was used to access lib.cpp and lib.h as they were stored in another directory.
8. target_include_directories(<target> <visibility scope> <directory to be searched>): 
Tells CMake which directories should be searched when looking for header files for a particular target.
What are visibility scopes?
Visibility scopes determine which targets can use a particular header search directory. There are 3 of them:
    1. PUBLIC: Allows both the target and its dependents to use the specified header search directory.
    2. PRIVATE: Allows only the target to use the specified header search directory.
    3. INTERFACE: Allows only the dependents(not the target) to use the specified header search directory.
9. option: Declares a boolean which can be named, described and can be set default on or off. if(), else, endif() or other functions may follow afterwards to specify to what follows if the option is off or on.
10. message(".."): Is like the 'cout' for CMake; it can be used to mark points of completion or declare the status quo etc while building.

Some additional things I observed and noted down:
- System libraries are not automatically compiled; they need to be mentioned to CMake first for it to build without error. One can type "cmake <name of the package>" on Google in order to know the how the package must be declared in a CMakeList.
- CMake also allows you to choose which C++ standard compiles your code using the set() keyword.
- Once you reconfigure a CMakeLists.txt file, you must input 'cmake ..' again before doing 'cmake --build . ', else the build files will not register the change, and terminal will display the previous error again.


Here is the workflow representing how I completed the build process of a simple program:

Open Terminal
    ↓
Navigate to the project directory
    ↓
code CMakeLists.txt
    ↓
Write project(<project name>)
    ↓
Write add_subdirectory(<name of the directory>)
    ↓
Write add_executable(<name of the executable> <src files>)
    ↓
Write target_link_libraries(<target> <name of the library>)
    ↓
Save CMakeLists.txt
    ↓
Open the library's CMakeLists.txt
    ↓
Write add_library(<name of the library> <src files>)
    ↓
Write target_include_directories(
        <target>
        <visibility scope>
        <directory to be searched>
    )
    ↓
Save CMakeLists.txt
    ↓
mkdir build
    ↓
cd build
    ↓
cmake ..
    ↓
cmake --build .
    ↓
Compile library source files
    ↓
Build library
    ↓
Compile executable source files
    ↓
Link executable with library
    ↓
Generate executable/library

In the process of learning how to write CMakeLists, many questions occured to me. Here is an elaboration on each one of them:
Q: What is the difference between a project and an executable?
A: A project is what contains the constituent executable(s). An executable is a program which we get after the compilation and the linking of its constituent files. A project can consist of more than one executable. 

Q: Why is it better to store build files in a separate folder rather than in the same folder as the src files?
A: Mainly for two reasons:
   1. The parent directory stays clean, and so does your git repository. 
   2. The source files are original, while the build files can be regenerated. If the build files need to be deleted because say they get corrupted, this can simply be done by deleting the build folder. 

Q: What does 'linking' exactly mean in this context?
A: After turning .cpp files into .obj etc, files who contain the code of functions need to be connected to those who require those functions. This connection is essentially what is meant by linking. 

Q: If .cpp file and the header of that library are in the same folder, why does the terminal give an error? And why did the Youtuber use INTERFACE mode inside the target_include_directories even though the library itself needs the header?
A: 
1. The error is caused not by the lib.cpp, but by the main.cpp file since he had just moved the header file into a separate 'lib' directory. 
2. Using INTERFACE didn't cause an error, as since the lib.cpp and lib.h were in the same directory, lib.cpp was able to access the  lib.h file without any external help.

cmake --version
code(or notepad) CMakeLists.txt
ls -Force 
