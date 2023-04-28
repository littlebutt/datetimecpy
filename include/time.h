#ifndef TIME_H
#define TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

static PyTypeObject Time_type;

typedef struct {
    PyObject_HEAD

    PyObject* hour;

    PyObject* minute;

    PyObject* second;

    PyObject* microsecond;

    PyObject* tzinfo;

    PyObject* fold;
} Time;

void Time_dealloc(PyObject* self);                                      // destruction method

PyObject* Time_new(PyTypeObject* type, PyObject* args, PyObject* kwds);  // __new__ method

int Time_init(PyObject* self, PyObject* args, PyObject* kwds);           // __init__ method

PyObject* Time_repr(PyObject* self);                                     // __repr__ method

PyObject* Time_richcompare(PyObject* self, PyObject* other, int op);     // __lt__, __le__, __eq__, __ne__, __gt__, __ge__ methods

PyObject* Time_strftime(PyObject* self, PyObject* args, PyObject* kwds);

static PyMemberDef time_members[] = {
    {"hour", T_OBJECT_EX, offsetof(Time, hour), 0, PyDoc_STR("hour")},
    {"minute", T_OBJECT_EX, offsetof(Time, minute), 0, PyDoc_STR("minute")},
    {"second", T_OBJECT_EX, offsetof(Time, second), 0, PyDoc_STR("second")},
    {"microsecond", T_OBJECT_EX, offsetof(Time, microsecond), 0, PyDoc_STR("microsecond")},
    {"tzinfo", T_OBJECT_EX, offsetof(Time, tzinfo), 0, PyDoc_STR("tzinfo")},
    {"fold", T_OBJECT_EX, offsetof(Time, fold), 0, PyDoc_STR("fold")},
    {NULL}
};

static PyMethodDef time_methods[] = {
    {"strftime", Time_strftime, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("strftime\n-\n\n Format the given time.")},
    {NULL, NULL}
};

PyDoc_STRVAR(doc_time, 
"time\n-\n\n\
The time implementation for datetimecpy");

static PyTypeObject Time_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "datetimecpy.time",
    .tp_basicsize = sizeof(Time),
    .tp_dealloc = (destructor) Time_dealloc,
    .tp_repr = (reprfunc) Time_repr,
    .tp_str = (reprfunc) Time_repr,
    .tp_getattro = (getattrofunc) PyObject_GenericGetAttr,
    .tp_setattro = (setattrofunc) PyObject_GenericSetAttr,
    .tp_flags = (Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE),
    .tp_doc = doc_time,
    .tp_init = Time_init,
    .tp_new = Time_new,
    .tp_richcompare = (richcmpfunc)Time_richcompare,
    .tp_members = time_members,
    .tp_methods = time_methods,
    .tp_free = PyObject_Del
};

#ifdef __cplusplus
}
#endif

#endif