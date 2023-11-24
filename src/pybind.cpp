#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/stl_bind.h>
#include "kernelmatrix.h"

namespace py = pybind11;

PYBIND11_MODULE(fsgm, m) {
    m.doc() = "Fast computation of Similartiy Gram Kernel Matrices";

    m.def("compute_kernel_matrix", &ComputeKernelSimilarityMatrix, py::return_value_policy::reference_internal);
}