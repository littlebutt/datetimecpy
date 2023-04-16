#include <Python.h>

/*
 * Implements an example function.
 */
PyDoc_STRVAR(datetimecpy_example_doc, "example(obj, number)\
\
Example function");

PyObject *datetimecpy_example(PyObject *self, PyObject *args, PyObject *kwargs) {
    /* Shared references that do not need Py_DECREF before returning. */
    PyObject *obj = NULL;
    int number = 0;

    /* Parse positional and keyword arguments */
    static char* keywords[] = { "obj", "number", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oi", keywords, &obj, &number)) {
        return NULL;
    }

    /* Function implementation starts here */

    if (number < 0) {
        PyErr_SetObject(PyExc_ValueError, obj);
        return NULL;    /* return NULL indicates error */
    }

    Py_RETURN_NONE;
}

/*
 * List of functions to add to datetimecpy in exec_datetimecpy().
 */
static PyMethodDef datetimecpy_functions[] = {
    { "example", (PyCFunction)datetimecpy_example, METH_VARARGS | METH_KEYWORDS, datetimecpy_example_doc },
    { NULL, NULL, 0, NULL } /* marks end of array */
};

/*
 * Initialize datetimecpy. May be called multiple times, so avoid
 * using static state.
 */
int exec_datetimecpy(PyObject *module) {
    PyModule_AddFunctions(module, datetimecpy_functions);

    PyModule_AddStringConstant(module, "__author__", "luoga");
    PyModule_AddStringConstant(module, "__version__", "1.0.0");
    PyModule_AddIntConstant(module, "year", 2023);

    return 0; /* success */
}

/*
 * Documentation for datetimecpy.
 */
PyDoc_STRVAR(datetimecpy_doc, "The datetimecpy module");


static PyModuleDef_Slot datetimecpy_slots[] = {
    { Py_mod_exec, exec_datetimecpy },
    { 0, NULL }
};

static PyModuleDef datetimecpy_def = {
    PyModuleDef_HEAD_INIT,
    "datetimecpy",
    datetimecpy_doc,
    0,              /* m_size */
    NULL,           /* m_methods */
    datetimecpy_slots,
    NULL,           /* m_traverse */
    NULL,           /* m_clear */
    NULL,           /* m_free */
};

PyMODINIT_FUNC PyInit_datetimecpy() {
    return PyModuleDef_Init(&datetimecpy_def);
}
