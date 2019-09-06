#pragma once

#define GL_CONTEXT(x) static_cast<Triton::PType::GLContext*>(x)

#define GL_CONTEXT_FROM_GLFW_WINDOW(window) PType::GLContext& context = *(PType::GLContext*)glfwGetWindowUserPointer(window)

#define OBJECT_AS(type, obj) dynamic_cast<type*>(obj);