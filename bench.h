#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*
 RSA ALGORITHM
 
 1. choose p,q -> 2 prime numbers
 2. compute n = p*q and t=(p-1)*(q-1);
 
 3. choose a number e , such that 1<e<t &&  (e,n) is co-prime && (e,n) is PUBLIC KEY
 4. compute d such that (d*e) % t = 1.  &&  (d,n) is PRIVATE KEY
 
*/

long int flag, e[100], d[100], j, i,temp[8192];

long int p = 337;
long int q = 47;

//long int n = p * q;
//long int t = (p - 1) * (q - 1);

long int n = 15839;
long int t = 15456;

char delimiter1[2]="\%";
char delimiter2[3]="@@";
char delimiter3[3]="$$";

//char buf[1024] ={0};
//char temp_buf[1024] = {0};

int prime(long int);
void public_key();
long int private_key(long int);
char * encrypt();
char * decrypt();


char encrypting[8192];
char decrypting[8192];

int prime(long int pr)
{
    int i;
    j = sqrt(pr);
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

long int private_key(long int x)
{
    long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

char * encrypt(char encrypting[])
{
    long int encrypted[8192];

    char *buf = malloc(8192);
    memset(buf,0,8192);

    char *temp_buf = malloc(8192);
    memset(temp_buf,0,8192);

    int variable = 0;

    for (i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;

        flag = prime(i);


        if (flag == 1 && i != p && i != q)
        {
            e[variable] = i;
            flag = private_key(e[variable]);

            if (flag > 0)
            {
                d[variable] = flag;
                variable++;

            }
            if (variable == 100)
                break;
        }

    }
    long int var, count, key = e[0], x;

//    printf("packet received is : %s\n", encrypting);
    long int len =  strlen(encrypting);

    i = 0;
    while (i != len)
    {
        var = encrypting[i];

        var = var - 96;

        x = 1;
        for (j = 0; j < key; j++)
        {
            x = x * var;
            x = x % n;
        }
        temp[i] = x;
        count = x + 96;
        encrypted[i] = count;

        i++;


    }
     encrypted[i] = -1;

    int y=0;
     for (i = 0; encrypted[i] != -1; i++)
         y += sprintf(temp_buf+y,"%ld%s",temp[i],delimiter3);
        //printf("temp_buf is: %s\n",temp_buf);

    int n =0;

    for (i = 0; encrypted[i] != -1; i++)
        n += sprintf(buf+n, "%ld%s", encrypted[i],delimiter1);
        //printf("temp_buf is: %s\n",temp_buf);

    strcat(buf,delimiter2);
    strcat(buf,temp_buf);
    strcat(buf,delimiter2);

 // printf( "\nTHE ENCRYPTED MESSAGE IS %s\n",buf);  
    return buf;

}

// this function gets encrypted[i] % temp[i]

char* decrypt(char decrypting[])
{

    char *buf_decrypt = malloc(8192);
    memset(buf_decrypt,0,8192);

    char value1[8192], value2[8192];

    bzero(value1,8192);
    bzero(value2,8192);

    //value 1 is encrypted[i]

    strcpy(value1,strtok(decrypting,delimiter2));
//    printf("value1 is : %s\n",value1);

    //value 2 is temp[i]

    strcpy(value2,strtok(NULL,delimiter2));
//    printf("value2 is : %s\n",value2);

    int tokens1[8192]= {0};
    int tokens2[8192]= {0};

    int n1 =0, a1 = 0, n2 =0, a2 =0;

    char *p1 = strtok(value1,delimiter1);
    for (; p1; p1 = strtok(NULL,delimiter1)) {
        a1 = atoi(p1);
        tokens1[n1++] = a1;
    }
//    for (int something1 = 0; something1!= n1; ++something1) {
//        printf("token1 is %d\n",tokens1[something1]);
//    }

    char *p2 = strtok(value2,delimiter3);
    for (; p2; p2 = strtok(NULL,delimiter3)) {
        a2 = atoi(p2);
        tokens2[n2++] = a2;
    }
//    for (int something2 = 0; something2!= n2; ++something2) {
//        printf("token2 is %d\n",tokens2[something2]);
//    }

    long int temp_variable, count, key = d[0], local_temp;
    char packetbuf1[8192];
    i = 0;
 while (tokens1[i] != 0)
    {
        count = tokens2[i];
        local_temp = 1;
        for (j = 0; j < key; j++)
        {
            local_temp = local_temp * count;
            local_temp = local_temp % n;
        }
        temp_variable = local_temp + 96;
        packetbuf1[i] = temp_variable;
        i++;
    }
    packetbuf1[i] = -1;

    int k =0;

    for (i = 0; packetbuf1[i] != -1; i++)
        k += sprintf(buf_decrypt+k, "%c", packetbuf1[i]);

 //printf( "\nTHE DECRYPTED MESSAGE IS: %s\n",buf_decrypt);   
    return buf_decrypt;
}


