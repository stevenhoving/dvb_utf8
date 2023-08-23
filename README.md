# dvb_utf8

This repository contains my efforts to re-engineer the dvb information data stream serialization and deserialization for OpenPLI.

## Testing and production readyness
This code has neven been tested with real data.

### The original (test) plan
* Inside OpenPLI create a way to dump dvb over tcp/udb.
* Create an application that receives the data and stores stores it to file.
* Create an application that reads the dvb stream data from file and parse using this library.

This would allow me (or anyone else) to properly test this library.
