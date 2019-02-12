/*
 * Created by IntelliJ IDEA.
 * Project: LED-Gpio-Linux-Device-Driver
 * ===========================================
 * User: ByeongGil Jung
 * Date: 2019-02-11
 * Time: 오후 6:28
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char input[10];
    int dev;
    int buff[1024];

    printf("Device driver test.\n");

    dev = open("/dev/my_device", O_RDWR);
    printf("dev = %d\n", dev);

    while(1) {
        printf("0 : exit, 1 : on LED, 2: off LED, 3: read LED\n");
        scanf("%s", input);
        if (*input == '0') {
            break;
        } else if (*input == '1') {
            write(dev, "1", 4);
            printf("LED on\n");
        } else if (*input == '2') {
            write(dev, "0", 4);
            printf("LED off\n");
        } else if (*input == '3') {
            read(dev, buff, 1);
            printf("read from device: %d\n", buff[0]);
        }
    }

    close(dev);

    exit(EXIT_SUCCESS);
}
