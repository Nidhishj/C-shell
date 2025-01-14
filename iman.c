#include "headers.h"

#define BUFFER_SIZE 8192
#define PORT 80

void fetch_man_page(const char *command) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char request[BUFFER_SIZE], response[BUFFER_SIZE];
    char *host_name = "man.he.net";
    char *page_template = "/?topic=%s&section=all";  // Fetching man pages
    char page[100];

    // Prepare the page URL
    snprintf(page, sizeof(page), page_template, command);

    // Get host by name
    host = gethostbyname(host_name);
    if (host == NULL) {
        fprintf(stderr, "Error: Unknown host\n");
        return;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting");
        close(sockfd);
        exit(1);
    }

    // Create the GET request
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n", 
             page, host_name);

    // Send the GET request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Error sending request");
        close(sockfd);
        exit(1);
    }

    // Read the response and print the body (excluding headers)
    int headers_ended = 0;
    ssize_t received;
    int ang_count=0;
    while ((received = recv(sockfd, response, BUFFER_SIZE - 1, 0)) > 0) {
        response[received] = '\0';  // Null-terminate the response
        char *html_strt;
        if(ang_count==0)
        {
           html_strt=strstr(response,"<");
            ang_count++;
        
        int k=0;
        for(int i=0;i<strlen(html_strt);i++)
        {
            if(k==0&&html_strt[i]=='<')
            {
                k=1;
            }
            else if(k==0)
            {
                printf("%c",html_strt[i]);
            }
            else if(k==1&&html_strt[i]=='>')
            {
                k=0;
            }
        }
        }
        else
        {
            char html_strt[8200];
            strcpy(html_strt,response);
            int k=0;
        for(int i=0;i<strlen(html_strt);i++)
        {
            if(k==0&&html_strt[i]=='<')
            {
                k=1;
            }
            else if(k==0)
            {
                printf("%c",html_strt[i]);
            }
            else if(k==1&&html_strt[i]=='>')
            {
                k=0;
            }
        }
        }
}   
  if (received < 0) {
        perror("Error receiving response");
    }

 // Close the socket
    close(sockfd);
}

void iman(char** instr,int k)
{
    if(k<=2)
    {
        printf("%sINVALID INPUT%s\n",RED,RESET);
    }
    fetch_man_page(instr[2]);
}