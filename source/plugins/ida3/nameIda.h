#ifndef IDAM_PLUGINS_IDA3_NAMEIDA_H
#define IDAM_PLUGINS_IDA3_NAMEIDA_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NOIDAPLUGIN
//-------------------------------------------------------------------
// Form the Filename of the IDA File

void nameIDA(const char* alias, int pulno, char* filename);

#endif

#ifdef __cplusplus
}
#endif

#endif // IDAM_PLUGINS_IDA3_NAMEIDA_H