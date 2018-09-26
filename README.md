# smrxtoedf

Basic utility to convert smrx files to edf+. 

Build & Run:
- Compile using Visual Studio 2017 provided project
- Run in command line from the runtime folder (thats where the dlls are 
located to avoid any additional setup in local host)
- example :  $local_path\smrxtoedf\runtime> .\smrxtoedf.exe .\input.smrx .\output.edf

Current status: 
- loading properly smrx files 

TODO: 
- complete edf file output
- refactor code to present as c library to be used in third party 
project intead of stand alone program

