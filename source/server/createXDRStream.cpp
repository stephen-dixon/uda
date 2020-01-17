// Create the Server Side XDR File Stream
//
//----------------------------------------------------------------
#include "createXDRStream.h"

#include <server/udaServer.h>

#include "writer.h"

#if !defined(FATCLIENT) && defined(SSLAUTHENTICATION)
#include <authentication/udaSSL.h>
#endif

void CreateXDRStream() {
    serverOutput->x_ops  = nullptr;
    serverInput->x_ops   = nullptr;

#if !defined(FATCLIENT) && defined(SSLAUTHENTICATION)

    if(getUdaServerSSLDisabled()){

#ifdef __APPLE__
       xdrrec_create( serverOutput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (void *, void *, int))Readin,
                      (int (*) (void *, void *, int))Writeout);

       xdrrec_create( serverInput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (void *, void *, int))Readin,
                      (int (*) (void *, void *, int))Writeout);
#else
       xdrrec_create( serverOutput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (char *, char *, int))Readin,
                      (int (*) (char *, char *, int))Writeout);

       xdrrec_create( serverInput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (char *, char *, int))Readin,
                      (int (*) (char *, char *, int))Writeout);
#endif     
    } else { 
#ifdef __APPLE__
       xdrrec_create( serverOutput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (void *, void *, int))readUdaServerSSL,
                      (int (*) (void *, void *, int))writeUdaServerSSL);

       xdrrec_create( serverInput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (void *, void *, int))readUdaServerSSL,
                      (int (*) (void *, void *, int))writeUdaServerSSL);
#else
       xdrrec_create( serverOutput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (char *, char *, int))readUdaServerSSL,
                      (int (*) (char *, char *, int))writeUdaServerSSL);

       xdrrec_create( serverInput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                      (int (*) (char *, char *, int))readUdaServerSSL,
                      (int (*) (char *, char *, int))writeUdaServerSSL);
#endif
    }
    
#else	// SSLAUTHENTICATION

#ifdef __APPLE__
    xdrrec_create( serverOutput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                   (int (*) (void *, void *, int))Readin,
                   (int (*) (void *, void *, int))Writeout);

    xdrrec_create( serverInput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                   (int (*) (void *, void *, int))Readin,
                   (int (*) (void *, void *, int))Writeout);
#else
    xdrrec_create( serverOutput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                   (int (*) (char *, char *, int))Readin,
                   (int (*) (char *, char *, int))Writeout);

    xdrrec_create( serverInput, DB_READ_BLOCK_SIZE, DB_WRITE_BLOCK_SIZE, nullptr,
                   (int (*) (char *, char *, int))Readin,
                   (int (*) (char *, char *, int))Writeout);
#endif

#endif   // SSLAUTHENTICATION

    serverInput->x_op   = XDR_DECODE;
    serverOutput->x_op  = XDR_ENCODE;
}