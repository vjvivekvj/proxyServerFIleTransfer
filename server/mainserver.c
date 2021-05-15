#include <sys/socket.h>  
 #include <sys/types.h>  
 #include <resolv.h>  
 #include <string.h>  
 #include <pthread.h>  
 #include <unistd.h>  
#include <ctype.h>
#include <arpa/inet.h>
 // A thread function  
 // A thread is created for each accepted client connection  
 void *runSocket(void *vargp)  
 {  
   int c_fd =(int)vargp; // get client fd from arguments passed to the thread  
   char buffer[65535];  
   char file[20] ;
   FILE *f ;
   int words = 0 ;
   char c;
   //receive filename from proxy server
   recv(c_fd, file, sizeof(file), 0);
   bzero(buffer, 512);
   printf("Searching for %s in main server directory.\n", file);
   f = fopen(file, "r") ;
   if(f==NULL){
        printf("File not found in server\n") ;
      }
      else{
        printf("File found. Transmitting to proxy server.\n") ;
        while((c=getc(f))!=EOF){
                fscanf(f, "%s", buffer);
                if(isspace(c)||c == '\t' || c == '\n') words++ ;
        }
        write(c_fd, &words, sizeof(int));
        rewind(f);
        char ch ;
        while(ch!=EOF){
                fscanf(f, "%s", buffer);
                send(c_fd, buffer, 512, 0);
                ch = fgetc(f);
        }
       }

   return NULL;  
 }  
 // main entry point  
 int main()  
 {  
      int client_fd;  
      char buffer[100];  
      int fd = 0 ;  
      struct sockaddr_in server_sd;  
 // add this line only if server exits when client exits  
 signal(SIGPIPE,SIG_IGN);  
      // create a socket  
      fd = socket(AF_INET, SOCK_STREAM, 0);  
      printf("Server started\n");  
      memset(&server_sd, 0, sizeof(server_sd));  
      // set socket variables  
      server_sd.sin_family = AF_INET;  
      server_sd.sin_port = htons(5010);  
      server_sd.sin_addr.s_addr = INADDR_ANY;  
      // bind socket to the port  
      bind(fd, (struct sockaddr*)&server_sd,sizeof(server_sd));  
      // start listening at the given port for new connection requests  
      listen(fd, SOMAXCONN);  
      // continuously accept connections in while(1) loop  
      while(1)  
      {  
           // accept any incoming connection  
           client_fd = accept(fd, (struct sockaddr*)NULL ,NULL);  
           //printf("accepted client with id: %d",client_fd);  
           // if true then client request is accpted  
           if(client_fd > 0)  
           {  
                 //multithreading variables    
                printf("proxy connected\n");     
                pthread_t tid;  
                // pass client fd as a thread parameter  
                 pthread_create(&tid, NULL, runSocket, (void *)client_fd);   
           }  
      }  
      close(client_fd);   
      return 0;  
 }  
