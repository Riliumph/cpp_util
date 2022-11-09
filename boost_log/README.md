# What is this?

This is a Logger system using Boost.Log.

## Preparation

At first, unzip _[Boost/x86/lib/*.zip]_  
Since lib files are too large to exceed the capacity of github, they are zip compressed.

## Log Stream

- logger  
Interface for the application to output logs.  
Define log levels, set spesial attribute -- time stamps, process id, theread id, etc...

- core  
Collect each logger's log elements.  
Filter log elements.  

- Sink - Front end  
Format output style.  
Filter log elements by log level.  

- Sink - Back end  
Set destination.  
ex) If destination is file, set file name.  
Set cyclic trigger - term, file size, and so on.  

details: <http://www.boost.org/doc/libs/1_57_0/libs/log/doc/html/log/design.html>

## How to use

The sample code is put in the main(void).

 Log( [LogLevel] ) << "Log comment";
