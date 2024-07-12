#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples

void  Handler(int signo)
{
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();
    exit(0);
}

int main(int argc, char *argv[])
{
    char *message_text = NULL;
    int c;
    int delay=0;
    int auto_erase=0;
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
    while ((c = getopt (argc, argv, "m:ed:h")) != -1)
	switch (c)
	{
    case 'm':
        message_text = optarg;
        break;
    case 'e':
        auto_erase=1;
        break;
    case 'd':
        delay = atoi(optarg);
        break;
    case 'h':
		fprintf(stderr,"edgemap-ui \n");
        fprintf(stderr,"Usage: -m [message] -d [delay in ms] -e [auto erase edgemap info]\n");
		return 1;
	break;
		default:
		break;
	}
    
    if ( message_text !=  NULL )
    {
        edgemap_message(message_text,delay);
        return 0;
    }
    
    // Hard codec for 2in13 
    //EPD_2in13_V3_test();
    edgemap_bootmsg(auto_erase);
    
    return 0;
}
