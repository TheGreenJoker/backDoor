# Secure Client-Server Communication Project

## Description
This project implements secure client-server communication using SSL/TLS. The server is implemented in C with OpenSSL, while the client is written in Python using the `ssl` library. This project is designed to provide a foundation for encrypted communication between a client and server over an unsecured network.

## Features
- Encryption of data transmitted between the client and the server.
- Mutual authentication via SSL/TLS certificates.
- Execution of commands on the server via client requests (caution: to be used only in a controlled environment).

## Project Structure

### Main Files
- `server.c`: Implements the server using OpenSSL to manage secure connections.
- `clien.py`: Implements the client for connecting to the server via SSL.
- `makefile`: Automates the server compilation process.

### File Details
#### `server.c`
- Initializes a listening server on port 8520.
- Loads certificates and private keys directly from hexadecimal data.
- Allows the client to send commands, which are executed on the server, and the results are sent back to the client.

#### `clien.py`
- Connects to the server on port 8520.
- Uses a certificate to authenticate the server.
- Allows the user to send messages or commands to the server.

#### `makefile`
- Simplifies the compilation and cleaning of the project.

## Prerequisites
- OpenSSL installed on the system.
- Python 3 with the standard `ssl` library.
- A certificate for both the client and server.

## Installation Instructions
1. Clone the project repository:
   ```bash
   git clone <repository_url>
   cd <project_name>
   ```

2. Compile the server:
   ```bash
   make
   ```

3. Generate certificates (if needed):
   ```bash
   openssl req -new -x509 -days 365 -nodes -out server.crt -keyout server.key
   openssl req -new -x509 -days 365 -nodes -out client.crt -keyout client.key
   ```

## Usage

### Run the Server
1. Launch the compiled server:
   ```bash
   ./server
   ```

### Run the Client
1. Run the Python client:
   ```bash
   python clien.py
   ```

2. Enter a command or message to send to the server. For example:
   ```
   > ls
   > exit # stop the connexion
   > stop # stop the sever
   ```

## Security Precautions
- **Command execution**: The command execution feature on the server presents security risks. Use it only in a controlled environment.
- **Certificates**: Ensure the certificates are correctly generated and secured.

## Limitations
- Hostname verification is disabled in the Python client. It should be enabled for production deployment.
- The server shuts down immediately, without sophisticated signal or thread management.

## Future Improvements
- Add user authentication.
- Implement advanced logging to monitor server activity.
- Improve error handling.

## Author
TheGreenJoker
