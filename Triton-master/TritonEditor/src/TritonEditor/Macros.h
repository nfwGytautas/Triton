#pragma once

#define TR_GET_EDTR_STATE(edtrClass) edtrClass = this->getClassByName("editorState").as<Triton::EditorState>();
#define TR_EDTR_STATE(name) Triton::reference<Triton::EditorState> name;