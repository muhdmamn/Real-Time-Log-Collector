# Real Time Log Collector

A simple C program that runs a command for a specified duration and logs its output with timestamps.

## Features

- Runs any command with arguments
- Sets a duration for the command
- Collects output in real-time
- Adds timestamps to log entries
- Logs if timer goes off

## Build
- ```bash
   make
```

## Usage
 ```bash 
 # RTLOG <Alarm-Seconds> <Command> <arguments>
 ```
 ### Example

Run `ping google.com` for 5 seconds:

```bash
./build/RTLOG 5 ping google.com
```

The output will be logged to `build/Logs.txt` with timestamps.

## Output Format

Each log entry is formatted as:

```
<timestamp> --- <command_output>
```

When the timer expires, the following is logged:

```
<timestamp> Alarm reached
```

## Cleaning

To remove the build artifacts:

```bash
make clean
```

## Structure

```
.
├── src/           # Source code
├── build/         # Compiled binaries and logs
├── docs/          # Documentation
├── tests/         # Test files
└── Makefile       # Build configuration

├── src/           # Source code
├── build/         # Compiled binaries and logs
├── docs/          # Documentation
├── tests/         # Test files
└── Makefile       # Build configuration
```

## Development Notes 
This project was developed as a learning project focusing on:
- Pipes and Redirection
- Signals and Timers
 
 

