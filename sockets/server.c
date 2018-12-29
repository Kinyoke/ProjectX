#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const unsigned char d_o[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};



char *split(const char *delimiter, char *input);

char *replace(const char *replace, const char *delimiter, char *input);

char *concatenate_string(char *original, char *add);

char *concatenate(char p[], char q[]);

int match(char text[], char pattern[]);

char *return_match(char pattern[], char text[]);

int return_match_lindex(char pattern[], char text[]);

char *get_string(char pattern[], char input[]);

int textVariadic(int count, ...);

int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize);

int base64decode (char *in, size_t inLen, unsigned char *out, size_t outLen);

char *c = "\n";

char *d = "\r\n";

const char SALT[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

int main( int argc, char *argv[] ) {
 
   int sockfd, newsockfd, portno, clilen;
 
   char buffer[1023];
 
   struct sockaddr_in serv_addr, cli_addr;
 
   int  n;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   portno = 5001;
   
   serv_addr.sin_family = AF_INET;
   
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */

   listen(sockfd,5);
   
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
   
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   bzero(buffer,1023);
   
   n = read( newsockfd,buffer,1023);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   printf("%s\n" , buffer);

   char payload[1023];

   strcpy(payload, buffer);

   char *reply =  "HTTP/1.1 101 Switching Protocols\r\n"
                  "Connection: Upgrade\r\n"
                  "Upgrade: websocket\r\n"
                  "Sec-WebSocket-Accept: \r\n\r\n";


   // extract a key from a the client request header.

   int counter = 0;

   int index = 0;

   while (payload[index] != '\0')
   {
      if (payload[index] == *c)
      {
         counter++;
      }
      index++;
   }

   char line_val[counter][255];   
   
   int j = 0;

   for (int i = 0; i < counter; i++)
   {  
      char myString[233] = "";

      int pointer = 0;

      for (; j < index; j++)
      { 
         if (payload[j] == *d)
         {  
            index = j;

         }else{
            myString[pointer] = payload[j];
       
            pointer++;
         }
      }

      index = strlen(payload);
      
      strcpy(line_val[i], myString);
   }

   char b[] = "Sec-WebSocket-Key: ";

   char out_key[] = "";


   for (int i = 0; i < counter; ++i)
    {
       if (return_match(b, line_val[i]) != NULL)
      {
        char myOutput[] = "";

        char *data_buf = get_string(b, line_val[i]);

        strcpy(data_buf, strstr(data_buf, "=="));

        strcat(data_buf, SALT);

        size_t d_length = strlen(data_buf);

        size_t r_length = 64;
        
        base64encode(data_buf, d_length, myOutput, r_length);

        printf("%s\n", "encoding...");
      
        printf("input_text | %s >> input_length | %d >> output_text | %s >> output_length | %d\n", data_buf, d_length, myOutput, r_length);


        char *in = myOutput;

        char dec_out[] = "";

        size_t outLen = 128;

        size_t inLen = strlen(in);

        base64decode (in, inLen, dec_out, outLen);

        printf("%s\n", "decoding...");

        printf("input_length | %d >> output_text | %s >> output_length | %d\n", inLen,dec_out, outLen);

      }
    
    }

   /* Write a response to the client */
 
   n = write(newsockfd, reply, strlen(reply)); 

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }

      
   return 0;
}


// char *split(const char *delimiter, char *input){
//    // prototype string = my name is faisal >> mynameisfaisal
//    char *tmp = input;
//    char *output = 0;

//    while(*tmp){

//       if (*delimiter != *tmp)
//       {
//          output = tmp;
//          printf("%s\n", "it's a match");  
//       }
//       tmp++;
//    }

//    return output;
// }


char *replace(const char *message, const char *delimiter){
   // char *val = strstr(replace, input);
  char dlmt[strlen(delimiter)];

  // char  

  strcpy(dlmt, delimiter);

  int index = 0;

  while(*message != '\0'){
    if (*message != dlmt[index])
    {
       *(output_msg++) = *message;
       // output_msg++; 
    }
    message++;
  }

   return 

}



char *concatenate(char p[], char q[]) {
   int c, d;
   
   c = 0;
 
   while (p[c] != '\0') {
      c++;      
   }
 
   d = 0;
 
   while (q[d] != '\0') {
      p[c] = q[d];
      d++;
      c++;    
   }
 
   p[c] = '\0';

   return p;
}



int match(char text[], char pattern[]) {

  int c, d, e, text_length, pattern_length, position = -1;
  text_length    = strlen(text);
  pattern_length = strlen(pattern);
 
  if (pattern_length > text_length) { return -1; }
 
  for (c = 0; c <= text_length - pattern_length; c++) {
    position = e = c;

    for (d = 0; d < pattern_length; d++) {
      if (pattern[d] == text[e]) {
        e++;
      }
      else {
        break;
      }
    }
    if (d == pattern_length) {
      return position;
    }
  }
 
  return -1;
}

char val_m[255];


char *return_match(char pattern[], char text[]){
   
   int pointer = 0; int sum = 0; char temp[255];

   for (int i = 0; i < strlen(text); i++)
   {
      if (pointer <= strlen(pattern))
      {
        if(pattern[pointer] == text[i]){

          //add sum, if sum == pattern length, return one
          temp[pointer] = text[i];

          // printf("%c <<>> %c <<>> %d <<>> %d <<>> %d\n", pattern[pointer], text[i], strlen(pattern)-1, pointer, sum);
          if (sum == (strlen(pattern)-1))
          {
            temp[pointer+1] = '\0';
            strcpy(val_m, temp);
            return val_m;//"it's a match";
          }
          sum++; pointer++;

        }else{
          sum =0;
        }

        // printf("%c <<>> %c <<>> %d <<>> %d <<>> %d\n", pattern[pointer], text[i], strlen(pattern)-1, pointer, sum);
      }else{
        pointer = 0;
      }
   }

   return NULL;
}



//return  last index of a matching character in the sequency.

int return_match_lindex(char pattern[], char text[]){
  int pointer = 0; int sum = 0; char temp[255];

   for (int i = 0; i < strlen(text); i++)
   {
      if (pointer <= strlen(pattern))
      {
        if(pattern[pointer] == text[i]){

          //add sum, if sum == pattern length, return one
          temp[pointer] = text[i];

          // printf("%c <<>> %c <<>> %d <<>> %d <<>> %d\n", pattern[pointer], text[i], strlen(pattern)-1, pointer, sum);
          if (sum == (strlen(pattern)-1)){ return i; }
          
          sum++; pointer++;

        }else{
          sum =0;
        }

        // printf("%c <<>> %c <<>> %d <<>> %d <<>> %d\n", pattern[pointer], text[i], strlen(pattern)-1, pointer, sum);
      }else{
        pointer = 0;
      }
   }

   return 0;
}


char my_fnstring[255];

char *get_string(char pattern[], char input[]){

  int pointer = 0; char temp[255];

  int i = return_match_lindex(pattern, input)+1;

   for (; i < strlen(input); i++)
   {
    //add sum, if sum == pattern length, return one

    if (input[i] == '\0')
    {
      break;
    }
    
    temp[pointer] = input[i];
          
    pointer++;    

    // printf("payload size %d\n", strlen(input));

   }

   temp[pointer] = '\0';
   strcpy(my_fnstring, temp);
   

   return my_fnstring;
}


int textVariadic(int count, ...){
  
  char pattern, text;

  int total = 0;
  
  // va_list is a type to hold information about 
  // variable arguments 
  va_list valist; 
  
  // va_start must be called before accessing 
  // variable argument list 
  va_start(valist, count);

  // Now arguments can be accessed one by one 
  // using va_arg macro. Initialize min as first 
  // argument in list 
  // min = va_arg(valist, int);
  // va_arg(valist, int); 
  
  // traverse rest of the arguments to find out minimum 
  /* access all the arguments assigned to valist */
  for (int i = 0; i < count; i++) {
      total += va_arg(valist, int);
      // int string = va_arg(valist, int);

      printf("%d\n", total);
  } 
     
  va_end(valist); 
  
  return total;

}





// 1: SHA1 ENCRYPTION ALGORITHIM IMPLEMENTATION


int encrypt_sha1(){
  return 0;
}


// 2; base64 ENCODING AND DECODING ALGORITHIM IMPLEMENTATION




int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
   
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;

   /* increment over the length of the string, three characters at a time */
   for (x = 0; x < dataLength; x += 3) 
   {
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+2) < dataLength)
         n += data[x+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n0];
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n1];

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((x+1) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n2];
      }

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((x+2) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n3];
      }
   }

   /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
   if (padCount > 0) 
   { 
      for (; padCount < 3; padCount++) 
      { 
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = '=';
      } 
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;

   resultSize = resultIndex;

   return 0;   /* indicate success */
}




int base64decode (char *in, size_t inLen, unsigned char *out, size_t outLen) { 
    char *end = in + inLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;

    
    while (in < end) {
        unsigned char c = d_o[*in++];
        
        switch (c) {
        case WHITESPACE: continue;   /* skip whitespace */
        case INVALID:    return 1;   /* invalid input, return error */
        case EQUALS:                 /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            /* If the buffer is full, split it into bytes */
            if (iter == 4) {
                if ((len += 3) > outLen) return 1; /* buffer overflow */
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;   
                buf = 0; iter = 0;
            }   
        }
    }
   
    if (iter == 3) {
        if ((len += 2) > outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    outLen = len; /* modify to reflect the actual output size */
    return 0;
}