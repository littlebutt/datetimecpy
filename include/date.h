#ifndef DATE_H
#define DATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

static PyTypeObject Date_type;

typedef struct {
    PyObject_HEAD

    char* timestamp;

} Date;

static void Date_dealloc(PyObject* self);                                       // destruction method

static PyObject* Date_new(PyTypeObject* type, PyObject* args, PyObject* kwds);  // __new__ method

static int Date_init(PyObject* self, PyObject* args, PyObject* kwds);                  // __init__ method

static PyObject* Date_repr(PyObject* self);                                     // __repr__ method

static PyObject* Date_richcompare(PyObject* self, PyObject* other, int op);            // __lt__, __le__, __eq__, __ne__, __gt__, __ge__ methods

#ifdef __cplusplus
}
#endif

#endif // DATE_H