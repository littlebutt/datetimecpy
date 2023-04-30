#ifndef TIMEDELTA_H
#define TIMEDELTA_H

#ifdef __cplusplus
extern "C" {
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

/*
 * timedelta内部成员只有三个：days，seconds和microseconds
 * 它们对应的范围分别为[-999999999, 999999999]，[0, 86399]，[0, 999999]
 * 因此可以用一个char数组存放所有数据，分别各占四个字节
*/
struct timedelta_buf {
    unsigned char buf[12];
};

struct timedelta_buf* timedelta_buf_new(long days, long seconds, long microseconds);

void timedelta_buf_delete(struct timedelta_buf* td_buf);

long timedelta_buf_get_days(struct timedelta_buf* td_buf);

long timedelta_buf_get_seconds(struct timedelta_buf* td_buf);

long timedelta_buf_get_microseconds(struct timedelta_buf* td_buf);

typedef struct {
    PyObject_HEAD

    struct timedelta_buf* timedelta;

} TimedeltaExporter;

void TimedeltaExporter_dealloc(PyObject* self);                                       // destruction method

int TimedeltaExporter_getbuffer(TimedeltaExporter* exporter, Py_buffer* view, int flag);

void TimedeltaExporter_releasebuffer(TimedeltaExporter* exporter, Py_buffer* view);

static PyBufferProcs TimedeltaExporter_as_buffer = {
    (getbufferproc) TimedeltaExporter_getbuffer,
    (releasebufferproc) TimedeltaExporter_releasebuffer
};

static PyTypeObject TimedeltaExporter_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "datetimecpy._timedelta_exporter",
    .tp_basicsize = sizeof(TimedeltaExporter),
    .tp_dealloc = (destructor) TimedeltaExporter_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_as_buffer = &TimedeltaExporter_as_buffer
};

typedef struct {
    PyObject_HEAD

    PyObject* exporter;

} Timedelta;

void Timedelta_dealloc(PyObject* self);                                       // destruction method

int Timedelta_init(PyObject* self, PyObject* args, PyObject* kwds);           // __init__ method

PyObject* Timedelta_repr(PyObject* self);                                     // __repr__ method

#ifdef __cplusplus
}
#endif

#endif