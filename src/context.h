#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Context)
    class Context {
    public:
        static ContextUPtr Create();
        void CreateCircle(float radius, float radiusin, int segment,float circleDegreeStart, float circleDegreeEnd, float circle_colorRad,
    float circle_colorGreen, float circle_colorBlue);
        void Render();    
    private:
        Context() {}
        bool Init();
        ProgramUPtr m_program;

        	
        VertexLayoutUPtr m_vertexLayout;
        BufferUPtr m_vertexBuffer;
        BufferUPtr m_indexBuffer;
        int m_indexCount { 6 };
};

#endif // __CONTEXT_H__