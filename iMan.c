#include "iMan.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void remove_pre_tags(char *input) {


    char *pre_start = strstr(input, "<PRE>");
    char *pre_end = strstr(input, "</PRE>");

    if (pre_start && pre_end) {
        // Calculate the length of the text between <PRE> and </PRE>
        size_t pre_length = pre_end - pre_start + 6; // 6 is the length of "</PRE>"

        // Copy the text between <PRE> and </PRE> back to the beginning of the input string
        memmove(input, pre_start, pre_length);

        // Null-terminate the string after the copied text
        input[pre_length] = '\0';
    } else {
        // If <PRE> and </PRE> are not found, clear the entire string
        input[0] = '\0';
    }

    char *start, *end;
    int tag_len;

    // Remove opening <STRONG><A ...>

    // Loop until no more '<' is found
    while ((start = strchr(input, '<')) != NULL) {
        // Find the closing '>'
        end = strchr(start, '>');
        if (end) {
            // Remove everything between '<' and '>'
            memmove(start, end + 1, strlen(end + 1) + 1);
        } else {
            // If closing '>' not found, break
            break;
        }
    }
}

int iMan(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s <command_name>\n", argv[0]);
        exit(1);
    }

    char *command_name = argv[1];

    // Perform DNS resolution to get the IP address of man.he.net
    struct hostent *server_info = gethostbyname("man.he.net");
    if (server_info == NULL) {
        error("Error in DNS resolution");
        return 0;
    }

    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
        return 0;
    }

    // Set up the server address structure
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    bcopy((char *)server_info->h_addr, (char *)&server_addr.sin_addr.s_addr, server_info->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error connecting to server");
        return 0;
    }

    // Send a GET request for the man page
    char request[BUFFFER_SIZE];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);
    if (write(sockfd, request, strlen(request)) < 0) {
        error("Error writing to socket");
        return 0;
    }

    // Read the response and copy it into a string
    char buffer[BUFFFER_SIZE];
    char *response = NULL; // This will store the entire response
    size_t response_size = 0;

    int n;
    while ((n = read(sockfd, buffer, sizeof(buffer))) > 0) {
        // Append the received data to the response string
        response = realloc(response, response_size + n + 1);
        if (response == NULL) {
            error("Error allocating memory");
        }
        memcpy(response + response_size, buffer, n);
        response_size += n;
    }

    if (n < 0) {
        error("Error reading from socket");
        return 0;
    }

    // Null-terminate the response string
    response[response_size] = '\0';

    // Close the socket
    close(sockfd);

    // Now, 'response' contains the entire HTTP response as a string
    remove_pre_tags(response);
    printf("\n%s\n", response);
    

    // Clean up memory
    free(response);

    return 1;
}
