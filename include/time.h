#ifndef TIME_H
#define TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

PyObject* Time_strftime(PyObject* self, PyObject* args, PyObject* kwds);

static PyMethodDef time_methods[] = {
    {"strftime", Time_strftime, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("strftime\n-\n\n Format the given time.")},
    {NULL, NULL}
};

PyDoc_STRVAR(doc_time, 
"time\n-\n\n\
The time implementation for datetimecpy");

static struct PyModuleDef time_def = {
    PyModuleDef_HEAD_INIT,
    .m_name = "time",
    .m_doc = doc_time,
    .m_size = 0,
    .m_methods = time_methods
};

#ifdef __cplusplus
}
#endif

#endif