# smrxtoedf

Basic utility to convert smrx files to edf+. 

Build & Run:
- Compile using Visual Studio 2017 provided project
- Run in command line from the runtime folder (thats where the dlls are 
located to avoid any additional setup in local host)
- example :  $local_path\smrxtoedf\runtime> .\smrxtoedf.exe .\input.smrx .\output.edf

Current status: 
- loading properly smrx files 
- writing out edf+ files

TODO:
- test with further inputs
- review physical vs digital data in edf+ (by using S64ReadSh and writing digital data, we might save space and avoid rounding errors)

