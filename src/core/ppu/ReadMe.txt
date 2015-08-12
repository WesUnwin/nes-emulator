========================================================================
    STATIC LIBRARY : PPU Project Overview
========================================================================

This project producs a static library (PPU.lib) designed to implement the functionality
of an NTSC Nintendo Entertainment System Picture Processor Unit.

The PPU Class (in PPU.cpp) acts as a central interface that encapsulates ALL the functionality
of this PPU Core. An emulator instantiates an instance of this class, representing the PPU,
then advances the PPU a desired number of clock cycles, reads/writes to its 8 ports then 
clocks it some etc. 

This PPU Core implementation is not designed to be optimal in terms of running performance
but simply strives to be 100% technically accurate, and have a clean simple architecture.
This PPU Core serves as a proof of concept to ensure the PPU's functionality is 100% understood
correctly.

/////////////////////////////////////////////////////////////////////////////
