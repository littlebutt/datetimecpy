#ifndef DATE_H
#define DATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include <time.h>

static PyTypeObject Date_type;

typedef struct {
    PyObject_HEAD

    long long timestamp;

} Date;

void Date_dealloc(PyObject* self);                                       // destruction method

PyObject* Date_new(PyTypeObject* type, PyObject* args, PyObject* kwds);  // __new__ method

int Date_init(PyObject* self, PyObject* args, PyObject* kwds);           // __init__ method

PyObject* Date_repr(PyObject* self);                                     // __repr__ method

PyObject* Date_richcompare(PyObject* self, PyObject* other, int op);     // __lt__, __le__, __eq__, __ne__, __gt__, __ge__ methods

PyObject* Date_today(PyObject* self, PyObject* Py_UNUSED(args));

PyObject* Date_totimestamp(PyObject* self, PyObject* Py_UNUSED(args));

static PyMethodDef date_methods[] = {
    {"today", Date_today, METH_NOARGS | METH_CLASS, PyDoc_STR("today\n-\n\n Get the current date.")},
    {"totimestamp", Date_totimestamp, METH_NOARGS, PyDoc_STR("totimetamp\n-\n\n Get the current timestamp.")},
    {NULL, NULL}
};

PyDoc_STRVAR(doc_date, 
"Date\n-\n\n\
The Date implementation for datetimecpy");

static PyTypeObject Date_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "datetimecpy.date",
    .tp_basicsize = sizeof(Date),
    .tp_dealloc = (destructor) Date_dealloc,
    .tp_repr = (reprfunc) Date_repr,
    .tp_str = (reprfunc) Date_repr,
    .tp_getattro = (getattrofunc) PyObject_GenericGetAttr,
    .tp_setattro = (setattrofunc) PyObject_GenericSetAttr,
    .tp_flags = (Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE),
    .tp_doc = doc_date,
    .tp_init = Date_init,
    .tp_new = Date_new,
    .tp_richcompare = (richcmpfunc)Date_richcompare,
    .tp_methods = date_methods,
    .tp_free = PyObject_Del
};

#ifdef __cplusplus
}
#endif

#endif // DATE_H