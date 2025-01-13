```markdown
# Injection Program

This project demonstrates a C program (`injection.c`) with a `Makefile` to dynamically compile and execute binaries derived from external binary files. It uses `xxd` to convert binary files into C arrays, which are then compiled into an executable capable of temporarily creating and executing these binaries.

## Project Structure

- **`injection.c`**: Main program that extracts binaries, writes them to temporary files, and executes them.
- **`Makefile`**: Automates the process of converting binaries into arrays, compilation, and cleanup.
- **Directories**:
  - `ssl/`: Contains the source for `prog1`.
  - `base_program/`: Contains the source for `prog2`.
  - `certs/`: Contains certificate and key files for OpenSSL libraries.

## Prerequisites

Ensure the following tools and libraries are installed on your system:
- **GCC**: C compiler.
- **xxd**: Utility to convert binary files into C arrays.
- **OpenSSL**: Provides `libssl` and `libcrypto` libraries.
- Binary files `prog1` and `prog2` placed in their respective directories (`ssl/` and `base_program/`).

## Compilation and Execution

### Steps

1. **Place your binary files** in their respective directories:
   - `ssl/prog1`
   - `base_program/prog2`

2. **Compile everything** by running:
   ```make all```

3. **Run the generated program** using:
   ```make run```
   > **Note**: Running the final executable may require `sudo` privileges.

4. **Clean up generated files** (temporary and object files) using:
   ```make clean```

## Program Operation

The program works as follows:
1. **Binary Conversion**: `prog1` and `prog2` are converted into C arrays using `xxd`.
2. **Code Integration**: The arrays are embedded into the `injection.c` program.
3. **Temporary Files**: The program writes these arrays into temporary files in the `.config/` directory.
4. **File Permissions**: Execution permissions are assigned to these temporary files.
5. **Program Execution**: Two separate processes are created to execute:
   - `prog1`
   - `prog2`
6. **Cleanup**: Once the binaries finish executing, the temporary files are deleted.

## Key Variables in the Makefile

- **`PROG1_BIN`** and **`PROG2_BIN`**: Names of the binary files.
- **`CERT_DIR`**, **`CERT_FILE`**, **`KEY_FILE`**: Paths for OpenSSL certificate-related files.
- **`CFLAGS`**: Compilation flags used during program building.
- **`LIBS`**: Libraries linked during compilation.

## Security Considerations

- Temporary files are created in the `.config/` directory. Ensure this directory is secure and properly managed.
- Only use trusted and verified binaries to avoid security risks.
- Execution privileges may require administrative access. Handle privileges responsibly.

---

## Author

**TheGreenJoker**
