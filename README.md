FlyBlind
========

This X-Plane plugin disables drawing of the outside world.

 - Boost framerates
 - Fly IFR

When the command is triggered, the plugin registers drawing callbacks for most
of X-Plane's scenery rendering phases. The callback renders a solid colour
instead of scenery.

The command is `Albair/FlyBlind/toggle`

Lights are still rendered, including the sun at some angles. Disabling the
drawing phase where they're rendered also causes visual glitches.

Compiled using Philipp Ringler's Plugin Patterns Library, as modified by me.
This is included as a git submodule.

Project is arranged to be built using qmake to compile PPL as a static library.

Developed on Linux for X-Plane 10.
