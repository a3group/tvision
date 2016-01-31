
#include "../inc/netprocess.h"

#include <iostream>

CNetProcess::CNetProcess()
{
}

CNetProcess::~CNetProcess()
{
	//data deleted in derived class!
}

CNetProcess *CNetProcess::processRun(sharedData *psharedData)
{
	volatile int i;
	for (i = 0; i < NCYCLE;i++){};

	psharedData->VNetflg = freeState;

	std::cout << "CNetProcess finished" << std::endl;
	return this;
}

#if 0

#ifdef USE_PCAP
#include <pcap.h>
#endif


void msg_disp_main_loop( struct service_globals_s *glob )
{
	int i;
	msg_poll_ctx_t *ctx = glob->disp->poll_ctx;
    msg_disp_create_set( glob->disp );
	switch ( msg_poll(glob->disp->poll_ctx) ) {
		case MSG_POLL_ERROR:
        puts("POLLERROR");
			for ( i = 0; i < ctx->items_number; ++i ) {
				//connection lost
				if ( ctx->items[i].revents == MSG_POLLERR ) {
					msg_disp_item_t *item = glob->disp->disp_items_map[i];
					if ( item->type == MSG_DISP_ITEM_BINDING ) {
						msg_disp_binding_t *binding = item->data.binding;
						fprintf(stderr,"FIX: problem with listen socket\n");
						//We have problem with LISTEN socket
						//TODO: handle it!!!
					} else if (item->type == MSG_DISP_ITEM_CLIENT ){
						io_layer_connection_failed( item->data.client->lower_layer );
						//Remove server connection
						log_error( item->data. client->lower_layer->log,"msg_disp_start: connection lost" );
					} else if (item->type == MSG_DISP_ITEM_SERVER_CONNECTION ){
						//Server lost the connection
					} else if ( item->type == MSG_DISP_ITEM_FILE ) {
						//TODO: check file error
					} else if ( item->type == MSG_DISP_ITEM_RAW ) {
						//TODO: check ethernet error
					}
				}
			}
            puts("POLLERROR HANDLE FINISHED");
			break;
		case MSG_POLL_EVENT:
			for ( i = 0; i < ctx->items_number; ++i ) {
				msg_disp_item_t *item = glob->disp->disp_items_map[i];
				if( item->type == MSG_DISP_ITEM_BINDING ) {
					msg_disp_binding_t *binding = item->data.binding;
					if ( ctx->items[i].revents == MSG_POLLIN ) {
						msg_disp_accept_connection( glob->disp, binding, glob );
					}
				} else if ( item->type == MSG_DISP_ITEM_CLIENT ||
							item->type == MSG_DISP_ITEM_SERVER_CONNECTION)
				{
					//TODO: check it is server connection or simple client!!!!
					msg_device_t *dev = item->data.client;
					if ( ctx->items[i].revents == MSG_POLLIN ) {
						if ( io_layer_data_received( dev->lower_layer, 0 ) != RESULT_OK ) {
							log_error(dev->lower_layer->log, "msg_disp_start: connection closed");
							io_layer_close_request( dev->lower_layer, 0 );
							if ( MSG_DISP_ITEM_SERVER_CONNECTION == item->type ) {
								glob->disp->rem_items[ glob->disp->rem_size ] = item;
								++glob->disp->rem_size;
							}
							continue;
						}
					}
					if ( ctx->items[i].revents == MSG_POLLOUT ) {
						log_debug(dev->lower_layer->log, "msg_disp_start: connection completed");
						io_layer_connection_done( dev->lower_layer );
						continue;
					}
					if ( ctx->items[i].revents == MSG_POLLERR ) {
						log_error(dev->lower_layer->log,"msg_disp_start: communication error");
						io_layer_close_request(dev->lower_layer, 0 );
						if ( MSG_DISP_ITEM_SERVER_CONNECTION == item->type ) {
							glob->disp->rem_items[ glob->disp->rem_size ] = item;
							++glob->disp->rem_size;
						}
						continue;
					}
				} else if ( item->type == MSG_DISP_ITEM_FILE || MSG_DISP_ITEM_RAW ) {
					msg_device_t *dev = item->data.client;
					if ( ctx->items[i].revents == MSG_POLLIN ) {
						if ( io_layer_data_received( dev->lower_layer, 0 ) != RESULT_OK ) {
							log_error(dev->lower_layer->log, "msg_disp_start: file/raw read error");
						}
					}
				}
			}
			break;
		case MSG_POLL_TIMEOUT:
		default:
			break;
			//fprintf(stderr,"!!!!msg_poll() timeout");
	}
    msg_disp_timer_event( glob->disp );//timeout_handler
}

int msg_poll(msg_poll_ctx_t *ctx)
{
    int i, retval;
    //select read set
    fd_set read_set;
    //select write set
    fd_set write_set;
    //select error set
    fd_set error_set;
    //maximum socket number
    int max = 0;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

//    if ( g_need_subscribe ) {
//    	msg_poll_subscribe();
//    }
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&error_set);
    //fprintf(stderr,"Context items number: %d\n",ctx->items_number);
    for ( i = 0; i < ctx->items_number; ++i ) {
        msg_poll_item_t *item = ctx->items + i;
        int fd = -1;
        //if no events are being polled
        if ( item->events == 0 )
            continue;
        switch ( item->type ) {
        case msg_poll_item_socket:
            fd = item->handle.sock;
            break;
        case msg_poll_item_file:
            fd = item->handle.fd;
            break;
        /*case msg_poll_item_raw:
            fd = pcap_get_selectable_fd(item->handle.raw);
            break;*/
        default:
            fd = -1;
        }
        if ( fd < 0 )
            continue;
        if ( item->events & MSG_POLLIN ) {
            FD_SET(fd,&read_set);
        }
        if ( item->events & MSG_POLLOUT ) {
            FD_SET(fd,&write_set);
        }
        if ( item->events & MSG_POLLERR ) {
            FD_SET(fd,&error_set);
        }
        if ( fd > max )
            max = fd;
    }
	g_net_raw = 0;
    //fprintf(stderr,"Before pselect\n");
    retval = select(max+1,&read_set,&write_set,&error_set,&timeout);
    //fprintf(stderr,"After pselect\n");
    if ( retval < 0 ) {
        perror("select");
        return MSG_POLL_ERROR;
    } else if ( retval > 0 ) {
        for ( i = 0; i < ctx->items_number; ++i ) {
            msg_poll_item_t *item = ctx->items + i;
            int fd = -1;
            //if no events are being polled
            if ( item->events == 0 )
                continue;
            switch ( item->type ) {
            case msg_poll_item_socket:
                fd = item->handle.sock;
                break;
            case msg_poll_item_file:
                fd = item->handle.fd;
                break;
            /*case msg_poll_item_raw:
                fd = pcap_get_selectable_fd(item->handle.raw);
                break;*/
            default:
                fd = -1;
            }
            if ( fd < 0 )
                continue;
            if (  FD_ISSET(fd,&read_set) ) {
                item->revents |= MSG_POLLIN;
            }
            if ( FD_ISSET(fd,&write_set) ) {
                item->revents |= MSG_POLLOUT;
            }
            if ( FD_ISSET(fd,&error_set) ) {
                item->revents |= MSG_POLLERR;
            }
        }
		return MSG_POLL_EVENT;
    }
/*	
	else {
		if ( g_net_raw ) {
			fprintf(stderr,"Net raw packet received\n");
			for ( i = 0; i < ctx->items_number; ++i ) {
				msg_poll_item_t *item = ctx->items + i;
				//if no events are being polled
				if ( item->type == msg_poll_item_raw && item->events & MSG_POLLIN ) {
					fprintf(stderr,"Raw layer found\n");
					item->revents = MSG_POLLIN;
				}
			}
			return MSG_POLL_EVENT;
		}
        return MSG_POLL_TIMEOUT;
    }
*/
}
#endif

#ifdef _WIN32
//#include <windows.h>
#endif /*WIN32*///int msg_poll(msg_poll_ctx_t *ctx)

#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib") //Winsock Library

#include <stdio.h>
#include <stdlib.h>

#define MSG_POLL_MAX 64

#define MSG_POLLIN  1
#define MSG_POLLOUT 2
#define MSG_POLLERR 4

typedef enum msg_poll_item_type {
    msg_poll_item_socket = 1,
    msg_poll_item_raw = 2,
    msg_poll_item_file = 3
} msg_poll_item_type_t;



int msg_poll(void)
{
    // Initialize Winsock
	WSADATA wsaData = {0};
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }

	
	int i, retval;
    //select read set
    fd_set read_set;
    //select write set
    fd_set write_set;
    //select error set
    fd_set error_set;
    //maximum socket number
    int max = 0;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

while(1){
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&error_set);

//debug
FILE * ptrFile = fopen("file.txt", "r");
int fd = _fileno( stdin  ) ;	//stdin
FD_SET(fd,&read_set);
FD_SET(fd,&write_set);
FD_SET(fd,&error_set);
max = fd;
int rd_count = 0;
int wr_count = 0;
int er_count = 0;

    //fprintf(stderr,"Context items number: %d\n",ctx->items_number);
//    for ( i = 0; i < ctx->items_number; ++i ) {
    for ( i = 0; i < 1; ++i ) {
//        msg_poll_item_t *item = ctx->items + i;
//        int fd = -1;
        //if no events are being polled
//        if ( item->events == 0 )
//            continue;
//        switch ( item->type ) {
//        case msg_poll_item_socket:
//            fd = item->handle.sock;
//            break;
//        case msg_poll_item_file:
//            fd = item->handle.fd;
//            break;
//        /*case msg_poll_item_raw:
//            fd = pcap_get_selectable_fd(item->handle.raw);
//            break;*/
//        default:
//            fd = -1;
//        }
//        if ( fd < 0 )
//            continue;
//        if ( item->events & MSG_POLLIN ) {
//            FD_SET(fd,&read_set);
//        }
//        if ( item->events & MSG_POLLOUT ) {
//            FD_SET(fd,&write_set);
//        }
//        if ( item->events & MSG_POLLERR ) {
//            FD_SET(fd,&error_set);
//        }
//        if ( fd > max )
//            max = fd;
//    }
//	g_net_raw = 0;
    //fprintf(stderr,"Before pselect\n");
//    retval = select(max+1,&read_set,&write_set,&error_set,&timeout);
    retval = select(max+1,&read_set,NULL,NULL,&timeout);
  if ( retval < 0 ) {
      return WSAGetLastError();//MSG_POLL_ERROR;
  } 
  else if ( retval == 0 ) {
	  //no data
  }
  else if ( retval > 0 ) {
          if (  FD_ISSET(fd,&read_set) ) {
			  std::cout<<"read_set"<<std::endl;
			  rd_count++;
          }
          if ( FD_ISSET(fd,&write_set) ) {
			  std::cout<<"write_set"<<std::endl;
			  wr_count++;
          }
          if ( FD_ISSET(fd,&error_set) ) {
			  std::cout<<"error_set"<<std::endl;
			  er_count++;
          }
  }

	}
}

  //  //fprintf(stderr,"After pselect\n");
  //  if ( retval < 0 ) {
  //      perror("select");
  //      return MSG_POLL_ERROR;
  //  } else if ( retval > 0 ) {
  //      for ( i = 0; i < ctx->items_number; ++i ) {
  //          msg_poll_item_t *item = ctx->items + i;
  //          int fd = -1;
  //          //if no events are being polled
  //          if ( item->events == 0 )
  //              continue;
  //          switch ( item->type ) {
  //          case msg_poll_item_socket:
  //              fd = item->handle.sock;
  //              break;
  //          case msg_poll_item_file:
  //              fd = item->handle.fd;
  //              break;
  //          /*case msg_poll_item_raw:
  //              fd = pcap_get_selectable_fd(item->handle.raw);
  //              break;*/
  //          default:
  //              fd = -1;
  //          }
  //          if ( fd < 0 )
  //              continue;
  //          if (  FD_ISSET(fd,&read_set) ) {
  //              item->revents |= MSG_POLLIN;
  //          }
  //          if ( FD_ISSET(fd,&write_set) ) {
  //              item->revents |= MSG_POLLOUT;
  //          }
  //          if ( FD_ISSET(fd,&error_set) ) {
  //              item->revents |= MSG_POLLERR;
  //          }
  //      }
  //		//return MSG_POLL_EVENT;
  //}

	return 0;
}
