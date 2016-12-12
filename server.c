#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define FALSE (1==0)
#define TRUE  (1==1)
#define BUF_LEN 256

void error(char *msg) {
  perror(msg);
  exit(1);
}

// isPalindrome returns whether or not the provided string is a palindrome
// assumes the the string provided is the same size as the buffer
int isPalindrome(char* str) {
  char temp[BUF_LEN];
  int str_count = 0;
  for (int i = 0; str[i]; i++){
    if (str[i] != '\0' && str[i] != '\n') {
      temp[i] = tolower(str[i]);
      str_count++;
    }
    else {
      break;
    }
  }
  int max_index = str_count / 2;
  int index = 0;
  while (index <= max_index) {
    if (temp[index] != temp[str_count - index - 1]) {
      return FALSE;
    }
    index++;
  }
  return TRUE;
}

void *initSocket(int new_port) {
  int sockfd, newsockfd, clilen;
  char buffer[BUF_LEN];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("ERROR opening socket");
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(new_port);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *) &serv_addr,
           sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
  }

  while(1) {
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
      error("ERROR on accept");
    }

    bzero(buffer, BUF_LEN);
    n = read(newsockfd, buffer, BUF_LEN-1);
    if (n < 0) {
      error("ERROR reading from socket");
    } 
    printf("Message Received: %s\n", buffer);

    if ( isPalindrome(buffer) ) {
      n = write(newsockfd, "You have entered a palindrome!", 30);      
    }
    else {
      n = write(newsockfd, "You have not entered a palindrome!", 35);
    }

    if (n < 0) {
      error("ERROR writing to socket");
    }
  }
}

int main (int argc, char *argv[]) {
  pthread_t pth0, pth1, pth2, pth3, pth4;

  pthread_create(&pth0,NULL,initSocket, 11111);
  pthread_create(&pth1,NULL,initSocket, 22222);
  pthread_create(&pth2,NULL,initSocket, 33333);
  pthread_create(&pth3,NULL,initSocket, 44444);
  pthread_create(&pth4,NULL,initSocket, 55555);

  pthread_join(pth0, NULL);
  pthread_join(pth1, NULL);
  pthread_join(pth2, NULL);
  pthread_join(pth3, NULL);
  pthread_join(pth4, NULL);

  return 0;
}