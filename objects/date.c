#include "../include/date.h"

void Date_dealloc(PyObject* self) {
    Py_TYPE(self)->tp_free(self);
}

PyObject* Date_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    Date* self;
    self = (Date*)type->tp_alloc(type, 0);
    if (self) {
        time_t now = time(NULL);
        self->timestamp = (long long)now;
    }
    return (PyObject*)self;
}

int Date_init(PyObject* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"timestamp", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|L", kwlist, &((Date*)self)->timestamp)) {
        return -1;
    }
    return 0;
}

PyObject* Date_repr(PyObject* self) {
    return PyUnicode_FromFormat("%s(%lld)", Py_TYPE(self)->tp_name, ((Date*)self)->timestamp);
}

PyObject* Date_richcompare(PyObject* self, PyObject* other, int op) {
        long long diff = ((Date*)self)->timestamp - ((Date*)other)->timestamp;
        Py_RETURN_RICHCOMPARE(diff, 0, op);
}
