/*	Consloe Utility for nooLite 
	(c) Alex Nemcov
	для подсказки - режим компиляции
	gcc nooLite.c -o noolite -lusb-1.0
	выставить режим запуска в правами root
	# chown root noolite 
	# chmod a+s noolite
*/ 

#define LINUX

#ifdef WINDOWS
#include "libusb.h"
#endif
#ifdef LINUX
#include <libusb-1.0/libusb.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DEV_VID 0x16c0 //0x5824
#define DEV_PID 0x05df //0x1503
#define DEV_CONFIG 1
#define DEV_INTF 0
#define EP_IN 0x81
#define EP_OUT 0x01

unsigned char COMMAND_ACTION[8] = {0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //{80,0x00,0xaa,0x00,0x0,0x1,0x14,0x05}

int main(int argc, char * argv[])
{
	libusb_device_handle * handle;
	
	int i, ret; 
	unsigned int level;
	unsigned char command[1], buf[8], channel;
	char param;
	
	//Arg Control

	if (argc == 1) {
		printf("Using: %s -help\n", argv[0]);
		return -1;
	}

	if (argc >= 2) {
		if(strcmp (argv[1],"-api")){      //first arg "-api" needed
			if (strcmp (argv[1],"-help")==0)
			{
				printf("Using %s -api -<command> <channel> [<level>]\n", argv[0]);
				printf("	<command> may be:\n");
				printf("		-on_ch - Turn channel ON\n");
				printf("		-off_ch - Turn channel OFF\n");
				printf("		-sw_ch - Switch channel ON/OFF\n");
				printf("		-set_ch - Set level for channel\n");
				printf("		-bind_ch - Bind channel\n");
				printf("		-unbind_ch - Unbind channel\n");
				printf("		-preset - Activate preset\n");
				printf("	<channel> must be [1..8]\n");
				printf("	<level> must be [0..100] - using for -set_ch\n");
				return -1;
			}
			printf("Неверно указан режим\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
			return -1;
		}
	} else {
		printf("Не указан параметр API\r\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
		return -1;
	}
	
	if (argc >= 3) {
		if (strcmp (argv[2],"-on_ch")==0)  //Set cnannel ON
		{
			COMMAND_ACTION[1] = 2; 
		} 
		else if (strcmp(argv[2],"-off_ch")==0) //Set channel OFF
		{
			COMMAND_ACTION[1] = 0;
		} 
		else if (strcmp(argv[2],"-sw_ch")==0) //Switch channel ON/OFF
		{
			COMMAND_ACTION[1] = 4;
		} 
		else if (strcmp(argv[2],"-set_ch")==0) //Set level on channel - needed arg "level"
		{
			COMMAND_ACTION[1] = 6; 
		} 
		else if (strcmp(argv[2],"-bind_ch")==0) //Привязать канал
		{
			COMMAND_ACTION[1] = 15;
		} 
		else if (strcmp(argv[2],"-unbind_ch")==0) //отвязать канал
		{
			COMMAND_ACTION[1] = 9;
		} 
		else if (strcmp(argv[2],"-preset")==0) //Вызвать сценарий
		{
			COMMAND_ACTION[1] = 7;
		} 
		else 
		{
			printf("Command unknown\n");
			return -1;
		}
	} else {
		printf("Не указана команда\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
		return -1;
	}

	if (argc >= 4) {
		channel	= atoi(argv[3]);
		channel--;
		if ((channel>7)||(channel<0)) {
			printf("Неверно указан канал (1-8)\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
			return -1;
		}
		COMMAND_ACTION[4] = channel;
	} else {
		printf("Не указан канал\nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
		return -1;
	}
	
	if (COMMAND_ACTION[1]==6)
	{
		if (argc >= 5) {
			level	= atoi(argv[4]);
			if (level>100)
			{
				level=100;
			}
			if (level<0)
			{
				level=0;
			}
			level=(int)(34+(float)level*1.23);
			COMMAND_ACTION[5]= level;
			COMMAND_ACTION[2]= 1;
		} else {
			printf("Не указан уровень \nИспользование: %s -api -<command> <channel> [<level>]\n", argv[0]);
			return -1;
		}
	}

//Prepare Command string
	libusb_init(NULL);
	libusb_set_debug(NULL, 3);
	handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID);
	if (handle == NULL) {
	    printf("Не удалось найти устройство\n");
	    libusb_exit(NULL);
	    return 0;
	}
	if (libusb_kernel_driver_active(handle,DEV_INTF))
		libusb_detach_kernel_driver(handle, DEV_INTF);
	if ((ret = libusb_set_configuration(handle, DEV_CONFIG)) < 0)
	{
		printf("Ошибка конфигурации\n");
		libusb_close(handle);
		libusb_exit(NULL);
		if (ret == LIBUSB_ERROR_BUSY)
		    printf("B\n");
		printf("ret:%i\n", ret);    
		return 0;
	}
	if (libusb_claim_interface(handle,  DEV_INTF) < 0)
	{
		printf("Ошибка интерфейса\n");
		libusb_close(handle);
		libusb_exit(NULL);
		return 0;
	}
	
	//0x9 - номер запроса
	//0x300 - значение запроса - их надо получить из мониторинга

	ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT, 0x9, 0x300, 0, COMMAND_ACTION, 8, 100);

	libusb_attach_kernel_driver(handle, DEV_INTF);
	libusb_close(handle);
	libusb_exit(NULL);
	
	return 0;
}
