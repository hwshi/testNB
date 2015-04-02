#include <stdio.h>
#include <stdlib.h>
#include <Python.h>

#include "pkt_wrap.c"
#include "main.h"
//test version for git in netbeans
// typedef struct _gpacket_t 
// {
//     int a;
//     char b[10];
// } gpacket_t;
int PythonError(PyObject *pObj)
{
    char *Str = PyString_AsString(pObj);
    return printf("%s", Str);
}

void CheckPythonError(void)
{
    if (PyErr_Occurred() != NULL)
    {
        PyErr_Print();
        PyErr_Clear();
    }
}

int MyPrint(char *Str)
{
    return printf(Str);
}

int main()
{
    Py_Initialize();
    SWIG_init();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    PyObject *pProtMod, *pProtGlobalDict, *pFunc, *pArg, *pResult;
    pProtMod = PyImport_ImportModule("testpkt");//load protocol.py
    if (!pProtMod)
    {
        printf("[main]loading protocol module failed!\n");
        return EXIT_FAILURE;
    }

    gpacket_t pkt;// = (gpacket_t *)malloc(sizeof(gpacket_t));
    printf("[main]New protocol Module loaded\n");
    if (pProtMod != NULL)
    {
        printf("[main]module imported\n");
        pProtGlobalDict = PyModule_GetDict(pProtMod);   // Get main dictionary
        if (pProtGlobalDict != NULL)
        {
            printf("[main]main dictionary got\n");
            pFunc = PyDict_GetItemString(pProtGlobalDict, "processor");//TODO: find function of getEntry
            if (pFunc) {
                printf("[main]pFunc got\n");

                pkt.a = 100;
                printf("pkt a in C: %d\n", pkt.a);
                pArg = SWIG_NewPointerObj(SWIG_as_voidptr(&pkt), SWIGTYPE_p__gpacket_t, 1);//TODO:
                //pArg = SWIG_NewPointerObj((void *)&ob, SWIG_TypeQuery("_p_SPythoned"), 1);
                if(pArg){
                    printf("[main]pArg got\n");
                    pResult = PyObject_CallFunction(pFunc, "O", pArg);
                    CheckPythonError();
                    printf("[main]after  pkt->a = %d\n", pkt.a);
                    return EXIT_SUCCESS;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
    }
    Py_Finalize();
    return EXIT_FAILURE;
}