# Project for Computer Communications and Networks (IPK)
Object of project was to develop REST API client and server.

## Implementation

### HTTP Request
HTTP Request is class that can hold request's data such as request type, request path, request headers and body. Class also contains several methods:
 * `ParseRequest(string)` to parses request from string.
 * `ToString()` to serializes request to http format

### HTTP Response
HTTP Response is a class that holds response data - response code, response headers and body. Class contains these methods:
 * `ParseResponse(string)` to parses response from string.
 * `ToString()` to serializes response to http format

### File controller
File controller is class that handles requests and responses and has direct access to files. File controller manipulates files both on server and client side based on combination of response's data, request's data and client's command line parameters.

### Client side
Command line arguments are read and HTTP request is created based on these arguments. TCP connection is created using BSD sockets and HTTP request is sent to socket. After sending request response is read from socket and then parsed. Response is passed to controller that checks response code - if error code is other than 200 then error is written to STDERR depending on requested operation. If operation such as `get` was used then also file from server is written to local file.

### Server side
Server binds selected port on server's address and waits for connections. If connection was created then communication socket is opened and passed to another thread that handles response to client. Request is read from socket and parsed to object. Request is then passed to file controller that runs function that was requested by client. Returns response with relevant return code:
 * 200 OK - If operation was succesful
 * 400 Bad Request - If request was malformed
 * 401 Unauthorized - If user in request doesn't exist
 * 404 Not Found - If manipulation with file that doesn't exist was requested
 * 405 Method Not Allowed - When requested operation wasn't allowed (e.g. deleting non-empty folder)

Response is then serialized and sent back to the socket and socket is closed.
