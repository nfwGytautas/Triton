#pragma once

#define TR_GET_CLASS(x) Triton::reference<Triton::Core::TritonClass> x = this->getClassByID((size_t)Triton::Core::TritonClasses::x);