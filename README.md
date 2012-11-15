Java connector for Metatrader (MT4)

This simple application shows you how to write 
your own extension for metatrader (EA, indicator, library) in JAVA.
The JAVA code is executed in a virtual machine which runs inside the terminal.exe (MT4) process. 

Repository folders:

1. java folder

Contains a sample eclipse project with a simple EA implemenation in JAVA.

2. mt4 folder

Contains the MT4 EA code, which calls the JAVA implementation.

3. vc folder

Contains a Visual Studio project with the sources for the MT4 Java connector.


Steps to run this application

1. Copy the experts folder from your mt4 source folder to your Metatrader installation directory <MT4 directory>

2. Open Visual Studio project and build (F7) DLL.
   Important: Please check the path for your JAVA SDK installation (Project settings -> VC++ directories - Include directories)
   
3. Copy DLL to your Metatrader installation directory <MT4 directory>/exterts/libraries

4. Open the eclipse project and export the project as JAR file.

5. Copy the JAR file to <MT4 directory>/exterts/libraries

6. Edit JVM configuration file <MT4 directory>/exterts/libraries/mt4j.properties and adjust the path to 
   your JVM dll and your JAR file.

7. Restart MT4. In the navigator panel under "Experts" you should now see the mt4jSample EA

