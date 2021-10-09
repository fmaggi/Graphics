#ifndef C_SCRIPT_H
#define C_SCRIPT_H

typedef int EntityID;

typedef void (*ScriptFunc)(EntityID);

typedef struct
{
    ScriptFunc func;
    int run;    
} ScriptComponent;

#endif