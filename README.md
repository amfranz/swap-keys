# swap-keys

`swap-keys` is an Interception Tools plugin that swaps arbitrary pairs of keys.

For more information about the [_Interception Tools_][interception-tools], check
the project's website.

## Build instructions

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Specify any number of pairs of named keys to swap as command line arguments.

The list of key names can be found in the libevdev source code, look for
constants with the prefix `KEY_`:

- <https://github.com/freedesktop/libevdev/blob/master/include/linux/linux/input-event-codes.h>

A typical use case is to swap WIN and ALT keys. A suggested `udevmon` job
configuration for this is:

```yaml
- JOB: "intercept -g $DEVNODE | swap-keys KEY_LEFTALT KEY_LEFTMETA KEY_RIGHTALT KEY_RIGHTMETA | uinput -d $DEVNODE"
  DEVICE:
    NAME: Apple Inc. Apple Keyboard
    EVENTS:
      EV_KEY: [KEY_LEFTALT, KEY_RIGHTALT, KEY_LEFTMETA, KEY_RIGHTMETA]
```

## License

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

[interception-tools]: https://gitlab.com/interception/linux/tools
