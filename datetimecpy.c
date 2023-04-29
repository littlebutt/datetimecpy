#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <time.h>
#include "include/date.h"
#include "include/time.h"

PyDoc_STRVAR(datetimecpy_doc, "The datetimecpy module");

static PyModuleDef datetimecpy_def = {
    PyModuleDef_HEAD_INIT,
    "datetimecpy",
    datetimecpy_doc,
    -1,                                 /* m_size */
    NULL,                               /* m_methods */
    NULL,                               /* m_slots */
    NULL,                               /* m_traverse */
    NULL,                               /* m_clear */
    NULL,                               /* m_free */
};

PyMODINIT_FUNC PyInit_datetimecpy() {
    if (PyType_Ready(&Date_type) < 0) {
        return NULL;
    }
    PyObject* m = PyModule_Create(&datetimecpy_def);
    if (!m)
    {
        return NULL;
    }
    PyModule_AddStringConstant(m, "__author__", "littlebutt");
    PyModule_AddStringConstant(m, "__version__", "1.0.0");
    Py_INCREF(&Date_type);
    if (PyModule_AddObject(m, "date", (PyObject*)&Date_type) < 0)
    {
        Py_DECREF(&Date_type);
        Py_DECREF(m);
        return NULL;
    }
    PyObject* time_mod = PyModule_Create(&time_def);
    if (time_mod != NULL) {
        Py_INCREF(time_mod);
        if (PyModule_AddObject(m, "time", time_mod) < 0) {
            Py_DECREF(time_mod);
            Py_DECREF(&Date_type);
            Py_DECREF(m);
            return NULL;
        }
    }
    return m;
}
