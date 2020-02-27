#ifndef UDA_SERVER_UDASERVER_H
#define UDA_SERVER_UDASERVER_H

#include <plugins/pluginStructs.h>
#include <clientserver/socketStructs.h>
#include <structures/genStructs.h>

int udaServer(CLIENT_BLOCK client_block);

int fatServer(CLIENT_BLOCK client_block, SERVER_BLOCK* server_block, REQUEST_BLOCK* request_block0,
              DATA_BLOCK* data_block0);

//--------------------------------------------------------------
// Static Global variables

extern unsigned int totalDataBlockSize;
extern int serverVersion;
extern int altRank;

#ifndef FATCLIENT
extern XDR* serverInput;
extern XDR* serverOutput;
#endif
extern int server_tot_block_time;
extern int server_timeout;

extern USERDEFINEDTYPELIST parseduserdefinedtypelist;

#endif // UDA_SERVER_UDASERVER_H
