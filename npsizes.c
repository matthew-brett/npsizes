/* Python extension to return object tp_basicsize
 */
#include <Python.h>

#include "numpy/arrayobject.h"
#include "numpy/ufuncobject.h"

static size_t get_obj_size(const char *module_name, const char *class_name);


static PyObject* get_mem_size(PyObject* self, PyObject *args)
{
    const char *module_name;
    const char *class_name;
    if (!PyArg_ParseTuple(args, "ss", &module_name, &class_name)) {
      return NULL;
    }
    return Py_BuildValue("i", get_obj_size(module_name, class_name));
}


static PyObject* get_def_sizes(PyObject* self)
{
    return Py_BuildValue("iii",
            sizeof(PyArray_Descr),
            sizeof(PyArrayObject),
            sizeof(PyUFuncObject));
}


static char get_mem_size_docs[] =
    "get_mem_size(modname, classnem): return object sizes in memory\n";

static char get_def_sizes_docs[] =
    "get_def_sizes(): return numpy object sizes in header\n";


static PyMethodDef npsizes_funcs [] = {
    {"get_mem_size", (PyCFunction)get_mem_size, METH_VARARGS, get_mem_size_docs},
    {"get_def_sizes", (PyCFunction)get_def_sizes, METH_NOARGS, get_def_sizes_docs},
    {NULL}
};


void initnpsizes(void)
{
    Py_InitModule3("npsizes", npsizes_funcs,
                   "Get numpy object sizes");
}


/* cython boilerplate */

#if !defined(__Pyx_PyIdentifier_FromString)
#if PY_MAJOR_VERSION < 3
  #define __Pyx_PyIdentifier_FromString(s) PyString_FromString(s)
#else
  #define __Pyx_PyIdentifier_FromString(s) PyUnicode_FromString(s)
#endif
#endif

static PyObject *__Pyx_ImportModule(const char *name) {
    PyObject *py_name = 0;
    PyObject *py_module = 0;
    py_name = __Pyx_PyIdentifier_FromString(name);
    if (!py_name)
        goto bad;
    py_module = PyImport_Import(py_name);
    Py_DECREF(py_name);
    return py_module;
bad:
    Py_XDECREF(py_name);
    return 0;
}


static size_t get_obj_size(const char *module_name, const char *class_name)
{
    PyObject *py_module = 0;
    PyObject *result = 0;
    size_t obj_size;
    PyObject *py_name = 0;
    py_module = __Pyx_ImportModule(module_name);
    if (!py_module)
        goto bad;
    py_name = __Pyx_PyIdentifier_FromString(class_name);
    if (!py_name)
        goto bad;
    result = PyObject_GetAttr(py_module, py_name);
    Py_DECREF(py_name);
    py_name = 0;
    Py_DECREF(py_module);
    py_module = 0;
    if (!result)
        goto bad;
    if (!PyType_Check(result)) {
        PyErr_Format(PyExc_TypeError,
            "%s.%s is not a type object",
            module_name, class_name);
        goto bad;
    }
    obj_size = (size_t)((PyTypeObject *)result)->tp_basicsize;
    Py_XDECREF(result);
    return obj_size;
bad:
    Py_XDECREF(py_module);
    Py_XDECREF(result);
    return 0;
}
