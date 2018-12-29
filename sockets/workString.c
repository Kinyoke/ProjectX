#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <unistd.h>

#include <stdarg.h>
#include <string.h>

#include <inttypes.h>

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

// #define rotate_left(n,s) uint32_t t4 = ( n<<s ) | (n>>>(32-s)); return t4;



#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66



char payload[1024] = 
   "GET / HTTP/1.1\r\n"
   "Host: localhost:5001\r\n"
   "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0\r\n"
   "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
   "Accept-Language: en-US,en;q=0.5\r\n"
   "Accept-Encoding: gzip, deflate\r\n"
   "Sec-WebSocket-Version: 13\r\n"
   "Origin: http://localhost\r\n"
   "Sec-WebSocket-Extensions: permessage-deflate\r\n"
   "Sec-WebSocket-Key: HytHQp+zzrG93zG/ZQzkPA==\r\n"
   "Connection: keep-alive, Upgrade\r\n"
   "Pragma: no-cache\r\n"
   "Cache-Control: no-cache\r\n"
   "Upgrade: websocket\r\n";

char *concatenate_string(char *original, char *add);

char *concatenate(char p[], char q[]);

int match(char text[], char pattern[]);

char *return_match(char pattern[], char text[]);

int return_match_lindex(char pattern[], char text[]);

char *get_string(char pattern[], char input[]);

int replace(const char *message, const char *delimiter, char output_msg[]);


int textVariadic(int count, ...);

int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize);

int base64decode (char *in, size_t inLen, unsigned char *out, size_t outLen);

int encrypt_sha1(const char *message, char *message_digest);

// char *rotate_left(uint32_t *n, uint32_t s) {

//         char *t4 = ( n<<s ) | (n>>>(32-s));


//         return t4;


//     };


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


char *c = "\n";
char *d = "\r\n";

const char SALT[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

int main( int argc, char *argv[] ) {
   

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



      char a[] = "Sec-WebSocket-Key: HytHQp+zzrG93zG/ZQzkPA==";
      char b[] = "Sec-WebSocket-Key: ";


    for (int i = 0; i < counter; ++i)
    {
       if (return_match(b, line_val[i]) != NULL)
      {
      	char myOutput[] = "";

      	char *data_buf = get_string(b, line_val[i]);

        char output_msg[] = "";

        replace(data_buf, "==", output_msg);

        // printf("%s\n", output_msg);

        strcpy(data_buf, output_msg);

        strcat(data_buf, SALT);

      	size_t d_length = strlen(data_buf);

      	size_t r_length = (size_t)128;
        
        base64encode(data_buf, d_length, myOutput, r_length);

        printf("%s\n", "encoding...");
   		
   		  printf("input_text | %s >> input_length | %d >> output_text | %s \n", data_buf, d_length, myOutput);


        encrypt_sha1(myOutput, data_buf);

        char *in = myOutput;

        char dec_out[] = "";

        size_t outLen = 128;

        size_t inLen = strlen(in);

        base64decode (in, inLen, dec_out, outLen);
        
    	  printf("%s\n", "decoding...");
    	
        printf("input_length | %d >> output_text | %s\n", inLen,dec_out);

      }
    
    }


   return 0;
   
}



int replace(const char *message, const char *delimiter, char *output_msg){
   // char *val = strstr(replace, input);
  char dlmt[strlen(delimiter)];

  strcpy(dlmt, delimiter);

  int index = 0;

  int flag = 0;

  while(*message != '\0'){

    if (*message == dlmt[index])
    {

       index = (index <= strlen(dlmt))? index++ : 0;

    }else{
      *(output_msg++) = *message;
    }

    message++;
  }

    *(output_msg++) = '\0';



   return 0;

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


// Note 1: All variables are unsigned 32-bit quantities and wrap modulo 2 32 when calculating, except for
// ml, the message length, which is a 64-bit quantity, and
// hh, the message digest, which is a 160-bit quantity.

// Note 2: All constants in this pseudo code are in big endian.
// Within each word, the most significant byte is stored in the leftmost byte position

int encrypt_sha1(const char *message, char *message_digest){

  // Initialize variables:

  uint32_t h0 = 0x67452301;
  uint32_t h1 = 0xEFCDAB89;
  uint32_t h2 = 0x98BADCFE;
  uint32_t h3 = 0x10325476;
  uint32_t h4 = 0xC3D2E1F0;

  char *myMsg;

  strcpy(myMsg , message);

  size_t ml = strlen(myMsg); //message length in bits (always a multiple of the number of bits in a character).

  char hh[160];

  printf("size of %d >> is multiple of 8 >> %d\n",  sizeof(hh), ml%8);

  //Pre-processing:

  // append the bit '1' to the message e.g. by adding 0x80 if message length is a multiple of 8 bits.

  myMsg[ml] = (ml%8 == 0)? 0x80 : '\0';

  printf("%s >> message length >> %d\n", myMsg, strlen(myMsg));

  // append 0 ≤ k < 512 bits '0', such that the resulting message length in bits

  // is congruent to −64 ≡ 448 (mod 512)

  // append ml, the original message length, as a 64-bit big-endian integer.

  // Thus, the total length is a multiple of 512 bits.

  // Process the message in successive 512-bit chunks:

  // break message into 512-bit chunks

  // for each chunk break chunk into sixteen 32-bit big-endian words w[i], 0 ≤ i ≤ 15

  // Extend the sixteen 32-bit words into eighty 32-bit words:

  for(int i  = 16; i  <= 79; i++) w[i] = (w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]) leftrotate 1;

  // Initialize hash value for this chunk:

  uint32_t a = h0;

  uint32_t b = h1;

  uint32_t c = h2;

  uint32_t d = h3;

  uint32_t e = h4;

  // Main loop

  for (int i = 0; i <= 79; i++){

    if (0 <= i <= 19){

      f = (b and c) or ((not b) and d)

      k = 0x5A827999;

    }else if(20 <= i <= 39){

      f = b ^ c ^ d;

      k = 0x6ED9EBA1;
    
    }else if(40 <= i <= 59){

      f = (b and c) or (b and d) or (c and d);

      k = 0x8F1BBCDC;

    }else if(60 <= i <= 79){

      f = b ^ c ^ d;

      k = 0xCA62C1D6;

    }

  }

  
  temp = (a leftrotate 5) + f + e + k + w[i];

  e = d;
  d = c;
  c = b leftrotate 30;
  b = a;
  a = temp;

  //Add this chunk's hash to result so far:

  h0 = h0 + a;
  h1 = h1 + b;
  h2 = h2 + c;
  h3 = h3 + d;
  h4 = h4 + e;

  //Produce the final hash value (big-endian) as a 160-bit number:

  message_digest = (h0 << 128) | (h1 << 96) | (h2 << 64) | (h3 << 32) | h4;

  return 0;
}

