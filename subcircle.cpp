#include "subcircle.h"

subCircle::subCircle(QString iInd, QString iType, direction* idir1, direction* idir2)
{
    index=iInd;
    type=iType;
    dir1=idir1;
    dir2=idir2;
}

subCircle::subCircle(QString iInd,QString iType,direction* idir1)
{
    index=iInd;
    type=iType;
    dir1=idir1;
}
