#include "../include/date.h"

void Date_dealloc(PyObject* self) {
    Py_CLEAR(((Date*)self)->year);
    Py_CLEAR(((Date*)self)->month);
    Py_CLEAR(((Date*)self)->day);
    Py_TYPE(self)->tp_free(self);
}

int _Date_timestamp2tm(time_t timestamp, struct tm* tm) {
    if (timestamp < 0) {
        return -1;
    }
#ifdef MS_WINDOWS
    int errorn = localtime_s(tm, &timestamp);
    if (errorn != 0) {
        return errorn;
    }
    return 0;
#else
    if (localtime_r(&timestamp, tm) == NULL) {
        return -1;
    }
    return 0;
#endif
}

PyObject* Date_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    Date* self;
    self = (Date*)type->tp_alloc(type, 0);
    if (self) {
        self->year = PyLong_FromLong(0L);
        self->month = PyLong_FromLong(0L);
        self->day = PyLong_FromLong(0L);
        return (PyObject*)self;
    }
    return NULL;
}

int Date_init(PyObject* self, PyObject* args, PyObject* kwds) {
    int errorn = 0;
    time_t now = time(NULL);
    struct tm tm;
    if (_Date_timestamp2tm(now, &tm) != 0) {
        errorn = -1;
    }

    static char* kwlist[] = {"year", "month", "day", NULL};
    PyObject* year = NULL;
    PyObject* month = NULL;
    PyObject* day = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &year, &month, &day)) {
        return -2;
    }

    // TODO: 检查范围
    if (year == NULL && errorn != -1) {
        year = PyLong_FromLong((long)(tm.tm_year + 1900));
    }

    if (month == NULL && errorn != -1) {
        month = PyLong_FromLong((long)(tm.tm_mon + 1));
    }
    if (day == NULL && errno != -1) {
        day = PyLong_FromLong((long)(tm.tm_mday));
    }

    PyObject* tmp;

    tmp = ((Date*)self)->year;
    Py_XINCREF(year);
    ((Date*)self)->year = year;
    Py_XDECREF(tmp);

    tmp = ((Date*)self)->month;
    Py_XINCREF(month);
    ((Date*)self)->month = month;
    Py_XDECREF(tmp);

    tmp = ((Date*)self)->day;
    Py_XINCREF(day);
    ((Date*)self)->day = day;
    Py_XDECREF(tmp);

    return 0;
}

PyObject* Date_repr(PyObject* self) {
    PyObject* year_repr = PyObject_Repr(((Date*)self)->year);
    PyObject* month_repr = PyObject_Repr(((Date*)self)->month);
    PyObject* day_repr = PyObject_Repr(((Date*)self)->day);
    PyObject* retval = PyUnicode_FromFormat("%s(year=%U, month=%U, day=%U)", 
        Py_TYPE(self)->tp_name, year_repr, month_repr, day_repr);
    Py_DECREF(year_repr);
    Py_DECREF(month_repr);
    Py_DECREF(day_repr);
    return retval;
}

// TODO: 总是返回false
PyObject* Date_richcompare(PyObject* self, PyObject* other, int op) {
        int diff = PyObject_RichCompareBool(((Date*)self)->year, ((Date*)other)->year, op);
        if (diff == 0) {
            diff = PyObject_RichCompareBool(((Date*)self)->month, ((Date*)other)->month, op);
            if (diff == 0) {
                diff = PyObject_RichCompareBool(((Date*)self)->day, ((Date*)other)->day, op);
            }
        }
        Py_RETURN_RICHCOMPARE(diff, 0, op);
        
}

PyObject* Date_today(PyObject* self, PyObject* Py_UNUSED(args)) {
    Date* retval = PyObject_New(Date, &Date_type);
    if (!retval) {
        return NULL;
    }

    time_t now = time(NULL);
    struct tm tm;
    if (_Date_timestamp2tm(now, &tm) != 0) {
        return NULL;
    }
    retval->year = PyLong_FromLong((long)(tm.tm_year + 1900));
    retval->month = PyLong_FromLong((long)(tm.tm_mon + 1));
    retval->day = PyLong_FromLong((long)(tm.tm_mday));
    return retval;
}

// TODO: 检查额外的format
PyObject* Date_strftime(PyObject* self, PyObject* args, PyObject* kwds) {
    PyObject* format; // Unicode类型的str
    static char* keywords[] = {"format", NULL};
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "U:strftime", keywords, &format)) {
        return NULL;
    }

    PyObject* ret = NULL;
    PyObject* format_ascii; // ASCII类型的str
    struct tm tm; // 用于格式化的tm

    const char* fmt; // 输入的format的C的字符串
    char* outbuf = NULL; // 输出的C的字符串
    size_t fmtlen, buflen;
    size_t i;

    // 将Unicode类型的str转化为ASCII类型的str
    format_ascii = PyUnicode_EncodeLocale(format, "surrogateescape");
    if (format_ascii == NULL)
        return NULL;
    // 将Python的字符串str转换为C的char*
    fmt = PyBytes_AS_STRING(format_ascii);

    memset((void*)&tm, '\0', sizeof(struct tm));
    tm.tm_year = (int)PyLong_AsUnsignedLong(((Date*)self)->year) - 1900;
    tm.tm_mon = (int)PyLong_AsUnsignedLong(((Date*)self)->month) - 1;
    tm.tm_mday = (int)PyLong_AsUnsignedLong(((Date*)self)->day);

    fmtlen = strlen(fmt);

    for (i = 1024;; i += i) {
        outbuf = (char*)PyMem_Malloc(i * sizeof(char));
        if (outbuf == NULL) {
            PyErr_NoMemory();
            break;
        }
        buflen = strftime(outbuf, i, fmt, &tm);
        if (buflen > 0 || i >= 256 * fmtlen) {
            ret = PyUnicode_DecodeLocaleAndSize(outbuf, buflen, "surrogateescape");
            PyMem_Free(outbuf);
            break;
        }
        PyMem_Free(outbuf);
    }
    Py_DECREF(format_ascii);
    return ret;
}

PyObject* Date_fromtimestamp(PyObject* self, PyObject* args, PyObject* kwds) {
    PyObject* timestamp;
     static char* keywords[] = {"timestamp", NULL};
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "O:fromtimestamp", keywords, &timestamp)) {
        return NULL;
    }

    PyObject* _timestamp = NULL; // PyLongObject
    if (PyUnicode_CheckExact(timestamp)) {
        _timestamp = PyLong_FromUnicodeObject(timestamp, 0);
    } else if (PyLong_CheckExact(timestamp)) {
        _timestamp = timestamp;
        Py_INCREF(_timestamp);
    } else {
        PyErr_Format(PyExc_TypeError, "The argument of timestamp must be int or str but %s found.", Py_TYPE(timestamp)->tp_name);
        Py_RETURN_NONE;
    }
    time_t t = (time_t)PyLong_AsLongLong(_timestamp);
    Py_XDECREF(_timestamp);
    struct tm tm;
    if (_Date_timestamp2tm(t, &tm) != 0) {
        PyErr_SetString(PyExc_RuntimeError, "Cannot parse the timestamp arg");
        Py_RETURN_NONE;
    }
    Date* retval = PyObject_New(Date, &Date_type);
    retval->year = PyLong_FromLong((long)(tm.tm_year + 1900));
    retval->month = PyLong_FromLong((long)(tm.tm_mon + 1));
    retval->day = PyLong_FromLong((long)(tm.tm_mday));
    return (PyObject*)retval;
}

