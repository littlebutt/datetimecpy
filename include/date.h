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

    PyObject* year;                 // year

    PyObject* month;                // month

    PyObject* day;                  // day

} Date;

void Date_dealloc(PyObject* self);                                       // destruction method

PyObject* Date_new(PyTypeObject* type, PyObject* args, PyObject* kwds);  // __new__ method

int Date_init(PyObject* self, PyObject* args, PyObject* kwds);           // __init__ method

PyObject* Date_repr(PyObject* self);                                     // __repr__ method

PyObject* Date_richcompare(PyObject* self, PyObject* other, int op);     // __lt__, __le__, __eq__, __ne__, __gt__, __ge__ methods

PyObject* Date_today(PyObject* self, PyObject* Py_UNUSED(args));

PyObject* Date_strftime(PyObject* self, PyObject* args, PyObject* kwds);

PyObject* Date_fromtimestamp(PyObject* self, PyObject* args, PyObject* kwds);

PyObject* Date_timetuple(PyObject* self, PyObject* Py_UNUSED(args));

static PyMemberDef date_members[] = {
    {"year", T_OBJECT_EX, offsetof(Date, year), 0, PyDoc_STR("year")},
    {"month", T_OBJECT_EX, offsetof(Date, month), 0, PyDoc_STR("month")},
    {"day", T_OBJECT_EX, offsetof(Date, day), 0, PyDoc_STR("day")},
    {NULL}
};

static PyMethodDef date_methods[] = {
    {"today", Date_today, METH_NOARGS | METH_CLASS, PyDoc_STR("today\n-\n\n Get the current date.")},
    {"strftime", Date_strftime, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("strftime\n-\n\n Format the given date.")},
    {"fromtimestamp", Date_fromtimestamp, METH_VARARGS | METH_KEYWORDS | METH_CLASS, PyDoc_STR("fromtimestamp\n-\n\n Get the date from float-type timestamp")},
    {"timetuple", Date_timetuple, METH_NOARGS, PyDoc_STR("timetuple\n-\n\n Get the tuple-like value of the current date.") },
    {NULL, NULL}
};

PyDoc_STRVAR(doc_date, 
"date\n-\n\n\
The date implementation for datetimecpy");

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
    .tp_members = date_members,
    .tp_methods = date_methods,
    .tp_free = PyObject_Del
};

#ifdef __cplusplus
}
#endif

#endif // DATE_H