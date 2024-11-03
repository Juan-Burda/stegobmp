# stegobmp

## Running the project

### Compiling normally

In order to compile the project, first run:

```bash
sudo apt update
sudo apt install gcc libssl-dev
```

After that, run `make all` from the root directory.

### Compile using Docker

To run the project normally run:

```bash
./stegobmp.sh [args]
```

### Debugging

If you want to run the project with a GDB terminal run:

```bash
./debug.sh
```

This will open a GDB terminal. To get help on GDB commands type `help`.

## Program arguments

| Parameter  | Description                                                             |
| ---------- | ----------------------------------------------------------------------- |
| `-extract` | Extraction mode                                                         |
| `-embed`   | Embedding mode                                                          |
| `-in`      | Payload to hide                                                         |
| `-p`       | Image which will hide the payload (must be a BMP file)                  |
| `-out`     | Output file                                                             |
| `-steg`    | Steganography method (`<steganography_method>`: LSB1, LSB4, LSBI)       |
| `-a`       | Encryption method (`<encryption_method>`: AES128, AES192, AES256, 3DES) |
| `-m`       | Chaining mode (`<mode>`: ECB, CBC, CFB, OFB)                            |
| `-pass`    | Password for encryption(`<password>`)                                   |
