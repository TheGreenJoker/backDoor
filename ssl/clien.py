import socket
import ssl

# Client parameters
HOST = 'localhost'  # Server address
PORT = 8520         # Server port
CA_CERT = 'client.crt'  # Path to the client certificate for server verification

def start_client():
    # Create the TCP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Create SSL context for server authentication
    context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH)
    context.load_verify_locations(cafile=CA_CERT)  # Load CA certificate

    # Disable hostname verification (only for testing purposes)
    context.check_hostname = False

    # Wrap the socket with SSL to secure communication
    with context.wrap_socket(client_socket, server_hostname=HOST) as secure_socket:
        try:
            # Connect to the server
            secure_socket.connect((HOST, PORT))
            print("Connected to the server.")

            while True:
                # Get input message from the user
                message = input("Enter a message (or 'exit' to quit): ")

                # Send the message to the server
                secure_socket.sendall(message.encode())

                # Break if the user types "exit" or "stop"
                if message.lower() == "exit":
                    print("Disconnecting from the server...")
                    break
                if message.lower() == "stop":
                    print("Disconnecting and stopping the server...")
                    break

                # Receive the server's response
                data = secure_socket.recv(1024)
                print(f"Server response:\n\033[32m{data.decode()}\033[0m")

        except Exception as e:
            print(f"Error: {e}")

        finally:
            print("Connection closed.")

if __name__ == "__main__":
    start_client()
