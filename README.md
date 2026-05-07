# HDLC Parser

HDLC protocol bit-stream parser that reads binary data, extracts HDLC frames with CRC validation, and outputs to SIG or PCAP formats.

## Build

```bash
mkdir -p build
cd build
cmake ..
make
```

**Requirements**: C++20 compatible compiler

## Usage

```bash
./hdlc_parser [OPTIONS]
```

### Options


| Option                     | Description                                                          | Default     |
| -------------------------- | -------------------------------------------------------------------- | ----------- |
| `--input-file <path>`      | Input binary filepath                                                | input.bin   |
| `--output-sig <path>`      | Output SIG format filepath                                           | output.sig  |
| `--output-pcap <path>`     | Output PCAP format file                                              | output.pcap |
| `--read-chunk-size <size>` | Chunk size (allowed postfixes - `b`, `kb`, example - `1024b`, `4kb`) | 1024b       |

### Examples

```bash
# Parse input.bin, write to output.sig
./hdlc_parser --input-file input.bin --output-sig output.sig  

# Parse with both formats
./hdlc_parser --input-file data.bin --output-sig data.sig --output-pcap data.pcap

# Custom chunk size
./hdlc_parser --input-file data.bin --read-chunk-size 8kb --output-sig result.sig
```

## Output Formats

**SIG**: 2-byte big-endian frame length header + frame payload 

**PCAP**: Standard PCAP format (magic 0xa1b2c3d4), suitable for import to Wireshark
