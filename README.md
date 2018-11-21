#### Preamble ####

My personal hackings on the Gish source release.

The primary goal of this "fork" is to modernise the engine,
cleaning up hacky code & fixing bugs while keeping the original game untouched.
Though enhancements are not off the table, changes are usually made with later possibilities in mind.

Like the original, this project is GPLv2 licensed.

#### What's in? ####

- Bugfixes, haven't kept count but I'll fix them when I see them.
- Ported the engine from the aging SDL1.2 to SDL2, fixing issues on modern Linux WMs.
- Complete overhaul of the video options;
    supporting various exclusive fullscreen modes,
    and borderless "fake" fullscreen for those who like that sorta thing.
    
#### What's planned? ####

- Porting the filesystem to PhysFS.
  - Allow the game data to be loaded from a compressed zip.
  - Users should be able to mount extra data over the original for easy modding.
  - Use the user's config folder for configuration/saves/replays instead of the program directory.
  - Support the demo version's assets, and freegish too potentially.
- Support for PNG textures.
- Load backgrounds as NPOT textures on video cards that support them.
- Extra video options:
  - Remove border/Minimal HUD.
  - Anti-aliasing.

#### Stuff to think about ####

- The joystick API still works fine, but should I port to SDL2's GameController API?
- Should test and aim for replay compatibility with the original executable.
- Would be nice to abtract away the OpenGL code an allow for alternative renderers.

#### References ####
- [Github page](https://github.com/ScrelliCopter/gish)
- [Original source drop](https://crypticsea.blogspot.com/2010/05/gish-open-source.html)
