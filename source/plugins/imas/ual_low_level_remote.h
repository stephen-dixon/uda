
char **getMdsCpoFields(int expIdx, char *path, int *numFields, int checkSegments);
void refreshExpCtx(char *name, int shot, int run);
struct descriptor_xd* getCpoDataServer(int *expIdx, char *path);
int getDataRemote(int connId, int expIdx, char *cpoPath, char *path, struct descriptor_xd *retXd, int evaluate);
int getObjectRemote(int connId, int expIdx, char *cpoPath, char *path, void **obj, int isTimed);
int mdsbeginIdsGetRemote(int connId, int expIdx, char *path, int isTimed, int *retSamples);
int mdsendIdsGetRemote(int connId, int expIdx, char *path);
struct descriptor_xd* getCpoSlicedDataServer(int *expIdx, char *path, double *time);
int getSlicedDataRemote(int connId, int expIdx, char *cpoPath, char *path, double time, struct descriptor_xd *retDataXd, 
    struct descriptor_xd *retTimesXd);
int getObjectSliceRemote(int connId, int expIdx, char *cpoPath, char *path, double time, void **obj);
int mdsbeginIdsGetSliceRemote(int connId, int expIdx, char *path, double time);
int mdsendIdsGetSliceRemote(int connId, int expIdx, char *path);
int mdsbeginIdsPutRemote(int connId, int expIdx, char *path);
int mdsendIdsPutRemote(int connId, int expIdx, char *path);
int mdsbeginIdsPutTimedRemote(int connId, int expIdx, char *path, int samples, double *inTimes);
int mdsendIdsPutTimedRemote(int connId, int expIdx, char *path);
int mdsbeginIdsPutNonTimedRemote(int connId, int expIdx, char *path);
int mdsendIdsPutNonTimedRemote(int connId, int expIdx, char *path);
int putDataRemote(int connId, int expIdx, char *cpoPath, char *path, struct descriptor *dataD);
int putSegmentRemote(int connId, int expIdx, char *cpoPath, char *path, char *timeBasePath, struct descriptor_a *dataD, double *times, int nTimes);
int putObjectSegmentRemote(int connId, int expIdx, char *cpoPath, char *path, void *objSegment, int segIdx);
int mdsbeginIdsPutSliceRemote(int connId, int expIdx, char *path);
int mdsendIdsPutSliceRemote(int connId, int expIdx, char *path);
int putSliceRemote(int connId, int expIdx, char *cpoPath, char *path, char *timeBasePath, struct descriptor *dataDsc, double time);
int mdsbeginIdsReplaceLastSliceRemote(int connId, int expIdx, char *path);
int mdsendIdsReplaceLastSliceRemote(int connId, int expIdx, char *path);
int mdsimasOpenEnvServer(char *name, int *shot, int *run, char *user, char *tokamak, char *version);
int mdsimasOpenServer(char *name, int *shot, int *run);
int mdsimasCreateEnvServer(char *name, int *shot, int *run, char *user, char *tokamak, char *version);
int mdsimasCreateServer(char *name, int *shot, int *run);
int mdsimasOpenEnvRemote(int connId, char *name, int shot, int run, int *retIdx, char *user, char *tokamak, char *version);
int mdsimasCreateEnvRemote(int connId, char *name, int shot, int run, int *retIdx, char *user, char *tokamak, char *version);
int mdsimasCreateRemote(int connId, char *name, int shot, int run, int *retIdx);
int mdsimasCloseRemote(int connId, int idx, char *name, int shot, int run);

int replaceLastSliceRemote(int connId, int expIdx, char *cpoPath, char *path, struct descriptor *dataDsc);
int mdsimasOpenRemote(int connId, char *name, int shot, int run, int *retIdx);
