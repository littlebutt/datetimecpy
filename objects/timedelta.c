#include "../include/timedelta.h"

struct timedelta_buf* timedelta_buf_new() {
    struct timedelta_buf* td_buf = (struct timedelta_buf*)PyMem_RawMalloc(10 * sizeof(char));
    if (td_buf == NULL) {
        return NULL;
    }
    memset(td_buf->buf, '\0', 10 * sizeof(char));
    return td_buf;
}

void timedelta_buf_delete(struct timedelta_buf* td_buf) {
    if (td_buf == NULL) {
        return;
    }
     PyMem_RawFree(td_buf);
}

long timedelta_buf_get_days(struct timedelta_buf* td_buf) {
    return (long)(td_buf->buf[0] << 24 | td_buf->buf[1] << 16 | td_buf->buf[2] << 8 | td_buf->buf[3]);
}

long timedelta_buf_get_seconds(struct timedelta_buf* td_buf) {
    return (long)(td_buf->buf[4] << 16 | td_buf->buf[5] << 8 | td_buf->buf[6]);
}

long timedelta_buf_get_microseconds(struct timedelta_buf* td_buf) {
    return (long)(td_buf->buf[7] << 16 | td_buf->buf[8] << 8 | td_buf->buf[9]);
}

int timedelta_buf_set_days(struct timedelta_buf* td_buf, long days) {
    if (td_buf == NULL) {
        return -1;
    }
    td_buf->buf[0] = (days & 0xff000000) >> 24;
    td_buf->buf[1] = (days & 0x00ff0000) >> 16;
    td_buf->buf[2] = (days & 0x0000ff00) >> 8;
    td_buf->buf[3] = days & 0x000000ff;
    return 0;
}

int timedelta_buf_set_seconds(struct timedelta_buf* td_buf, long seconds) {
    if (td_buf == NULL) {
        return -1;
    }
    td_buf->buf[4] = (seconds & 0xff000000) >> 16;
    td_buf->buf[5] = (seconds & 0x00ff0000) >> 8;
    td_buf->buf[6] = (seconds & 0x0000ff00) >> 0;
    return 0;
}

int timedelta_buf_set_microseconds(struct timedelta_buf* td_buf, long microseconds) {
    if (td_buf == NULL) {
        return -1;
    }
    td_buf->buf[7] = (microseconds & 0xff000000) >> 16;
    td_buf->buf[8] = (microseconds & 0x00ff0000) >> 8;
    td_buf->buf[9] = (microseconds & 0x0000ff00) >> 0;
    return 0;
}

void TimedeltaExporter_dealloc(PyObject* self) {
    timedelta_buf_delete(((TimedeltaExporter*)self)->timedelta);
    Py_TYPE(self)->tp_free(self);
}

int TimedeltaExporter_getbuffer(TimedeltaExporter* exporter, Py_buffer* view, int flag) {
    struct timedelta_buf* buf = (struct timedelta_buf*)exporter->timedelta;
    if (buf == NULL || exporter->exports == 0) {
        buf = timedelta_buf_new();
        if (buf == NULL) {
            return -1;
        }
    }
    if (view == NULL) {
        return -1;
    }
    PyBuffer_FillInfo(view, exporter, buf, sizeof(buf), 0, flag);
    if (PyErr_Occurred()) {
        PyErr_Print();
    }
    exporter->exports ++;
    return 0;
}

void TimedeltaExporter_releasebuffer(TimedeltaExporter* exporter, Py_buffer* view) {
    timedelta_buf_delete(exporter->timedelta);
    exporter->exports --;
}

void Timedelta_dealloc(PyObject* self) {
    PyBuffer_Release(((Timedelta*)self)->exporter);
    Py_CLEAR(((Timedelta*)self)->exporter);
    Py_TYPE(self)->tp_dealloc(self);
}

int Timedelta_init(PyObject* self, PyObject* args, PyObject* kwds) {
    long days = 0;
    long seconds = 0;
    long microseconds = 0;
    long milliseconds = 0;
    long minutes = 0;
    long hours = 0;
    long weeks = 0;
    static char* kwlist[] = {"days", "seconds", "microseconds", "milliseconds", "minutes", "hours", "weeks", NULL};
    if (PyArg_ParseTupleAndKeywords(args, kwds, "|lllllll", kwlist, 
                                    &days, &seconds, &microseconds, 
                                    &milliseconds, &minutes, &hours, &weeks));
    
    days += weeks * 7;
    seconds += hours * 3600 + minutes * 60;
    microseconds += milliseconds * 1000;
    TimedeltaExporter* exporter = PyObject_New(TimedeltaExporter, &TimedeltaExporter_type);
    exporter->exports = 0;
    exporter->timedelta = NULL;
    Py_buffer buffer = {NULL, NULL};
    if (PyObject_GetBuffer(exporter, &buffer, PyBUF_WRITABLE) < 0) {
        return -1;
    }
    struct timedelta_buf* buf = (struct timedelta_buf*) buffer.buf;
    timedelta_buf_set_days(buf, days);
    timedelta_buf_set_seconds(buf, seconds);
    timedelta_buf_set_microseconds(buf, microseconds);
    exporter->timedelta = buf;
    ((Timedelta*)self)->exporter = exporter;
    Py_INCREF(exporter);
    Py_INCREF(self);
    return 0;
}

PyObject* Timedelta_repr(PyObject* self) {
    TimedeltaExporter* exporter = ((Timedelta*)self)->exporter;
    Py_buffer buffer = {NULL, NULL};
    if (PyObject_GetBuffer(exporter, &buffer, PyBUF_WRITABLE) < 0) {
        return NULL;
    }
    struct timedelta_buf* _buf = (struct timedelta_buf*)buffer.buf;
    long days = timedelta_buf_get_days(_buf);
    long seconds = timedelta_buf_get_seconds(_buf);
    long microseconds = timedelta_buf_get_microseconds(_buf);
    return PyUnicode_FromFormat("%s(days=%ld, seconds=%ld, microseconds=%ld)", 
        Py_TYPE(self)->tp_name, days, seconds, microseconds);

}

