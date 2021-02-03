#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

char buffer[64];

int main()
{
    int ret,fd;

    fd = open("/dev/usbd2",O_RDWR);

    if(fd < 0)
    {
        printf("Failed to open device!\n");
        return -1;
    }
    
    
    printf("enter data to write to the device: ");
    scanf("%s",buffer);
    printf("You entered %s, sending to device..\n",buffer);

    ret = write(fd,buffer,strlen(buffer));
    if(ret < 0)
    {
        printf("Failed to write to device! error: %d\n",ret);
        return -1;
    }
    

    printf("reading from device...\n");
    memset(buffer,0,2);
    ret = read(fd,buffer,64);
    if(ret < 0)
    {
        printf("Failed to read from device! error: %d\n",ret);
        return -1;
    }

    printf("data read: %s\n",buffer);
    return 0;
}

