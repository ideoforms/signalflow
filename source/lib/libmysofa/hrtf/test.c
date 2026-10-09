#include "mysofa.h"

int main()
{
    int filter_length;
    int err;
    struct MYSOFA_EASY *hrtf = NULL;

    hrtf = mysofa_open("file.sofa", 48000, &filter_length, &err);
    if(hrtf==NULL)
        return err;
}
