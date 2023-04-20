#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <time.h>

PyObject*
datetimecpy_now(PyObject* self, PyObject* Py_UNUSED(args)) {
    time_t now = time(NULL);

    if (now < 0) {
        Py_RETURN_NONE;
    } else {
        return PyLong_FromLongLong((long long)now);
    }
}

static PyMethodDef datetimecpy_functions[] = {
    { "now", (PyCFunction)datetimecpy_now, METH_NOARGS, PyDoc_STR("now\n-\n\nQuery the current timestamp from the os.") },
    { NULL, NULL, 0, NULL }
};

PyDoc_STRVAR(datetimecpy_doc, "The datetimecpy module");

static PyModuleDef datetimecpy_def = {
    PyModuleDef_HEAD_INIT,
    "datetimecpy",
    datetimecpy_doc,
    -1,                                 /* m_size */
    datetimecpy_functions,              /* m_methods */
    NULL,                               /* m_slots */
    NULL,                               /* m_traverse */
    NULL,                               /* m_clear */
    NULL,                               /* m_free */
};

PyMODINIT_FUNC PyInit_datetimecpy() {
    PyObject* m = PyModule_Create(&datetimecpy_def);
    if (!m)
    {
        return NULL;
    }
    PyModule_AddStringConstant(m, "__author__", "littlebutt");
    PyModule_AddStringConstant(m, "__version__", "1.0.0");
    return m;
}
