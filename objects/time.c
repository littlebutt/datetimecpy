#include "../include/time.h"

PyObject* _Time_get_module_attr(const char* mod, const char* attr) {
    PyObject* pmodname = PyUnicode_FromString(mod);
    if (pmodname == NULL) {
        return NULL;
    }
    PyObject* pattrname = PyUnicode_FromString(attr);
    if (pattrname == NULL) {
        Py_DECREF(pmodname);
        return NULL;
    }
    PyObject* m = PyImport_Import(pmodname);
    if (m == NULL) {
        return NULL;
    }
    PyObject* result = PyObject_GetAttr(m, pattrname);
    Py_DECREF(m);
    return result;
}

PyObject* _Time_get_default_timetuple() {
    struct tm tm;
    time_t t = time(NULL);
#ifdef MS_WINDOWS
    int errorn = localtime_s(&tm, &t);
    if (errorn != 0) {
        return NULL;
    }
#else
    if (localtime_r(&t, &tm) == NULL) {
        return NULL;
    }
#endif
    PyTupleObject* ret = PyTuple_New(9);
    if (ret == NULL) {
        return NULL;
    }
    PyTuple_SetItem(ret, 0, PyLong_FromLong((long)tm.tm_year + 1900L));
    PyTuple_SetItem(ret, 1, PyLong_FromLong((long)tm.tm_mon + 1L));
    PyTuple_SetItem(ret, 2, PyLong_FromLong((long)tm.tm_mday));
    PyTuple_SetItem(ret, 3, PyLong_FromLong((long)tm.tm_hour));
    PyTuple_SetItem(ret, 4, PyLong_FromLong((long)tm.tm_min));
    PyTuple_SetItem(ret, 5, PyLong_FromLong((long)tm.tm_sec));
    PyTuple_SetItem(ret, 6, PyLong_FromLong((long)(tm.tm_wday + 6) % 7));
    PyTuple_SetItem(ret, 7, PyLong_FromLong((long)tm.tm_yday));
    PyTuple_SetItem(ret, 8, PyLong_FromLong((long)tm.tm_isdst));
    return ret;
}

PyObject* Time_strftime(PyObject* self, PyObject* args, PyObject* kwds) {
    PyObject* format = NULL;
    PyObject* t = NULL;
    static char* kwlist[] = {"format", "t", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|O:strftime", kwlist, &format, &t)) {
        return NULL;
    }
    if (t == NULL) {
        t = _Time_get_default_timetuple();
        if (t == NULL) {
            return NULL;
        }
    }
    PyObject* result = NULL;
    PyObject* strftime = _Time_get_module_attr("time", "strftime");
    if (strftime == NULL) {
        return NULL;
    }
    result = PyObject_CallFunctionObjArgs(strftime, format, t, NULL);
    
    Py_DECREF(strftime);
    return result;
}