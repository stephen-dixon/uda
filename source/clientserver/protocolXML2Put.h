#ifndef UDA_CLIENTSERVER_PROTOCOLXML2PUT_H
#define UDA_CLIENTSERVER_PROTOCOLXML2PUT_H

#include <stdio.h>
#include <rpc/rpc.h>

#include <structures/genStructs.h>

int xdrUserDefinedDataPut(XDR* xdrs, LOGMALLOCLIST* logmalloclist, USERDEFINEDTYPELIST* userdefinedtypelist,
                          USERDEFINEDTYPE* userdefinedtype, void** data, int datacount, int structRank,
                          int* structShape, int index, NTREE** NTree);

// Send/Receive Array of Structures

int xdrUserDefinedTypeDataPut(XDR* xdrs, LOGMALLOCLIST* logmalloclist, USERDEFINEDTYPELIST* userdefinedtypelist,
                              USERDEFINEDTYPE* userdefinedtype, void** data);

bool_t xdr_userdefinedtypelistPut(XDR* xdrs, USERDEFINEDTYPELIST* str);

int protocolXML2Put(XDR* xdrs, int protocol_id, int direction, int* token, LOGMALLOCLIST* logmalloclist,
                    USERDEFINEDTYPELIST* userdefinedtypelist, void* str);

#endif // UDA_CLIENTSERVER_PROTOCOLXML2PUT_H
