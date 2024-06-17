# TS Audio Parser

This repository contains a Transport Stream (TS) parser specifically designed to parse audio streams with a given PID (Program Identifier). The parser extracts audio packets from the TS file and writes the extracted audio data to an output file.

## Features

- Parses TS packets and extracts audio data based on a specified PID.
- Writes extracted audio data to a file in binary format.
- Supports TS packets with adaptation fields.

## Requirements

- C++11 or later
- Your own TS file

## Building the Project

1. **Clone the repository:**

    ```sh
    git clone https://github.com/Seggenz/TS-Parser.git
    cd TS-Parser
    ```

## Usage

1. **Prepare your TS file:**

    Ensure you have a TS file that contains audio streams. Place the file in the project directory or provide the correct path.

2. **Run the parser:**

    ```sh
    ./ts_parser
    ```

    The parser will read the input TS file (e.g., `example_new.ts`), parse the audio packets with the specified PID (default is 136 for audio), and write the audio data to `PID136.mp2`.

