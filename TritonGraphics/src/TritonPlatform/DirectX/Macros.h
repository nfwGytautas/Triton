#pragma once

#define GL_CONTEXT(x) static_cast<Triton::PType::DXContext*>(x)

#define OBJECT_AS(type, obj) dynamic_cast<type*>(obj);