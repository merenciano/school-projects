Instructions to compile the project:  
Execute the right script according to your SO and architecture and then compile the project created to make the binary.  
In order to change the base speed (5 m/s is veeeeery slow), change the constant value kBaseSpeed (include/agent.h) from 5 to something like 60 before compiling.  
In windows you will need the .dll stored in bin/Debug (is the path where the executable will be).
Run the executable from the folder where it is stored (bin/Debug) because paths are relative to this point (executing from visual studio solution will not load the images).
Doxygen documentation can be created from this project.
