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

#define DEV_VID 0x5824
#define DEV_PID 0x1503
#define DEV_CONFIG 1
#define DEV_INTF 0
#define EP_IN 0x81
#define EP_OUT 0x01

unsigned char COMMAND_START[8] = {0x1F,0x1E,0x92,0x7C,0xB8,0x1,0x14,0x03};
unsigned char COMMAND_FINISH[8] = {0x00,0x1E,0x92,0x7C,0xB8,0x1,0x14,0x04};
unsigned char COMMAND_ACTION[8] = {0x00,0x00,0xaa,0x00,0x0,0x1,0x14,0x05};

int main(int argc, char * argv[])
{
	libusb_device_handle * handle;
	//int ret; 
	unsigned int level;
	unsigned char command[1], buf[8];
	char channel, param;
	
	//Arg Control
	if (argc >= 2) {
		if(strcmp (argv[1],"-api")){
			printf("Неверно указан режим\nИспользование: %s -api [command channel [params]]\n", argv[0]);
			return -1;
		}
	} else {
		printf("Не указан параметр API\r\nИспользование: %s -api [command channel [params]]\n", argv[0]);
		return -1;
	}
	
	if (argc >= 3) {
		if (strcmp (argv[2],"-on_ch")==0)  //Включить канал
		{
			command[0]=2; 
		} 
		else if (strcmp(argv[2],"-off_ch")==0) //Выключить канал
		{
			command[0]=0;
		} 
		else if (strcmp(argv[2],"-sw_ch")==0) //Переключить канал
		{
			command[0]=4;
		} 
		else if (strcmp(argv[2],"-set_ch")==0) //Установить уровень
		{
			command[0]=6; 
		} 
		else if (strcmp(argv[2],"-bind_ch")==0) //Привязать канал
		{
			command[0]=15;
		} 
		else if (strcmp(argv[2],"-unbind_ch")==0) //отвязать канал
		{
			command[0]=9;
		} 
		else if (strcmp(argv[2],"-preset")==0) //Вызвать сценарий
		{
			command[0]=7;
		} 
		else 
		{
			printf("Command unknown\n");
			return -1;
		}
	} else {
		printf("Не указана команда\nИспользование: %s -api [command channel [params]]\n", argv[0]);
		return -1;
	}

	if (argc >= 4) {
		param	= atoi(argv[3]);
	} else {
		printf("Не указан канал\nИспользование: %s -api [command channel [params]]\n", argv[0]);
		return -1;
	}
	
	if (command[0]==6)
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
			printf("LEVEL %i\n", level);
		} else {
			printf("Не указан уровень \nИспользование: %s -api [command [params]]\n", argv[0]);
			return -1;
		}
	}
	
/* Пока закомментируем работу с устройством

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
	

	ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT, 0x9, 0x200, 0, COMMAND_1, 8, 100);
	libusb_interrupt_transfer(handle, EP_IN, buf, 8, &ret, 100);
	ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT, 0x9, 0x200, 0, COMMAND_2, 8, 100);
	libusb_interrupt_transfer(handle, EP_IN, buf, 8, &ret, 100);
	COMMAND_ON[0] = r;
	COMMAND_ON[1] = g;
	COMMAND_ON[2] = b;
	ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT, 0x9, 0x200, 0, COMMAND_ON, 8, 100);
	buf[7] = 0;
	libusb_interrupt_transfer(handle, EP_IN, buf, 8, &ret, 100);
	if (buf[7] != 1) {
		printf("Сбой в управлении устройством\n");
		libusb_close(handle);
		libusb_exit(NULL);
		return 0;
	}
	libusb_attach_kernel_driver(handle, DEV_INTF);
	libusb_close(handle);
	libusb_exit(NULL);
	*/
	return 0;
}
