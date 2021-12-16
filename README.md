# DWM-CUSTOM

My custom dwm config and install.

Build using `make`, install using `make install`, building on debian requires the following packages:
- `libx11-dev`
- `libxft-dev̀
- `libxinerama-dev̀

For the screenshot utilities to work, you need the following packages (from the debian repos) :
- `imagemagick` (for the `import` command
- ̀`xdotools̀` to get the id of the running window
- `xclip` to copy the screenshot to the clipboard

The associated status bar is [here](https://github.com/etienne-lelouet/dwm-status).

The utilities required to control the volume are [here](https://github.com/etienne-lelouet/pactl-volumectl) .

## TODO :
- use getopt to have proper options for the screenshot utilities
- find some way to issue a warning at compile / run time if the required utilities are not installed (should be done for all utilities)
- update the brightness utility to be more generic (aparently you can control brightness trough xrandr)
- check updategom func in dwm for ordering screens (so focusmon has a predictable behavior based on the X screen layout)
- check that one function I already forgot that sets the cursor size somewhore the `setup()` function